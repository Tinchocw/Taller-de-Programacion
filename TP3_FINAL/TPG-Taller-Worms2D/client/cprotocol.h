#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <memory>
#include <string>

#include <stdint.h>

#include "../common/const.h"
#include "../common/socket.h"

#include "Action.h"

#define PPM 33.33f  // pixel per meter ratio.
#define GADGETS_QUANTITY 10

class States;
class GameInfoL;
class ProjectileStateG;
class LevelStateG;
class PlayerStateG;
class WormStateG;
class CrateState;

namespace ClientSide {
class Protocol {

private:
    Socket skt;
    /*
        Los separo en dos bool y no en uno atomic ya que el socket necesita un *bool. Podria usar
        load de atomic pero estaria en la misma situacion que en un solo bool normal.
    */
    bool send_was_closed;
    bool recv_was_closed;
    /*
        @param data: Puntero al dato a recibir
        @param sz: Cantidad de bytes a recibir

        @brief Recibe data chequeando si se cierra el socket
    */
    void recv(void* data, unsigned int sz);
    /*
        @param meter_position: Posicion en metros en el eje x

        @brief Convierte la posicion en x de metros a pixeles
    */
    float meter_to_pixel_x(const float& meter_position);
    /*
        @param meter_position: Posicion en metros en el eje y

        @brief Convierte la posicion en y de metros a pixeles
    */
    float meter_to_pixel_y(const float& meter_position);
    /*
        @brief Recibe un GameInfoL state
    */
    std::shared_ptr<GameInfoL> recvGameInfo();
    /*
        @brief Recibe un LevelStateG state
    */
    std::shared_ptr<LevelStateG> recvLevelBuild();
    /*
        @brief Recibe un PlayerStateG state
    */
    std::shared_ptr<PlayerStateG> recvPlayerGame();
    /*
        @brief Recibe un ProjectileStateG state
    */
    std::shared_ptr<ProjectileStateG> recvProjectileGame();
    /*
        @brief Recibe un WormStateG state
    */
    std::shared_ptr<WormStateG> recvWormGame();
    /*
        @brief Recibe un CrateState state
    */
    std::shared_ptr<CrateState> recvCrate();

public:
    /*
        @param data: Dato a ser enviado
        @param sz: Cantidad de bytes a ser enviados

        @brief Envia data chequeando si se cierra el socket
    */
    void send(const void* data, unsigned int sz);
    /*
        @param str: Referencia al string a ser enviado

        @brief Envia un string de 64 bytes maximo
    */
    void sendString64(const std::string& str);
    /*
        @param number: Referencia al float a ser enviado

        @brief Envia un float
    */
    void sendFloat(const float& number);
    /*
        @brief Retorna un uint8_t el cual es recibido a traves del socket
    */
    uint8_t recvUint8();
    /*
        @brief Retorna un float el cual es recibido a traves del socket
    */
    float recvFloat();
    /*
        @brief Retorna un bool el cual es recibido a traves del socket
    */
    bool recvBool();
    /*
        @param str: Referencia al string a asignar

        @brief Recibe un string de 64 bytes maximo
    */
    void recvString64(std::string& str);
    /*
        @param c: Referencia a la accion a asignar

        @brief Recibe un uint8_t y lo castea a Actions
    */
    void recvCommand(Actions& c);
    /*
        @brief Construye el protocolo y su respectivo socket
    */
    explicit Protocol(const char* hostname, const char* servname);
    /*
        @brief Cierra forzosamente el socket del protocolo (en caso de que no se haya hecho)
    */
    void close();
    /*
        @brief Retorna un state que es recibido a traves del socket
    */
    std::shared_ptr<States> recvStates();
    /*
        No tiene queremos ni copiar ni mover el protocolo
    */
    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;

    Protocol(Protocol&&) = delete;
    Protocol& operator=(Protocol&&) = delete;
};
}  // namespace ClientSide

#endif
