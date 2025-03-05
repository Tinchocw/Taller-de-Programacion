#include <string>
#include <fstream>
#include <iostream>
#include <vector>

struct coordenadas{
        int x;
        int y;
};


int main() {

    std::vector<std::vector<std::vector<char>>> mapas;
    std::vector<std::string> tipo_de_mapa;

    std::ifstream archivo_mapas; //Como quiero leer un archivo lo define de esta forma que se define como i = input, f = file
    archivo_mapas.open("escenarios.txt");

    if(!archivo_mapas.is_open()){
        std::cout <<"No se pude abrir el archivo con el nombre " << "escenarios.txt" << std::endl;
        archivo_mapas.close();
    }

    std::string columnas, filas, nombre_mapa;

    while (!archivo_mapas.eof()){
        archivo_mapas >> columnas >> filas >> nombre_mapa >> std::ws;
        tipo_de_mapa.push_back(nombre_mapa);
        std::vector <std::vector<char>> mapa;
        for(int i = 0; i < stoi(filas); i++){


            std::string fila_aux;
            getline(archivo_mapas,fila_aux);

            std::vector <char> filas_(fila_aux.begin(), fila_aux.end());
            mapa.push_back(filas_);
        }
        mapas.push_back(mapa);
    }


    //Tengo que ver que pasa en este caso cuando quiero guardarme la posición del gusanito, porque capaz me conviene hacerlo cuando
    // estoy leyendo el archivo, ver tema de complejidad.
    std::vector<std::vector<char>> mapa2 (mapas.at(0).begin(),mapas.at(0).end());

    //PROVISIONAL

    //Quiero conseguir del primer mapa la coordenada del jugador
    for(std::size_t i = 0; i < mapa2.size(); i++){
        for(std::size_t j = 0; j < mapa2.at(i).size(); j++){
            if ( mapa2.at(i).at(j) == 'G') {
                std::cout << "Personaje encontrado en: "<< std::endl;
                std::cout << "x: " << i  << std::endl;
                std::cout << "y: " << j  << std::endl;
            }
        }
    }

        return 0;
    }

