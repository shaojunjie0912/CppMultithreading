#include <msg_server_condition/msg_server_condition.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

int main(int argc, char* argv[]) {
    MsgServer server;
    server.Start();
    for (int i{0}; i < 3; ++i) {
        std::stringstream ss;
        ss << " msg :" << i + 1;
        server.SendMsg(ss.str());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    server.Stop();
    std::cout << "你好" << std::endl;
    return 0;
}