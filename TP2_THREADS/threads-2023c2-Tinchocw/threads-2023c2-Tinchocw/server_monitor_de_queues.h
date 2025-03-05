#ifndef THREADS_2023C2_TINCHOCW_SERVER_MONITOR_DE_QUEUES_H
#define THREADS_2023C2_TINCHOCW_SERVER_MONITOR_DE_QUEUES_H

#include <list>

#include "common_printer.h"
#include "common_queue.h"

class Monitor_de_queues {
private:
    std::mutex m;
    std::list<Queue*> jugadores;

    void broadcastear_cantidad_jugadores();
    void agregar_queue(Queue* jugador);
    void eliminar_jugador(Queue* jugador);
    int size();

public:
    void broadcastear_mensaje(struct mensaje msj);
    void comunicar_entrada_de_nuevo_jugador(Queue* jugador);
    void comunicar_salida_de_jugador(Queue* jugador);
};


#endif  // THREADS_2023C2_TINCHOCW_SERVER_MONITOR_DE_QUEUES_H
