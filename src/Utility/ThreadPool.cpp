#include "Utility/ThreadPool.hpp"

Worker::Worker(ThreadPool& pool)
    : m_pool(pool)
    , m_thread([this](){loop();})
    , m_stopping(false)
{}

void Worker::loop()
{
    while (true)
    {
        std::function<void()> job;

        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);

            m_condition.wait(lock, [this]() {
                return m_stopping || !m_jobs.empty();
            });

            if (m_stopping && m_jobs.empty())
                return;

            job = std::move(m_jobs.front());
            m_jobs.pop();
        }

        job();
        --m_pool.m_jobs_remaining;

        if (m_pool.m_jobs_remaining == 0)
            m_pool.m_wait_condition.notify_all();
    }
}

void Worker::enqueue(std::function<void()> job)
{
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_jobs.emplace(std::move(job));
    }

    m_condition.notify_one();
}

void Worker::stop()
{
    m_stopping = true;
    m_condition.notify_all(); // should be only one
    m_thread.join();
}

ThreadPool::ThreadPool()
    : ThreadPool(std::thread::hardware_concurrency())
{}

ThreadPool::ThreadPool(int num_threads)
{
    if (num_threads <= 0)
        num_threads = 1;
    m_workers.reserve(num_threads);
    for(int i = 0; i < num_threads; ++i)
    {
        m_workers.emplace_back(std::make_unique<Worker>(*this));
    }
}

ThreadPool::~ThreadPool()
{
    for (auto& worker : m_workers)
        worker->stop();
}

void ThreadPool::wait()
{
    std::unique_lock<std::mutex> lock(m_shared_mutex);
    m_wait_condition.wait(lock, [this]() {
        return m_jobs_remaining == 0;
    });
}

std::size_t ThreadPool::size()
{
    return m_workers.size();
}

void ThreadPool::enqueue(std::function<void()> job)
{
    ++m_jobs_remaining;

    m_workers[m_worker_to_enqueue]->enqueue(job);
    m_worker_to_enqueue = ++m_worker_to_enqueue % m_workers.size();
}