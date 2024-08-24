#pragma once
#include <msg_server_condition/xthread.h>

#include <list>
#include <mutex>
#include <string>



class MsgServer : public XThread {
public:
    // 给当前线程发送消息
    // 主线程
    void SendMsg(std::string msg);
    void Stop() override;

private:
    // 处理消息的线程入口函数
    // 子线程
    void Main() override;
    // 消息队列缓冲
    std::list<std::string> msgs_;
    // 互斥访问消息队列
    std::mutex mtx_;
    // 条件变量
    std::condition_variable cv_;
};