#ifndef THREADS_2023C2_TINCHOCW_SERVER_SENDER_H
#define THREADS_2023C2_TINCHOCW_SERVER_SENDER_H

#include "common_queue.h"
#include "common_thread.h"
#include "server_protocol.h"

class Sender: public Thread {
private:
    Protocolo_servidor& protocolo;
    Queue& queue;
    std::atomic<bool> estoy_vivo;

public:
    explicit Sender(Protocolo_servidor& protocolo_, Queue& queue_);
    virtual void run() override;
    void kill();
};
#endif  // THREADS_2023C2_TINCHOCW_SERVER_SENDER_H
