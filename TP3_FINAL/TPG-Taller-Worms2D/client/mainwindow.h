#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QSoundEffect>
#include <QString>
#include <QTimer>
#include <QWidget>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <stdint.h>

#include "client_event_loop.h"
#include "lobbyListener.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class GameFrame;
}  // namespace Ui
QT_END_NAMESPACE

class PlayerFrame;
class GameFrame;

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    /*
        @brief Carga el funcionamiento de todas las capas del stacked widget
    */
    explicit MainWindow(EventLoop& client, bool& initGame, QWidget* parent = nullptr);
    /*
        @brief Libera la memoria alocada por la ui
    */
    ~MainWindow();
    /*
        @brief Carga los handlers de la intro
    */
    void loadIntro();
    /*
        @brief Muestra la intro
    */
    void showIntro();
    /*
        @brief Carga los handlers del menu
    */
    void loadMenu();
    /*
        @brief Muestra el menu
    */
    void showMenu();
    /*
        @brief Carga los handlers del game search
    */
    void loadGameSearch();
    /*
        @brief Muestra el game search
    */
    void showGameSearch();
    /*
        @brief Recarga todos los games disponibles
    */
    void refreshGameSearch();
    /*
        @brief Carga todos los handlers de Help
    */
    void loadHelp();
    /*
        @brief Muestra la ventana de Help
    */
    void showHelp();
    /*
        @brief Carga todos los handlers de Lobby
    */
    void loadLobby();
    /*
        @brief Muestra el Lobby
    */
    void showLobby();
    /*
        @brief Oculta todos los elementos del lobby
    */
    void lobbyHideAll();
    /*
        @brief Inicia el eventloop
    */
    void startGame();
    /*
        @brief Setea la informacion necesaria para mostrar los player frames
    */
    void setPlayerFrames();
    /*
        Carga el handler del boton de mute
    */
    void loadMuteButton();

    friend class GameFrame;
    friend class PlayerFrame;

private:
    bool muted;
    EventLoop& client;
    Ui::MainWindow* ui;
    QMovie* movie;
    QMovie* movie_aux;
    QTimer* timer;
    int preHelpIndex;
    bool& initGame;
    QSoundEffect sound;
    QSoundEffect sound_aux;
    std::map<uint8_t, std::unique_ptr<PlayerFrame>> players;
    std::vector<std::unique_ptr<GameFrame>> games;

    std::vector<std::tuple<QLabel*, QPushButton*, QLabel*>> lobby_widgets;
    /*
        @brief Valida el input ingresado en el menu de CreateGame
    */
    void validateCreateGame();
    /*
        @brief Sobrecarga del close event, para enviar al servidor comandos de salida
    */
    void closeEvent(QCloseEvent* event);
};

#endif  // MAINWINDOW_H
