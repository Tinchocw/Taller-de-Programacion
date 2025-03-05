#ifndef THREADS_2023C2_TINCHOCW_SERVER_RECEIVER_H
#define THREADS_2023C2_TINCHOCW_SERVER_RECEIVER_H

#include <atomic>

#include "common_liberror.h"
#include "common_queue.h"
#include "common_thread.h"
#include "server_monitor_de_queues.h"
#include "server_protocol.h"

class Receiver: public Thread {
private:
    Protocolo_servidor& protocolo;
    Monitor_de_queues& lista_de_chats;
    Queue& queue;
    std::atomic<bool> estoy_vivo;

public:
    explicit Receiver(Protocolo_servidor& protocolo_, Monitor_de_queues& lobby, Queue& queue);
    virtual void run() override;
    void kill();
};

#endif  // THREADS_2023C2_TINCHOCW_SERVER_RECEIVER_H
