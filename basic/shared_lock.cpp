#include <mutex>
#include <shared_mutex>


// 类 shared_lock 是一种通用共享互斥体所有权包装器，允许延迟锁定、定时锁定和锁所有权的转移。
//  解决 shared_mutex 需要手动加解锁的问题

std::shared_mutex smtx;

void TestSharedLock() {
    // 1. 读数据
    {
        // shared_lock 默认调用共享锁上锁
        std::shared_lock<std::shared_mutex> lk{smtx};
        // 解锁
    }
    // 2. 写数据
    {
        // 借助 unique_lock 调用互斥锁
        std::unique_lock<std::shared_mutex> lk{smtx};
    }
}