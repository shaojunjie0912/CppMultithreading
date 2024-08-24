#include <ffmpeg_video/xthread_pool.h>

#include <chrono>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>

using std::cout;
using std::endl;

int main() {
    XThreadPool pool;
    pool.Init(16);
    pool.Start();
    return 0;
}