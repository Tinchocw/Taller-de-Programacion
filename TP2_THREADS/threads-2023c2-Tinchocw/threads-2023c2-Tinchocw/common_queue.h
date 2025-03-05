#ifndef THREADS_2023C2_TINCHOCW_COMMON_QUEUE_H
#define THREADS_2023C2_TINCHOCW_COMMON_QUEUE_H

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

#include "common_constantes_protocolo.h"

struct ClosedQueue: public std::runtime_error {
    ClosedQueue(): std::runtime_error("The chats is closed") {}
};

class Queue {
private:
    std::queue<struct mensaje> q;
    const unsigned int max_size;

    bool closed;

    std::mutex mtx;
    std::condition_variable is_not_full;
    std::condition_variable is_not_empty;

public:
    explicit Queue(const unsigned int max_size);


    bool try_push(const struct mensaje& val);
    bool try_pop(struct mensaje& val);
    void push(const struct mensaje& val);
    struct mensaje pop();

    void close();

private:
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;
};
#endif  // THREADS_2023C2_TINCHOCW_COMMON_QUEUE_H
