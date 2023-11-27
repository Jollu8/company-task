//
// Created by Jollu Emil on 11/27/23.
//

#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
//#include <openssl/sha.h>
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

