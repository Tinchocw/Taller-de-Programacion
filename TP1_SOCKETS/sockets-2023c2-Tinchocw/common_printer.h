#ifndef SOCKETS_2023C2_TINCHOCW_CLIENT_PRINTER_H
#define SOCKETS_2023C2_TINCHOCW_CLIENT_PRINTER_H

#include <iostream>
#include <string>
#include <vector>

class Printer {
public:
    void mostrar_coordenada_actual(std::vector<std::string> movimientos);
    void mostrar_mensaje_de_error();
    void mostrar_error_input_cliente(const char* ejecutable);
    void mostrar_error_input_servidor(const char* ejecutable);
};


#endif  // SOCKETS_2023C2_TINCHOCW_CLIENT_PRINTER_H
