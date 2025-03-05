#include <iostream>
#include <string>
#include <fstream>
#include <iostream>

    int main() {

        //std::string escenarios = "escenarios.txt";

        std::ifstream archivo_mapas; //Como quiero leer un archivo lo define de esta forma que se define como i = input, f = file
        archivo_mapas.open("escenarios.txt", std::ios::in);

        if(archivo_mapas.fail()){
            std::cout <<"No se pude abrir el archivo con el nombre " << "escenarios.txt" << std::endl;
            archivo_mapas.close();
        }

        std::string columnas, filas, nombre_mapa;
        archivo_mapas >> columnas >> filas >> nombre_mapa;


        return 0;
}
