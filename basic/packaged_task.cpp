#include <future>
#include <iostream>
#include <thread>

template <typename R, typename... Ts, typename... Args>
void AsyncTask(std::packaged_task<R(Ts...)>& task, Args&&... args) {
    task(std::forward<Args>(args)...);
}

int main() {
    // std::packaged_task 包装可调用目标(函数、lambda 表达式、bind 表示或其它函数对象)
    // task 具有 () 重载，但是无返回值
    // task 只能移动不能复制
    std::packaged_task<double(int, int)> task([](int a, int b) { return std::pow(a, b); });
    // 在调用 () 之前使其与 future 关联
    std::future<double> future = task.get_future();
    task(10, 2);
    std::cout << "future.get(): " << future.get() << std::endl;  // 不阻塞，此处获取返回值
}