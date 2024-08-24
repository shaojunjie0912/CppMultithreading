#pragma once

#include <thread>

class XThread {
public:
    virtual void Start();
    virtual void Wait();
    virtual void Stop();

    bool is_exit();

protected:
    bool is_exit_{false};

private:
    virtual void Main() = 0;
    std::thread th_;
};