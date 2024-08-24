#include <chrono>
#include <condition_variable>
#include <iostream>
#include <list>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

std::mutex mtx;
std::condition_variable cv;
std::list<std::string> msgs;

// 单个写进程（充当生产者）
void WriteThread() {
    for (int i{0};; ++i) {
        std::stringstream ss;
        ss << "Write msg " << i;
        // 上锁并将消息加到队列中
        std::unique_lock<std::mutex> lk{mtx};
        msgs.push_back(ss.str());
        lk.unlock();  // 必须将lk解锁，否则wait结束会再次上锁
        cv.notify_one();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// 多个读进程（充当消费者）
void ReadThread(int i) {
    while (true) {
        std::unique_lock<std::mutex> lk{mtx};
        cv.wait(lk);  // cv 被唤醒后会重新上锁
        // 读数据
        while (!msgs.empty()) {
            std::cout << i << " read" << msgs.front() << std::endl;
            msgs.pop_front();
        }
    }
}

int main() {
    std::thread t1{WriteThread};
    t1.detach();
    for (int i{0}; i < 3; ++i) {
        std::thread t2{ReadThread, i + 1};
        t2.detach();
    }
    while (1) {
    }
    return 0;
}