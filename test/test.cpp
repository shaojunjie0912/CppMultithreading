#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using namespace std;

deque<string> str_deq;
int a{0};
mutex mtx;
condition_variable cv;

void t1() {
    cv.notify_one();
    stringstream ss;
    unique_lock lk{mtx};
    ++a;
    ss << "t1 你说得对 " << a;
    str_deq.push_front(ss.str());
    lk.unlock();
    cv.notify_one();
}

void t2() {
    // this_thread::sleep_for(1s);
    unique_lock lk{mtx};
    cv.wait(lk, [] { return !str_deq.empty(); });
    cout << "t2 收到消息 " << str_deq.back() << endl;
    str_deq.pop_back();
}

int main() {
    vector<jthread> pool;
    pool.push_back(jthread(t1));
    pool.push_back(jthread(t2));
    pool.clear();
    return 0;
}