#include "player_frame.h"

#include <QLabel>
#include <QPushButton>
#include <QString>
#include <string>

PlayerFrame::PlayerFrame(const uint8_t& player_id, const bool& ready):
        character_label(nullptr),
        ready_button(nullptr),
        player_id(nullptr),
        id(player_id),
        ready_state(ready) {}

void PlayerFrame::hide() {
    character_label->hide();
    ready_button->hide();
    this->ready_button->setStyleSheet("border-image: url(:/images/notReady.png)");
}

void PlayerFrame::setFrame(QLabel* label, QPushButton* button, QLabel* id_label) {
    this->character_label = label;
    this->ready_button = button;
    this->player_id = id_label;
    this->hide();
    // cppcheck-suppress danglingTemporaryLifetime
    QString lblTxt = std::string("Player ID: " + std::to_string(id)).data();
    // cppcheck-suppress danglingTemporaryLifetime
    id_label->setText(lblTxt);
    this->ready();
}


void PlayerFrame::show() {
    character_label->show();
    ready_button->show();
    player_id->show();
}

void PlayerFrame::ready() {
    this->ready_button->setStyleSheet("border-image: url(:/images/notReady.png)");
    if (ready_state) {
        this->ready_button->setStyleSheet("border-image: url(:/images/ready.png)");
    }
}
