#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

// 任务纯虚基类
class XTask {
public:
    // 任务入口函数
    virtual int Run() = 0;  // 返回值 int 用于异步获取结果
public:
    // 每个任务都可以获得线程池退出标志
    std::function<bool()> is_exit{nullptr};  // 函数指针

public:
    auto GetReturn() {
        // 阻塞等待 set_value
        return p_.get_future().get();
    }
    void SetValue(int v) { p_.set_value(v); }

private:
    std::promise<int> p_;  // 接收返回值
};

// 线程池类
class XThreadPool {
public:
    // 初始化线程池
    void Init(int num);

    // 启动所有线程
    void Start();

    // 线程池退出
    void Stop();

    // 等待所有线程结束
    void Wait();

    // 添加任务到队列
    void AddTask(std::shared_ptr<XTask> task);

    // 从队列获取任务
    std::shared_ptr<XTask> GetTask();

    bool is_exit() { return is_exit_; }

    int task_run_count() { return task_run_count_; }

private:
    // 线程池线程的入口函数
    void Run();

private:
    int thread_num_{0};                                  // 线程数量
    std::mutex mtx_;                                     // 互斥锁
    std::vector<std::shared_ptr<std::thread>> threads_;  // 线程列表
    std::list<std::shared_ptr<XTask>> tasks_;            // 任务队列
    std::condition_variable cv_;                         // 条件变量
    bool is_exit_{false};                                // 线程池退出标志
    std::atomic<int> task_run_count_{0};                 // 正在运行的任务数量
};