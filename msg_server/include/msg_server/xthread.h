#pragma once

#include <thread>

class XThread {
public:
    virtual void Start();
    virtual void Wait();
    virtual void Stop();

    bool is_exit();

private:
    virtual void Main() = 0;
    std::thread th_;
    bool is_exit_{false};
};