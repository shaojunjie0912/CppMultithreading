#include <thread_queue/thread_queue.h>

#include <chrono>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <variant>
#include <vector>

using std::cout;

ThreadQueue<std::optional<int>> counter_queue;  // 存储所有要计算的数的队列
ThreadQueue<std::monostate> finish_queue;       // 标志结束的队列

// 计数线程
void CounterThread() {
    int count{0};
    while (true) {
        if (auto i{counter_queue.Pop()}) {
            count += i.value();
        } else {
            cout << "CounterThread 收到结束消息\n";
            break;
        }
    }
    cout << "最终 counter: " + std::to_string(count) + '\n';
}

// 将beg-end范围内数据推入队列
void Compute(int beg, int end) {
    for (int i{beg}; i < end; ++i) {
        counter_queue.Push(i);
    }
    // 单个cout<<线程安全，多个不一定
    // 解决方法1：使用字符串拼接
    // 解决方法2：使用字符串流 stringstream 一次性输出到cout
    // cout << "Compute " + std::to_string(beg) + " ~ " + std::to_string(end) +
    //             " 计算结束，等待大部队会师\n";
    std::ostringstream oss;
    oss << "Compute: " << beg << " ~ " << end << " 计算结束，等待大部队会师\n";
    cout << oss.str();
    finish_queue.Push(std::monostate{});
}

void FinishThread() {
    cout << "FinishThread 等待会师\n";
    for (int i{0}; i < 10; ++i) {
        finish_queue.Pop();
    }
    cout << "FinishThread 会师成功，正在发送结束消息\n";
    counter_queue.Push(std::nullopt);
}

int main() {
    std::vector<std::jthread> pool;
    for (int i{0}; i < 10000; i += 1000) {
        pool.push_back(std::jthread{Compute, i, i + 1000});  // 10个推入线程
    }
    pool.push_back(std::jthread{CounterThread});  // 一个计数线程
    pool.push_back(std::jthread{FinishThread});   // 一个结束标志线程
    pool.clear();
    return 0;
}