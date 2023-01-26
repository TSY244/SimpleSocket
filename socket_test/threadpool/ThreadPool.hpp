#pragma once
#ifndef _THREADPOOL_HPP
#define _THREADPOOL_HPP

#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <cstdint>
#include <vector>

class ThreadPool
{
private:
    using Self = ThreadPool;  //取别名

    std::mutex lock_;  //锁
    std::condition_variable cond_;  //条件变量
    std::vector<std::function<void()>> tasks_;  //任务队列
    std::vector<std::thread> threads_;  //线程
    bool running_;  //是否运行

    void ThreadEntry() noexcept; //异常抛出
public:

    explicit ThreadPool(std::size_t threads);

   /* ThreadPool(const Self &other) = delete;

    ThreadPool(Self &&other) noexcept = delete;

    Self &operator=(const Self &other) = delete;

    Self &operator=(Self &&other) noexcept = delete;
*/
    ~ThreadPool() noexcept
    {
        this->Stop();
    }

    template<typename _Fn,typename ..._Args,
            typename _Check = decltype(std::declval<std::function<void()>&>() = std::bind(std::declval<_Fn>(),
                    std::declval<_Args>()...))>
    inline void SubmitTask(_Fn &&fn,_Args &&...args)
    {
        std::unique_lock<std::mutex> lock(this->lock_);
        this->tasks_.emplace_back(std::bind(std::forward<_Fn>(fn),std::forward<_Args>(args)...));
        this->cond_.notify_one();
    }

    void Stop() noexcept;
};

#endif