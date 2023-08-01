//
// Created by Pooch on 7/31/23.
//

#ifndef THREADPOOL_THREAD_POOL_H
#define THREADPOOL_THREAD_POOL_H
#include <deque>
#include <mutex>
#include <thread>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(unsigned int num_threads = std::thread::hardware_concurrency());
    ~ThreadPool();

    void start();
    void stop();
    void queue_task(const std::function<void()>& task);
    bool busy();

private:
    void thread_loop();

    bool terminate = false;
    unsigned int num_threads_;
    std::vector<std::thread> pool_;
    std::deque<std::function<void()>> tasks_;
    std::mutex m_;
    std::condition_variable tasks_to_run_;
};

#endif // THREADPOOL_THREAD_POOL_H
