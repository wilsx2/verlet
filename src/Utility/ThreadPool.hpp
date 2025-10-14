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
    std::condition_variable m_take_condition;
    std::condition_variable m_wait_condition;

    bool m_stopping = false;
    int m_busy_jobs = 0;

    void worker_loop();

public:
    ThreadPool();
    ThreadPool(int num_threads);
    ~ThreadPool();

    void enqueue(std::function<void()> job);
    void wait();
};

#endif