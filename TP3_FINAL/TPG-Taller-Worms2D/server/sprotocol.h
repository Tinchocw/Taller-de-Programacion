#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <memory>
#include <string>

#include <stdint.h>

#include "../common/const.h"
#include "../common/socket.h"
#include "box2d/b2_math.h"

#define PPM 33.33f  // pixel per meter ratio.

class States;

namespace ServerSide {
class Protocol {

private:
    Socket peer;
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
        @param pos: Referencia a la posicion a ser enviada

        @brief Envia una posicion a traves del socket
    */
    void sendPosition(const b2Vec2& pos);
    /*
        @param ps: State de tipo PlayerStateG
    */
    void sendPlayerState(const std::shared_ptr<States>& ps);
    /*
        @param state: State de tipo WormState

        @brief Envia el game state completo de WormState
    */
    void sendWormState(const std::shared_ptr<States>& state);
    /*
        @param count: State de tipo GameInfo

        @brief Envia el game state completo de GameInfo
    */
    void sendGameInfo(const std::shared_ptr<States>& count);
    /*
        @param count: State de tipo CountState o sus subclases

        @brief Envia el game state completo de CountState
    */
    void sendCount(const std::shared_ptr<States>& count);
    /*
        @param count: State de tipo PlayerLobby

        @brief Envia el game state completo de PlayerLobby
    */
    void sendPlayerLobby(const std::shared_ptr<States>& count);
    /*
        @param lb: State de tipo Level

        @brief Envia el game state completo de Level
    */
    void sendLevelBuild(const std::shared_ptr<States>& lb);
    /*
        @param ps: State de tipo Projectile

        @brief Envia el game state completo de Projectile
    */
    void sendProjectileState(const std::shared_ptr<States>& ps);
    /*
        @param state: State de tipo Turn

        @brief Envia el game state completo de Turn
    */
    void sendTurn(const std::shared_ptr<States>& state);
    /*
        @param state: State de tipo BattlefieldG

        @brief Envia el game state completo de BattlefieldG
    */
    void sendBattlefield(const std::shared_ptr<States>& state);
    /*
        @param pixel_position: Posicion en x en pixeles

        @brief Convierte la posicion en x en pixeles a metros
    */
    float pixel_to_meter_x(const float& pixel_position);
    /*
        @param pixel_position: Posicion en y en pixeles

        @brief Convierte la posicion en y en pixeles a metros
    */
    float pixel_to_meter_y(const float& pixel_position);
    /*
        @param state: Game state de una caja

        @brief Envia el game state completo de una caja de provisiones
    */
    void sendCrate(const std::shared_ptr<States>& state);
    /*
        @param state: State de tipo YouWin

        @brief Envia el game state completo de YouWin
    */
    void sendYouWin(const std::shared_ptr<States>& state);

public:
    /*
        @param data: Dato a ser enviado
        @param sz: Cantidad de bytes a ser enviados

        @brief Envia data chequeando si se cierra el socket
    */
    void send(const void* data, unsigned int sz);
    /*
        @brief Retorna un uint8_t el cual es recibido a traves del socket
    */
    uint8_t recvUint8();
    /*
        @param state: Game/Lobby State a ser enviado

        @brief Envia un state a traves del socket dependiendo de su tag
    */
    void sendStates(const std::shared_ptr<States>& state);
    /*
        Construye el protocolo y su respectivo socket
    */
    explicit Protocol(Socket&& peer);
    /*
        Recibe el comando que el cliente desea ejecutar
    */
    void recvCommand(Actions&);
    /*
        Recibe la direccion hacia la cual se debe mover el gusano
    */
    void recvDirection(Direction&);
    /*
        Recibe la direccion hacia la cual debe saltar el gusano
    */
    void recvJumpDir(JumpDir& dir);
    /*
        @param number: Referencia al float a ser enviado

        @brief Envia un float
    */
    void sendFloat(float number);
    /*
        @param str: Referencia al string a ser enviado

        @brief Envia un string de 64 bytes maximo
    */
    void sendString64(const std::string& str);
    /*
        Recibe la direccion hacia la cual debe inclinar la mira el gusano
    */
    void recvADSAngleDir(ADSAngleDir& dir);
    /*
        Recibe la cantidad de retraso que tiene un proyectil apto
    */
    void recvDelay(DelayAmount& amount);
    /*
        Recibe el arma o herramienta que el gusano se debe equipar
    */
    void recvGadget(WeaponsAndTools& gadget);
    /*
        Cierra forzosamente el socket del protocolo (en caso de que no se haya hecho)
    */
    void close();
    /*
        Recibe un string de maximo 64 caracteres
    */
    void recvString64(std::string& desc);
    /*
        @brief Retorna un float el cual es recibido a traves del socket
    */
    float recvFloat();
    /*
        @param position: Referencia a la posicion a ser recibida

        @brief Recibe una posicion a traves del socket
    */
    void recvPosition(b2Vec2& position);
    /*
        Recibe la id del game al que se quiere conectar el cliente
    */
    void recvGameID(int16_t&);
    /*
        No tiene sentido ni copiar ni mover el protocolo
    */
    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;

    Protocol(Protocol&&) = delete;
    Protocol& operator=(Protocol&&) = delete;
};
}  // namespace ServerSide

#endif
