#include <iostream>
#include <mutex>

using std::cout;
using std::endl;

std::mutex mtx;

// 测试 lock_guard，RAII 的🔒
void TestLockGuard(int i) {
    // 1. RAII, 离开{}作用域自动解锁
    {
        // 获得锁的所有权，加锁
        std::lock_guard<std::mutex> lk{mtx};
        cout << "i: " << i << endl;
        // 解锁
    }
    // 2. std::adopt_lock 假设调用方线程已拥有互斥体的所有权
    {
        // 加锁
        mtx.lock();
        // 继承锁的所有权，不加锁
        std::lock_guard<std::mutex> lk{mtx, std::adopt_lock};
        cout << "i: " << i << endl;
        // 解锁
    }
}

// 测试 unique_lock，更灵活的🔒
void TestUniqueLock(int i) {
    // 1. 相比于 lock_guard 它可以解锁/上锁
    {
        // 获得锁的所有权，加锁
        std::unique_lock<std::mutex> lk{mtx};
        // 解锁
        lk.unlock();
        // 加锁
        lk.lock();
        // 解锁
    }
    // 2. std::adopt_lock
    {
        // 加锁
        mtx.lock();
        // 继承锁的所有权，不加锁
        std::unique_lock<std::mutex> lk{mtx, std::adopt_lock};
        // 解锁
    }
    // 3. std::defer_lock 不获得互斥体的所有权
    {
        // 延迟加锁，此时退出{}作用域不解锁
        std::unique_lock<std::mutex> lk{mtx, std::defer_lock};
        // 加锁
        lk.lock();
        // 解锁
    }
    // 4. std::try_to_lock
    {
        // 尝试获得互斥体的所有权而不阻塞
        std::unique_lock<std::mutex> lk{mtx, std::try_to_lock};
    }
}

int main() { return 0; }