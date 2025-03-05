#include <iostream>
#include <sstream>

int main() {

    std::string instruccion;
    while(getline(std::cin,instruccion)){
        std::string comando;
        std::string argumento;

        std::istringstream stream(instruccion);
        stream >> comando >> argumento;

    }



    return 0;
}
