// Adapted from https://github.com/progschj/ThreadPool/blob/master/ThreadPool.h

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <functional>
#include <condition_variable>
#include <thread>
#include <mutex>

class ThreadPool
{
    private:
    std::mutex m_queue_mutex;
    std::queue<std::function<void()>> m_jobs;
    std::vector<std::thread> m_workers;
    std::condition_variable m_condition;
    bool m_deconstruting;
    void take_jobs();

    public:
    ThreadPool();
    ThreadPool(int num_threads);
    ~ThreadPool();
    void enqueue(std::function<void()> job);
};

#endif