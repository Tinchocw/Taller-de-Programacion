#include "server_receiver.h"

Receiver::Receiver(Protocolo_servidor& protocolo_, Monitor_de_queues& lobby, Queue& queue):
        protocolo(protocolo_), lista_de_chats(lobby), queue(queue), estoy_vivo(true) {}

void Receiver::run() {
    try {
        while (protocolo.estoy_conectado() && estoy_vivo) {
            struct mensaje mensaje_recibido;
            protocolo.recibir_mensaje(mensaje_recibido);
            if (protocolo.estoy_conectado()) {
                lista_de_chats.broadcastear_mensaje(mensaje_recibido);
            } else {
                lista_de_chats.comunicar_salida_de_jugador(&queue);
            }
        }
        estoy_vivo = false;

    } catch (std::runtime_error& error) {
        std::cerr << "Se cerro el socket de forma inesperada" << std::endl;
    }
}

void Receiver::kill() { estoy_vivo = false; }
