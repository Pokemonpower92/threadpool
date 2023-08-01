//
// Created by Pooch on 7/31/23.
//

#include "threadpool.h"
#include <iostream>
#include <thread>

ThreadPool::ThreadPool(unsigned int num_threads)
{
    this->num_threads_ = num_threads;
}

ThreadPool::~ThreadPool()
{
    this->stop();
}

void ThreadPool::start()
{
    for (auto _ = 0; _ < this->num_threads_; _++) {
        this->pool_.emplace_back(&ThreadPool::thread_loop, this);
    }
    this->terminate = false;
}

void ThreadPool::stop()
{
    {
        std::unique_lock<std::mutex> guard { this->m_ };
        this->terminate = true;
    }

    this->tasks_to_run_.notify_all();
    for (auto& thread : this->pool_) {
        thread.join();
    }
    this->pool_.clear();
}

void ThreadPool::queue_task(const std::function<void()>& task)
{
    {
        std::unique_lock<std::mutex> guard { this->m_ };
        this->tasks_.emplace_back(task);
    }
    this->tasks_to_run_.notify_one();
}

bool ThreadPool::busy()
{
    std::unique_lock<std::mutex> guard { this->m_ };
    return !this->tasks_.empty();
}

void ThreadPool::thread_loop()
{
    for (;;) {
        std::unique_lock<std::mutex> guard(this->m_);
        this->tasks_to_run_.wait(guard, [this] {
            return !this->tasks_.empty() || this->terminate;
        });
        if (this->terminate) {
            return;
        }
        auto job = this->tasks_.front();
        this->tasks_.pop_front();
        job();
    }
}
