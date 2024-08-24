#include <msg_server_condition/msg_server_condition.h>

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

// 条件变量(如果 wait 带 lambda)
// notify_one 唤醒等待线程
// 随后检查 wait 的条件
// 若条件为真，则运行
// 若条件为假，则继续阻塞

// 如果不带 lambda 则被唤醒就运行

// 重载基类 Stop 函数
// 通知条件变量
void MsgServer::Stop() {
    is_exit_ = true;
    cv_.notify_all();  // !! 重要
    Wait();
}

void MsgServer::SendMsg(std::string msg) {
    // 由于 list 并非线程安全，因此需要加锁
    std::unique_lock<std::mutex> lk{mtx_};
    msgs_.push_back(std::move(msg));
    lk.unlock();       // 记得先解锁
    cv_.notify_one();  // 唤醒一个等待条件变量的线程
}

void MsgServer::Main() {
    while (!is_exit()) {
        std::unique_lock<std::mutex> lk{mtx_};
        cv_.wait(lk, [this] {
            if (is_exit_) {  // 如果不判断则会一直阻塞
                return true;
            }
            return !msgs_.empty();
        });
        while (!msgs_.empty()) {
            std::cout << "recv: " << msgs_.front() << std::endl;
            msgs_.pop_front();
        }
    }
}
