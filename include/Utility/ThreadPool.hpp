#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <functional>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>

class ThreadPool;

class Worker
{
    private:
    ThreadPool& m_pool;
    std::thread m_thread;
    std::mutex m_queue_mutex;
    std::condition_variable m_condition;
    std::queue<std::function<void()>> m_jobs;
    std::atomic<bool> m_stopping;

    public:
    Worker(ThreadPool& pool);
    void loop();
    void enqueue(std::function<void()> job);
    void stop();
};

class ThreadPool
{
    private:
    std::vector<std::unique_ptr<Worker>> m_workers;
    std::mutex m_shared_mutex;
    std::condition_variable m_wait_condition;
    std::atomic<int> m_jobs_remaining = 0;
    int m_worker_to_enqueue;

    template <class V, class F>
    void enqueue_for_each_generic(V& data, F&& procedure);

    public:
    ThreadPool();
    ThreadPool(int num_threads);
    ~ThreadPool();
    void wait();
    std::size_t size();
    void enqueue(std::function<void()> job);
    template <class T>
    void enqueue_for_each(std::vector<T>& data, std::function<void(T&, std::size_t)>&& procedure);
    template <class T>
    void enqueue_for_each(const std::vector<T>& data, std::function<void(const T&, std::size_t)>&& procedure);

    friend class Worker;
};

template <class V, class F>
void ThreadPool::enqueue_for_each_generic(V& data, F&& procedure){
    int slice_size = ((data.size() + m_workers.size() - 1) / m_workers.size());
    if(slice_size <= 0)
        return;

    for (int i = 0; i < data.size(); i += slice_size)
    {
        int start = i;
        int end = std::min(i + slice_size, static_cast<int>(data.size()));
        enqueue([&, start, end]() mutable {
            for(int j = start; j < end; ++j)
            {
                procedure(data[j], j);
            }
        });
    }
}

template <class T>
void ThreadPool::enqueue_for_each(std::vector<T>& data, std::function<void(T&, std::size_t)>&& procedure){
    enqueue_for_each_generic(data, procedure);
}

template <class T>
void ThreadPool::enqueue_for_each(const std::vector<T>& data, std::function<void(const T&, std::size_t)>&& procedure){
    enqueue_for_each_generic(data, procedure);
}

#endif