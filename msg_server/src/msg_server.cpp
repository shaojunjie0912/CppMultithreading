#include <msg_server/msg_server.h>

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

void MsgServer::SendMsg(std::string msg) {
    // 由于 list 并非线程安全，因此需要加锁
    std::unique_lock<std::mutex> lk{mtx_};
    msgs_.push_back(std::move(msg));
}

void MsgServer::Main() {
    while (!is_exit()) {
        // 线程等待 10ms 防止队列一直为空则持续上锁
        // 这里存在时延和循环等待（若链表为空），可利用条件变量解决
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::unique_lock<std::mutex> lk{mtx_};
        // 如果消息队列为空，则进入下一次循环
        if (msgs_.empty()) {
            continue;
        }
        while (!msgs_.empty()) {
            std::cout << "recv: " << msgs_.front() << std::endl;
            msgs_.pop_front();
        }
    }
}
