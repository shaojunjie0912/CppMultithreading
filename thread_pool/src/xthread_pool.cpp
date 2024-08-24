#include <thread_pool/xthread_pool.h>

#include <cmath>
#include <cstddef>
#include <exception>
#include <iostream>
#include <thread>

using std::cout;
using std::endl;

void XThreadPool::Init(int num) {
    std::unique_lock<std::mutex> lock{mtx_};  // 加锁
    thread_num_ = num;
}

void XThreadPool::Start() {
    std::unique_lock<std::mutex> lock{mtx_};  // 加锁
    // 检测是否初始化线程池
    if (thread_num_ <= 0) {
        std::cerr << "Thread Pool Not Init!" << std::endl;
        return;
    }
    // 检测是否已经启动线程池
    if (!threads_.empty()) {
        std::cerr << "Thread Pool Has Started!" << std::endl;
        return;
    }
    // 启动线程
    for (size_t i{0}; i < thread_num_; ++i) {
        // 使用 XThreadPool::Run 构造一个新线程
        auto th{std::make_shared<std::thread>(&XThreadPool::Run, this)};
        threads_.push_back(th);
    }
}

void XThreadPool::Stop() {
    is_exit_ = true;
    cv_.notify_all();
    for (auto& th : threads_) {
        if (th->joinable()) {
            th->join();  // 等待所有线程执行结束
        }
    }
    std::unique_lock<std::mutex> lock{mtx_};  // 加锁
    threads_.clear();                         // 清除线程数组
}

void XThreadPool::Run() {
    cout << "线程 " << std::this_thread::get_id() << " 启动";
    // 当线程池没有退出
    while (!is_exit()) {
        auto task = GetTask();
        if (!task) {
            continue;
        }
        ++task_run_count_;  // 正在运行的任务数++
        try {
            auto re{task->Run()};  // 执行任务并获取返回值
            task->SetValue(re);    // 设置值
        } catch (...) {
        }
        --task_run_count_;  // 正在运行的任务数--
    }
    cout << "线程 " << std::this_thread::get_id() << " 结束";
}

void XThreadPool::AddTask(std::shared_ptr<XTask> task) {
    std::unique_lock<std::mutex> lock{mtx_};  // 加锁
    tasks_.push_back(task);
    // 每个任务都可获取线程池退出标志
    task->is_exit = [this] { return is_exit(); };  // 用 lambda 返回函数指针
    lock.unlock();                                 // 先解锁
    cv_.notify_one();                              // 再通知
}

std::shared_ptr<XTask> XThreadPool::GetTask() {
    std::unique_lock<std::mutex> lock{mtx_};  // 加锁
    if (tasks_.empty()) {
        cv_.wait(lock);  // 释放lock并阻塞
    }
    // 如果已经线程池已经退出，则返回nullptr
    if (is_exit()) {
        return nullptr;
    }
    // 再次检查是否任务队列为空
    if (tasks_.empty()) {
        return nullptr;  // 是否可以在wait中增加lambda
    }
    auto task{tasks_.front()};
    tasks_.pop_front();
    return task;
}