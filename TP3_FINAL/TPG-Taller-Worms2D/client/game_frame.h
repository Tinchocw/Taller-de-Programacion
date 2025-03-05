#ifndef GAME_FRAME_H
#define GAME_FRAME_H

#include <string>

#include <stdint.h>

class QWidget;
class QLabel;
class QHBoxLayout;
class MainWindow;
class QPushButton;


class GameFrame {
public:
    QWidget* frame;

private:
    QHBoxLayout* layout;
    QLabel* description;
    QLabel* map;
    QLabel* players;
    QPushButton* joinGame;

public:
    uint8_t game_id;

    explicit GameFrame(QWidget* parent = 0);
    /*
        @param descrip: Descripcion del juego
        @param map_name: Mapa del juego
        @param p_quantity: Cantidad de jugadores en el juego
        @param game_id: ID del juego

        @brief Setea los valores de los label y botones del game frame
    */
    void setFrame(const std::string& descrip, const std::string& map_name,
                  const uint8_t& p_quantity, const uint8_t& game_id);
    /*
        @brief Setea el handler de joinGame
    */
    void setHandler(MainWindow& w);
    /*
        @brief Muestra todos los widgets
    */
    void show();
    /*
        @brief Libera la memoria de todos los widgets alocados
    */
    ~GameFrame();

    GameFrame(const GameFrame&) = delete;
    GameFrame& operator=(const GameFrame&) = delete;
};


#endif
