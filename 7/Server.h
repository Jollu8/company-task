//
// Created by Jollu Emil
//



#pragma once

#include <netinet/in.h>
#include <openssl/sha.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <condition_variable>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <vector>
#include <regex>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <openssl/sha.h>

// SHA1
std::string CalculateSHA1(const std::string& input) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);

    std::ostringstream oss;
    for (unsigned char i : hash)
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
    return oss.str();
}

// Функция для подсчета hitcount
void CountHits(const std::string& input, std::unordered_map<std::string, int>& hit_count_Map) {
    auto it = hit_count_Map.find(input);
    if (it != hit_count_Map.end()) {
        ++it->second;
    } else {
        hit_count_Map[input] = 1;
    }
}

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

namespace threadpool {
class Threadpool final {
public:
    explicit Threadpool(size_t threads_count) : m_stop_pool(false) {
        for (auto i = 0u; i < threads_count; ++i) {
            m_workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->m_queue_mutex);
                        this->m_condition.wait(lock, [this] { return this->m_stop_pool || !this->m_tasks.empty(); });
                        if (this->m_stop_pool && this->m_tasks.empty())
                            return;
                        task = std::move(this->m_tasks.front());
                        this->m_tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~Threadpool() {
        {
            std::lock_guard<std::mutex> lock(m_queue_mutex);
            m_stop_pool = true;
        }
        m_condition.notify_all();
        for (std::thread& worker : m_workers)
            worker.join();
    }

    template <class Fn, class... Arg>
    void Enqueue(Fn&& fn, Arg&&... args) {
        {
            std::lock_guard<std::mutex> lock(m_queue_mutex);
            if (m_stop_pool)
                throw std::runtime_error("Enqueue on stopped ThreadPool");
            m_tasks.emplace([fn = std::forward<Fn>(fn), ... args = std::forward<Arg>(args)]() mutable {
                    std::invoke(std::forward<Fn>(fn), std::forward<Arg>(args)...);
                });
        }
        m_condition.notify_all();
    }

private:
    std::mutex m_queue_mutex;
    std::condition_variable m_condition;
    bool m_stop_pool;
    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;
};

}  // namespace threadpool


std::string ExtractPath(const std::string& request) {
    std::regex path_regex(R"(GET\s(.*?)\sHTTP)");
    std::smatch match;
    if (std::regex_search(request, match, path_regex) && match.size() > 1) {
        return match.str(1);
    } else {
        return {};
    }
}

std::string ExtractUserAgent(const std::string& request) {
    std::regex ua_regex(R"(User-Agent:\s(.*))");
    std::smatch match;
    if (std::regex_search(request, match, ua_regex) && match.size() > 1) {
        return match.str(1);
    } else {
        return {};
    }
}

void ProcessRequest(int client_socket, threadpool::Threadpool& pool,
                    std::unordered_map<std::string, int>& hit_count_Map) {
    char buffer[BUFFER_SIZE];
    std::string reqstr;
    ssize_t bytes_received;

    // Read from the socket until we find "\r\n\r\n"
    while (reqstr.find("\r\n\r\n") == std::string::npos) {
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
            std::cerr << "Error reading from socket. " << std::endl;
            close(client_socket);
            return;
        }
        if (bytes_received == 0) {
            std::cerr << "Client closed connection. " << std::endl;
            close(client_socket);
            return;
        }
        buffer[bytes_received] = '\0';
        reqstr += buffer;
    }

    // Check if the request is a GET request
    if (reqstr.substr(0, 3) != "GET") {
        std::cerr << "Received non-GET request. Ignoring.\n";
        close(client_socket);
        return;
    }

    if (!reqstr.empty()) {
        // Send a simple HTTP response
        const char response[] = "HTTP/1.1 200 OK\r\nContent-Length: 7\r\nConnection: close\r\n\r\nqueued\n";
        auto bytes_sent = send(client_socket, response, sizeof(response), 0);
        if (bytes_sent < 0) {
            std::cerr << "Error sending response. " << std::endl;
            close(client_socket);
            return;
        }
        if (bytes_sent < static_cast<ssize_t >(sizeof(response))) {
            std::cerr << "Not all bytes sent. " << std::endl;
            close(client_socket);
            return;
        }

        // Extract path and User-Agent from the request
        std::string path = ExtractPath(reqstr);
        std::string userAgent = ExtractUserAgent(reqstr);

        pool.Enqueue([&]{
            std::cout << reqstr << std::endl;
            // Calculate SHA1 and hitcount
            std::string pathSHA1 = CalculateSHA1(path);
            std::string userAgentSHA1 = CalculateSHA1(userAgent);
            CountHits(path, hit_count_Map);
            CountHits(userAgent, hit_count_Map);
            int path_Hit_count = hit_count_Map[path];
            int user_Agent_Hit_count = hit_count_Map[userAgent];
            std::cout << "Thread ID: " << std::this_thread::get_id() << "\n";
            std::cout << "Path: " << path << ", SHA1: " << pathSHA1 << ", Hitcount: " << path_Hit_count << "\n";
            std::cout << "User-Agent: " << userAgent << ", SHA1: " << userAgentSHA1
                      << ", Hitcount: " << user_Agent_Hit_count << "\n";
        });
    }

    // Close the client socket
    close(client_socket);
}


