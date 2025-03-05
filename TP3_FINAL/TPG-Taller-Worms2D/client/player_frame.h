#ifndef PLAYER_FRAME_H
#define PLAYER_FRAME_H

#include <stdint.h>

class QLabel;
class QPushButton;

class PlayerFrame {
private:
    QLabel* character_label;
    QPushButton* ready_button;
    QLabel* player_id;

public:
    const uint8_t id;
    const bool ready_state;
    /*
        @brief Crea el player frame
    */
    explicit PlayerFrame(const uint8_t& player_id, const bool& ready);
    /*
        @brief Oculta todos los widgets del player frame
    */
    void hide();
    /*
        @brief Setea el player frame con toda la informacion necesaria
    */
    void setFrame(QLabel* label, QPushButton* button, QLabel* id_label);
    /*
        @brief Cambia el estado de ready del jugador, enviando un state al servidor
    */
    void ready();
    /*
        @brief Muestra todos los widgets del frame
    */
    void show();

    ~PlayerFrame() = default;
};


#endif
