#include "game_frame.h"

#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QString>
#include <QWidget>
#include <memory>

#include "mainwindow.h"

GameFrame::GameFrame(QWidget* parent):
        frame(new QWidget(parent)),
        layout(new QHBoxLayout(frame)),
        description(new QLabel(frame)),
        map(new QLabel(frame)),
        players(new QLabel(frame)),
        joinGame(new QPushButton(frame)),
        game_id(0)

{}

void GameFrame::setFrame(const std::string& descrip, const std::string& map_name,
                         const uint8_t& p_quantity, const uint8_t& game_id) {
    this->game_id = game_id;

    std::string quantity(std::to_string(p_quantity));
    quantity = quantity.append(" / 4");

    QString desc = descrip.data();
    QString name_map = map_name.data();
    QString p_quant = quantity.data();

    frame->setLayout(layout);
    frame->setMinimumHeight(70);
    frame->setMaximumHeight(70);

    layout->addWidget(description, 55, Qt::AlignCenter);
    layout->addWidget(map, 15, Qt::AlignCenter);
    layout->addWidget(players, 10, Qt::AlignCenter);
    layout->addWidget(joinGame, 10, Qt::AlignCenter);
    // cppcheck-suppress danglingTemporaryLifetime
    description->setText(desc);
    description->setStyleSheet("color: white;");
    // cppcheck-suppress danglingTemporaryLifetime
    map->setText(name_map);
    map->setStyleSheet("font-size: 16px; color: white;");
    // cppcheck-suppress danglingTemporaryLifetime
    players->setText(p_quant);
    players->setStyleSheet("font-size: 16px; color: white;");

    joinGame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    joinGame->setStyleSheet("border-image: url(:/images/joinGame.bmp);");
    joinGame->setMinimumSize(130, 70);
}

void GameFrame::setHandler(MainWindow& w) {
    MainWindow::connect(joinGame, &QPushButton::clicked, &w, [&w, this]() {
        w.client.action_queue.push(std::make_unique<JoinGame>(this->game_id));
        w.showLobby();
    });
}

void GameFrame::show() {
    frame->show();
    description->show();
    map->show();
    players->show();
    joinGame->show();

    frame->raise();
    description->raise();
    map->raise();
    players->raise();
    joinGame->raise();
}

GameFrame::~GameFrame() {
    delete joinGame;
    delete players;
    delete map;
    delete description;
    delete layout;
    delete frame;
}
