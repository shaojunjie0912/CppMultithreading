#include <future>
#include <string>
#include <thread>

void TestFuture(std::promise<std::string> p){
    p.set_value("TestFuture Value");
}

int main(int argc, char* argv[]) {
    std::promise<std::string> p;
    auto future{p.get_future()};// get_future 阻塞接收

    return 0;
}