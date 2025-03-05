#include <iostream>
#include <arpa/inet.h>
#include <vector>
#include <cstring>
#include <string.h>
#include <sstream>

int main() {
/*
    std::vector<char> mensaje_decodificado;


    uint16_t size_mapa  = 60000;
    auto sz = sizeof(uint16_t);
    std::cout << sz << std::endl;
    uint16_t size_mapa_net = htons(size_mapa); //Lo paso a big endian

    unsigned char buf_aux[2];
    memcpy(buf_aux, &size_mapa_net,2); //Hago el casteo a un vector de chars para luego cargarlo al vector

    mensaje_decodificado.insert(mensaje_decodificado.end(),buf_aux, buf_aux + 2);

    char buf_recibido[2];
    std::copy(mensaje_decodificado.begin(),mensaje_decodificado.end(),buf_recibido);



    //Lado del server para recibir el mensaje
    //Hago el camino inverso para recuperar el número que envie

    uint16_t size_mapa2_net;
    memcpy(&size_mapa2_net, buf_recibido, sizeof(uint16_t));
    uint16_t size_mapa2 = ntohs(size_mapa2_net);
    std::cout << size_mapa2 << std::endl;

*/
    std::string hola = "hola";
    int cero = 0;
    char  cero_char = '0';
    cero_char = cero;
    std::string cero_string = std::string(1,cero_char);
    std::string creo = "\000";

    std::stringstream prueba


    if(creo == cero_string){
        cero ++;
    }




    return 0;
}
