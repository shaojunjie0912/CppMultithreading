#include <future>
#include <iostream>
#include <thread>

int main() {
    // std::packaged_task 包装可调用目标(函数、lambda 表达式、bind 表示或其它函数对象)
    std::packaged_task<double(int, int)> task([](int a, int b) { return std::pow(a, b); });
}