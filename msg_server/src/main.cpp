#include <msg_server/msg_server.h>

#include <chrono>
#include <sstream>
#include <thread>

int main(int argc, char* argv[]) {
    MsgServer server;
    server.Start();
    for (int i{0}; i < 10; ++i) {
        std::stringstream ss;
        ss << " msg :" << i + 1;
        server.SendMsg(ss.str());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    server.Stop();
    return 0;
}