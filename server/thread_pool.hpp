#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
public:
    explicit ThreadPool(size_t threads);
    ~ThreadPool();
    
    template<class F>
    void enqueue(F&& task);
    
    void wait_completion();
    
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop{false};
    std::atomic<int> active_tasks{0};
    std::condition_variable completion_cv;
};

template<class F>
void ThreadPool::enqueue(F&& task) {
    {
        std::unique_lock lock(queue_mutex);
        tasks.emplace(std::forward<F>(task));
    }
    condition.notify_one();
}