//
// Created by Jollu Emil
//

#pragma once
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <unordered_map>

class Server {
private:
    std::queue<std::pair<std::string, std::string>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    std::unordered_map<std::string, int> pathHitCount;
    std::unordered_map<std::string, int> userAgentHitCount;

    static std::string sha1(const std::string &input) {
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1((unsigned char*)input.c_str(), input.size(), hash);
        char buf[SHA_DIGEST_LENGTH*2+1];
        for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
            snprintf(buf+i*2, 3, "%02x", hash[i]);
        return {buf};
    }

    void worker(int id) {
        std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
        while (true) {
            lock.lock();
            cv.wait(lock, [this]{ return !tasks.empty(); });
            std::pair<std::string, std::string> task = tasks.front();
            tasks.pop();
            lock.unlock();

            std::string pathSha1 = sha1(task.first);
            std::string userAgentSha1 = sha1(task.second);

            lock.lock();
            ++pathHitCount[task.first];
            ++userAgentHitCount[task.second];
            int pathHits = pathHitCount[task.first];
            int userAgentHits = userAgentHitCount[task.second];
            lock.unlock();

            std::cout << "Thread " << id << ": " << task.first << " -> " << pathSha1 << " " << pathHits << " " << task.second << " -> " << userAgentSha1 << " " << userAgentHits << std::endl;
        }
    }

public:
    int getPathHitCount(const std::string& path) {
        std::lock_guard<std::mutex> lock(mtx);
        return pathHitCount[path];
    }

    int getUserAgentHitCount(const std::string& userAgent) {
        std::lock_guard<std::mutex> lock(mtx);
        return userAgentHitCount[userAgent];
    }

    void run() {
        std::array<std::thread,5 > workers;

        for (int i = 0; i < 5; i++)
            workers[i] = std::thread(&Server::worker, this, i);

        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in servaddr{};
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(8080);
        bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
        listen(sockfd, 5);

        while (true) {
            sockaddr_in cliaddr  {};
            socklen_t len = sizeof(cliaddr);
            int connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);

            char buffer[1024] = {0};
            read(connfd, buffer, sizeof(buffer));
            std::string request(buffer);


            std::string path = request.substr(0, request.find(" "));
            std::string userAgent = request.substr(request.find("User-Agent: ") + 12);
            userAgent = userAgent.substr(0, userAgent.find("\r\n"));

            std::lock_guard<std::mutex> lock(mtx);
            tasks.push({path, userAgent});
            cv.notify_all();

            std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nqueued";
            write(connfd, response.c_str(), response.size());
            close(connfd);
        }

        for (auto & worker : workers)
            worker.join();
    }

};





