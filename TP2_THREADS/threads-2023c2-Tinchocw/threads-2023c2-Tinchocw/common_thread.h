#ifndef THREADS_2023C2_TINCHOCW_COMMON_THREAD_H
#define THREADS_2023C2_TINCHOCW_COMMON_THREAD_H

#include <iostream>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>


class Thread {
private:
    std::thread thread;

public:
    Thread();

    void start();
    void join();

    virtual void run() = 0;
    virtual ~Thread();

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    Thread(Thread&& other);
    Thread& operator=(Thread&& other);
};


#endif  // THREADS_2023C2_TINCHOCW_COMMON_THREAD_H
