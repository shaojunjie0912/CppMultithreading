#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>

using std::cout;
using std::endl;

std::string TestAsync(int index) {
    cout << "TestAsync Begin " << index << " " << std::this_thread::get_id() << endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return "TestAsync Return";
}

int main() {
    cout << "Main " << std::this_thread::get_id() << endl;
    auto future{std::async(std::launch::deferred, TestAsync, 42)};
    cout << "Future Get Begin " << endl;
    cout << "future.get() = " << future.get() << endl;
    cout << "Future Get End " << endl;
    return 0;
}