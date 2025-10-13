#include "ThreadPool.hpp"

void ThreadPool::take_jobs()
{
}

ThreadPool::ThreadPool()
    : ThreadPool(std::thread::hardware_concurrency())
{}

ThreadPool::ThreadPool(int num_threads)
{
    m_deconstruting = false;
    for(int i = 0; i < num_threads; ++i)
        m_workers.emplace_back(take_jobs);
}

void ThreadPool::take_jobs()
{
    while (true) {
        std::function<void()> job;

        {
            std::unique_lock<std::mutex> lock (m_queue_mutex);
            m_condition.wait(lock, [this](){ return m_deconstruting || !m_jobs.empty();});
            if (m_deconstruting && m_jobs.empty())
                return;
            job = std::move(m_jobs.front());
            m_jobs.pop();
        }

        job();
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_deconstruting = true;
    }
    m_condition.notify_all();
    for (auto& worker : m_workers)
        worker.join();
}

void ThreadPool::enqueue(std::function<void()> job)
{
    {
        std::unique_lock<std::mutex> lock (m_queue_mutex);
        m_jobs.emplace(job);
    }
    m_condition.notify_one();
}
