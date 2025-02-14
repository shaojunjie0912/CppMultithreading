#pragma once

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <optional>
#include <queue>

// 线程安全的队列模板类
template <typename T, typename Queue = std::queue<T>>
class ThreadQueue {
public:
    // -1转无符号最大数
    ThreadQueue() : limit_(static_cast<std::size_t>(-1)) {}

    // 指定最大允许堆积的元素数量，超过该数量后会阻塞
    explicit ThreadQueue(std::size_t limit) : limit_(limit) {}

public:
    // 向队列中插入元素
    void Push(T value) {
        std::unique_lock lk{mtx_};
        cv_notfull_.wait(lk, [this] { return queue_.size() <= 5; });
        queue_.push(std::move(value));
        lk.unlock();
        cv_notempty_.notify_one();  // 通知可取
    }

    // 从队列中取出元素(阻塞版本)
    T Pop() {
        std::unique_lock lk{mtx_};
        cv_notempty_.wait(lk, [this] { return !queue_.empty(); });  // 等待可取
        T value{std::move(queue_.front())};
        queue_.pop();
        lk.unlock();
        cv_notfull_.notify_one();
        return value;
    }

    // 尝试从队列中取出元素(不阻塞版本)
    std::optional<T> TryPop() {
        std::unique_lock lk{mtx_};
        if (queue_.empty()) {
            return std::nullopt;
        }
        T value{std::move(queue_.front())};
        queue_.pop();
        lk.unlock();
        cv_notfull_.notify_one();
        return value;
    }

    // 尝试从队列中取出元素，等待一段时间，若超时则返回 nullopt
    std::optional<T> TryPopFor(std::chrono::steady_clock::duration timeout) {
        std::unique_lock lk{mtx_};
        if (!cv_notempty_.wait_for(lk, timeout, [this] { return !queue_.empty(); })) {
            return std::nullopt;
        }
        T value{std::move(queue_.front())};
        queue_.pop();
        lk.unlock();
        cv_notfull_.notify_one();
        return value;
    }

    // 尝试从队列中取出元素，等待至一个时间点，若超时则返回 nullopt
    std::optional<T> TryPopUntil(std::chrono::steady_clock::time_point timeout) {
        std::unique_lock lk{mtx_};
        if (!cv_notempty_.wait_until(lk, timeout, [this] { return !queue_.empty(); })) {
            return std::nullopt;
        }
        T value{std::move(queue_.front())};
        queue_.pop();
        lk.unlock();
        cv_notfull_.notify_one();
        return value;
    }

private:
    Queue queue_;
    std::mutex mtx_;
    std::condition_variable cv_notfull_;   // 队列未满条件变量
    std::condition_variable cv_notempty_;  // 队列非空条件变量
    std::size_t limit_;                    // 最大允许堆积的元素数量
};
