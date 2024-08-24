#include <thread_pool/xthread_pool.h>

#include <chrono>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>

using std::cout;
using std::endl;

class MyTask : public XTask {
public:
    int Run() {
        cout << "================================================" << endl;
        cout << std::this_thread::get_id() << " Mytask " << name << endl;
        cout << "================================================" << endl;
        for (int i = 0; i < 10; i++) {
            if (is_exit())
                break;
            cout << "." << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        return 0;
    }

public:
    std::string name = "";
};

int main() {
    XThreadPool pool;
    pool.Init(8);
    pool.Start();

    auto task3 = std::make_shared<MyTask>();
    task3->name = "test shared 003";
    pool.AddTask(task3);

    auto task4 = std::make_shared<MyTask>();
    task4->name = "test shared 004";
    pool.AddTask(task4);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    cout << "task run  count = " << pool.task_run_count() << endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    pool.Stop();
    cout << "task run  count = " << pool.task_run_count() << endl;

    getchar();
    return 0;
}