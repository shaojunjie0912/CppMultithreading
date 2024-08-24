#include <msg_server_condition/xthread.h>

void XThread::Start() { th_ = std::thread{&XThread::Main, this}; }

void XThread::Stop() {
    is_exit_ = true;
    Wait();
}

void XThread::Wait() {
    if (th_.joinable()) {
        th_.join();
    }
}

bool XThread::is_exit() { return is_exit_; }