#include "client_protocolo_cliente.h"


Protocolo_cliente::Protocolo_cliente(const std::string& hostname, const std::string& servname):
        hostname(hostname), skt(hostname.c_str(), servname.c_str()) {

    es_select = false;
}

void Protocolo_cliente::procesar_select(std::string argumento) {
    bool was_closed = false;
    es_select = true;
    uint16_t len_instruccion = 0;
    uint16_t len_instruccion_net = 0;
    char codigo_instruccion = SELECT_;

    std::vector<char> intruccion_net(argumento.begin(), argumento.end());

    len_instruccion = argumento.size();
    len_instruccion_net = htons(len_instruccion);

    unsigned char buf[2];
    memcpy(buf, &len_instruccion_net, sizeof(buf));

    skt.sendall(&codigo_instruccion, 1, &was_closed);
    skt.sendall(buf, sizeof(buf), &was_closed);
    skt.sendall(intruccion_net.data(), len_instruccion, &was_closed);
}
void Protocolo_cliente::procesar_dir(std::string argumento) {
    bool was_closed = false;
    es_select = false;
    char codigo_instruccion = DIR_;

    char instruccion_net = argumento.at(0);
    instruccion_net = (instruccion_net - 48);

    skt.sendall(&codigo_instruccion, 1, &was_closed);
    skt.sendall(&instruccion_net, 1, &was_closed);
}
void Protocolo_cliente::procesar_mov() {
    bool was_closed = false;
    es_select = false;
    char codigo_instruccion = MOV_;

    skt.sendall(&codigo_instruccion, 1, &was_closed);
}
void Protocolo_cliente::procesar_jump(std::string argumento) {
    bool was_closed = false;
    es_select = false;
    char codigo_instruccion = JUMP_;
    char instruccion_net = argumento.at(0);
    instruccion_net = (instruccion_net - 48);

    skt.sendall(&codigo_instruccion, 1, &was_closed);
    skt.sendall(&instruccion_net, 1, &was_closed);
}
std::vector<std::string> Protocolo_cliente::instruccion_select_recibida() {
    bool estaba_cerrado = false;
    std::vector<std::string> mensaje_recibido;
    std::vector<std::string> mensaje_recibido_provisional;

    char verificacion;
    std::string verificacion_;

    skt.recvall(&verificacion, 1, &estaba_cerrado);
    recibir_coordenada(mensaje_recibido_provisional);

    if (verificacion == EXITO) {
        mensaje_recibido.insert(mensaje_recibido.end(), mensaje_recibido_provisional.begin(),
                                mensaje_recibido_provisional.end());
    } else if (verificacion == ERROR) {
        verificacion_ = FALLO;
        mensaje_recibido.push_back(verificacion_);
    }

    return mensaje_recibido;
}
void Protocolo_cliente::recibir_coordenada(std::vector<std::string>& coordenada) {
    bool was_closed = false;
    unsigned char coordenada_x_[4];
    unsigned char coordenada_y_[4];

    skt.recvall(coordenada_x_, 4, &was_closed);
    skt.recvall(coordenada_y_, 4, &was_closed);

    uint32_t coordenada_x = 1;
    uint32_t coordenada_y = 1;

    memcpy(&coordenada_x, coordenada_x_, 4);
    memcpy(&coordenada_y, coordenada_y_, 4);

    coordenada.push_back(std::to_string(ntohl(coordenada_x)));
    coordenada.push_back(std::to_string(ntohl(coordenada_y)));
}

std::vector<std::string> Protocolo_cliente::instruccion_recibida() {
    std::vector<std::string> mensaje_recibido;
    if (es_select) {
        std::vector<std::string> mensaje_select = instruccion_select_recibida();
        mensaje_recibido.insert(mensaje_recibido.end(), mensaje_select.begin(),
                                mensaje_select.end());
    } else {
        recibir_coordenada(mensaje_recibido);
    }
    return mensaje_recibido;
}
