#include <chrono>
#include <iostream>
#include <shared_mutex>
#include <thread>

using std::cout;
using std::endl;

// 共享锁可用于存在多个读、写进程的业务中
// 功能1：共享（方便读）
// 功能2：互斥（方便写）
std::shared_timed_mutex stmtx;  // 超时共享锁 C++14
std::shared_mutex smtx;         // 共享锁 C++17

// 读线程（只包含读数据）
void ReadThread(int i) {
    while (true) {
        smtx.lock_shared();  // 共享上锁
        // 模拟读业务逻辑
        cout << "Read i: " << i << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        smtx.unlock_shared();                                        // 共享解锁
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // 防止解锁后立即上锁
    }
}

// 写线程（同时包含读数据和写数据）
void WriteThread(int i) {
    while (true) {
        smtx.lock_shared();
        // -- 读数据 --
        smtx.unlock_shared();
        smtx.lock();  // 互斥上锁
        // 模拟写业务逻辑
        cout << "Write i: " << i << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        smtx.unlock();                                               // 互斥解锁
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // 防止解锁后立即上锁
    }
}

int main() {
    // 开启三个读线程
    for (int i{0}; i < 3; ++i) {
        std::thread t{ReadThread, i + 1};
        t.detach();
    }
    // 开启三个写线程
    for (int i{0}; i < 3; ++i) {
        std::thread t{WriteThread, i + 1};
        t.detach();
    }
    // ------- 没有出现同时对i进行读写的情况
    while (1) {
    }
    return 0;
}