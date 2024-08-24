#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using std::cout;
using std::endl;

static const char base16[]{"0123456789abcdef"};

void Base16Encode(const unsigned char* data, int size, unsigned char* out) {
    for (int i{0}; i < size; ++i) {
        unsigned char d = data[i];
        // cout << "d: " << static_cast<int>(d) << endl;
        char high = base16[d >> 4];
        // cout << "high: " << high << endl;
        char low = base16[d & 0x0F];
        // cout << "low: " << low << endl;
        out[i * 2] = high;
        out[i * 2 + 1] = low;
    }
}

void Base16EncodeThread(const std::vector<unsigned char>& data, std::vector<unsigned char>& out) {
    size_t size{data.size()};
    size_t th_count{std::thread::hardware_concurrency()};
    size_t slice_count = size / th_count;
    if (size < th_count) {
        th_count = 1;
        slice_count = size;
    }
    std::vector<std::thread> threads;
    for (size_t i{0}; i < th_count; ++i) {
        size_t offset{i * slice_count};
        size_t count{slice_count};
        if (th_count > 1 && i == th_count - 1) {
            count = slice_count + size % th_count;
        }
        threads.emplace_back([&, offset, count] {
            Base16Encode(data.data() + offset, count, out.data() + offset * 2);
        });
    }
    for (auto& th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }
}

int main() {
    std::vector<unsigned char> in_data(1024 * 1024 * 100);
    for (size_t i{0}; i < in_data.size(); ++i) {
        in_data[i] = i % 256;
    }
    std::vector<unsigned char> out_data(2 * in_data.size());
    // 单线程 base16 编码
    {
        auto start{std::chrono::system_clock::now()};
        Base16Encode(in_data.data(), in_data.size(), out_data.data());
        auto end{std::chrono::system_clock::now()};
        auto duration{std::chrono::duration_cast<std::chrono::milliseconds>(end - start)};
        cout << "单线程编码 10 MB 耗时：" << duration << endl;
    }
    // 多线程 C++11 编码
    {
        std::vector<unsigned char> out_data(2 * in_data.size());
        auto start{std::chrono::system_clock::now()};
        Base16EncodeThread(in_data, out_data);
        auto end{std::chrono::system_clock::now()};
        auto duration{std::chrono::duration_cast<std::chrono::milliseconds>(end - start)};
        cout << "多线程编码 10 MB 耗时：" << duration << endl;
    }
    return 0;
}