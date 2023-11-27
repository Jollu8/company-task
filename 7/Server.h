//
// Created by Jollu Emil
//

#pragma once

#include <arpa/inet.h>
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
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#define TEST_false

// constexpr int PORT = 8080;
// constexpr int BUFFER_SIZE = 1024;

namespace threadpool {
class Threadpool final {
public:
    friend class ServerTest;
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

class Server {
public:



    Server(int port, int buffer_size) : m_PORT(port), m_BUFFER_SIZE(buffer_size) {
        m_server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_server_socket == -1) {
            throw std::runtime_error("Error creating socket.");
        }

        sockaddr_in server_address{};
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = INADDR_ANY;
        server_address.sin_port = htons(m_PORT);

        if (bind(m_server_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == -1) {
            close(m_server_socket);
            throw std::runtime_error("Error binding socket.");
        }

        if (listen(m_server_socket, 10) == -1) {
            close(m_server_socket);
            throw std::runtime_error("Error listening on socket.");
        }

        std::cout << "Server listening on port: " << m_PORT << "..." << std::endl;
    }
    Server() : Server(8080, 1024) {}

    ~Server() {
        close(m_server_socket);
    }

    void Run() {
        while (true) {
            sockaddr_in client_address{};
            socklen_t client_address_len = sizeof(client_address);
            int client_socket =
                accept(m_server_socket, reinterpret_cast<sockaddr*>(&client_address), &client_address_len);

            if (client_socket == -1) {
                std::cerr << "Error accepting connection. " << std::endl;
                continue;
            }

            ProcessRequest(client_socket, m_pool, m_hit_count_Map);
        }
    }


private:
    // SHA1
    static std::string CalculateSHA1(const std::string& input) {
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);

        std::ostringstream oss;
        for (unsigned char i : hash)
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
        return oss.str();
    }
    // Функция для подсчета hitcount
    static void CountHits(const std::string& input, std::unordered_map<std::string, int>& hit_count_Map) {
        auto it = hit_count_Map.find(input);
        if (it != hit_count_Map.end()) {
            ++it->second;
        } else {
            hit_count_Map[input] = 1;
        }
    }

    static std::string ExtractPath(const std::string& request) {
        std::regex path_regex(R"(GET\s(.*?)\sHTTP)");
        std::smatch match;
        if (std::regex_search(request, match, path_regex) && match.size() > 1)
            return match.str(1);
        else
            return {};
    }

    static std::string ExtractUserAgent(const std::string& request) {
        std::regex ua_regex(R"(User-Agent:\s(.*))");
        std::smatch match;
        if (std::regex_search(request, match, ua_regex) && match.size() > 1)
            return match.str(1);
        else
            return {};
    }

    void ProcessRequest(int client_socket, threadpool::Threadpool& pool,
                        std::unordered_map<std::string, int>& hit_count_Map) {
        std::string reqstr = ReceiveRequest(client_socket);
        if (reqstr.empty()) {
            return;
        }

        // Проверка на GET-запрос
        if (reqstr.substr(0, 3) != "GET") {
            std::cerr << "Only GET requests are supported. " << std::endl;
            close(client_socket);
            return;
        }

        SendResponse(client_socket);

        std::string path = ExtractPath(reqstr);
        std::string userAgent = ExtractUserAgent(reqstr);

        ProcessData(client_socket, pool, hit_count_Map, path, userAgent);
    }

    [[nodiscard]] std::string ReceiveRequest(int client_socket) const {
        std::unique_ptr<char[]> buffer(new char[m_BUFFER_SIZE]);
        std::string reqstr;
        ssize_t bytes_received;

        // Чтение из сокета, пока не найдем "\r\n\r\n"
        while (reqstr.find("\r\n\r\n") == std::string::npos) {
            bytes_received = recv(client_socket, buffer.get(), m_BUFFER_SIZE - 1, 0);
            if (bytes_received < 0) {
                std::cerr << "Error reading from socket. " << std::endl;
                close(client_socket);
                return {};
            }
            if (bytes_received == 0) {
                std::cerr << "Client closed connection. " << std::endl;
                close(client_socket);
                return {};
            }
            buffer[bytes_received] = '\0';
            reqstr += buffer.get();
        }

        return reqstr;
    }

    static void SendResponse(int client_socket) {
        const char response[] = "HTTP/1.1 200 OK\r\nContent-Length: 7\r\nConnection: close\r\n\r\nqueued\n";
        auto bytes_sent = send(client_socket, response, sizeof(response), 0);
        if (bytes_sent < 0) {
            std::cerr << "Error sending response. " << std::endl;
            close(client_socket);
        }
        if (bytes_sent < static_cast<ssize_t>(sizeof(response))) {
            std::cerr << "Not all bytes sent. " << std::endl;
            close(client_socket);
        }
    }

    static void ProcessData(int client_socket, threadpool::Threadpool& pool,
                     std::unordered_map<std::string, int>& hit_count_Map, const std::string& path,
                     const std::string& userAgent) {
        pool.Enqueue([&] {
            std::cout << "Thread ID: " << std::this_thread::get_id() << "\n";
            // Вычисление SHA1 и подсчет hitcount
            std::string pathSHA1 = CalculateSHA1(path);
            std::string userAgentSHA1 = CalculateSHA1(userAgent);
            CountHits(path, hit_count_Map);
            CountHits(userAgent, hit_count_Map);
            int path_Hit_count = hit_count_Map[path];
            int user_Agent_Hit_count = hit_count_Map[userAgent];
            std::cout << "Path: " << path << ", SHA1: " << pathSHA1 << ", Hitcount: " << path_Hit_count << "\n";
            std::cout << "User-Agent: " << userAgent << ", SHA1: " << userAgentSHA1
                      << ", Hitcount: " << user_Agent_Hit_count << "\n";
        });

        // Закрытие клиентского сокета
        close(client_socket);
    }


    const int m_PORT;
    const int m_BUFFER_SIZE;
    int m_server_socket;
    threadpool::Threadpool m_pool{5};
    std::unordered_map<std::string, int> m_hit_count_Map;

#ifdef TEST_true
    FRIEND_TEST(ServerTest, CalculateSHA1);
    FRIEND_TEST(ServerTest, CountHits);
    FRIEND_TEST(ServerTest, ExtractPath);
    FRIEND_TEST(ServerTest, ExtractUserAgent);
#endif

};

