#include "thread_pool.hpp"

ThreadPool::ThreadPool(size_t threads) {
    for(size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this] {
            while(true) {
                std::function<void()> task;
                {
                    std::unique_lock lock(queue_mutex);
                    condition.wait(lock, [this]{ 
                        return stop || !tasks.empty(); 
                    });
                    
                    if(stop && tasks.empty()) return;
                    
                    task = std::move(tasks.front());
                    tasks.pop();
                    active_tasks++;
                }
                
                task();
                active_tasks--;
                completion_cv.notify_one();
            }
        });
    }
}

void ThreadPool::wait_completion() {
    std::unique_lock lock(queue_mutex);
    completion_cv.wait(lock, [this]() { 
        return tasks.empty() && active_tasks == 0; 
    });
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(auto& worker : workers) {
        if(worker.joinable()) worker.join();
    }
}