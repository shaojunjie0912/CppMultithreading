#include <chrono>
#include <iostream>
#include <thread>

using std::cout;
using std::endl;

class Foo {
public:
    Foo() { cout << "构造" << endl; };
    Foo(const Foo&) { cout << "拷贝构造" << endl; }
    // Foo(Foo&& other) = delete;
    ~Foo() { cout << "析构" << endl; }

public:
    int a{42};
};

void Func(Foo* pf) {
    cout << "pf->a: " << pf->a << endl;
    // std::cout << "Foo in TestFoo()" << std::endl;
}

// 抽象类（至少包含一个纯虚函数）
// 封装线程基类接口
class XThread {
public:
    // 启动线程
    virtual void Start() {
        th_ = std::thread{&XThread::Main, this};  // 貌似不必须加当前对象地址this
    }
    // 等待线程退出
    virtual void Wait() {
        if (th_.joinable()) {
            th_.join();
        }
    }
    // 手动停止线程
    virtual void Stop() {
        is_exit_ = true;
        Wait();
    }

public:
    // 开放is_exit查询
    bool is_exit() { return is_exit_; }

private:
    // 派生类必须实现该方法
    virtual void Main() = 0;  // 纯虚函数，不需要实现
    std::thread th_;          // 私有成员变量
    bool is_exit_{false};     // 退出标志
};

class TestXThread : public XThread {
    void Main() override {
        cout << "TestXThread Begin" << endl;
        while (!is_exit()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            cout << "." << std::flush;
        }
        cout << "TestXThread End" << endl;
    }
};

int main() {
    TestXThread test_th;
    test_th.Start();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    test_th.Stop();
    return 0;
}