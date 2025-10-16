#include "ThreadPool.hpp"

ThreadPool::ThreadPool()
    : ThreadPool(std::thread::hardware_concurrency())
{}

ThreadPool::ThreadPool(int num_threads)
{
    if (num_threads <= 0)
        num_threads = 1;

    m_workers.reserve(num_threads);
    for (int i = 0; i < num_threads; ++i)
        m_workers.emplace_back([this]() { worker_loop(); });
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_stopping = true;
    }

    m_take_condition.notify_all();

    for (auto& worker : m_workers)
        worker.join();
}

void ThreadPool::worker_loop()
{
    while (true)
    {
        std::function<void()> job;

        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);

            m_take_condition.wait(lock, [this]() {
                return m_stopping || !m_jobs.empty();
            });

            if (m_stopping && m_jobs.empty())
                return;

            job = std::move(m_jobs.front());
            m_jobs.pop();
            ++m_busy_jobs;
        }

        job();

        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            --m_busy_jobs;

            if (m_jobs.empty() && m_busy_jobs == 0)
                m_wait_condition.notify_all();
        }
    }
}

void ThreadPool::wait()
{
    std::unique_lock<std::mutex> lock(m_queue_mutex);
    m_wait_condition.wait(lock, [this]() {
        return m_jobs.empty() && m_busy_jobs == 0;
    });
}

std::size_t ThreadPool::size()
{
    return m_workers.size();
}

void ThreadPool::enqueue(std::function<void()> job)
{
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_jobs.emplace(std::move(job));
    }

    m_take_condition.notify_one();
}