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