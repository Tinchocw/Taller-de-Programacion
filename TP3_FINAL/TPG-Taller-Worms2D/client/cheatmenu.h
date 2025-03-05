#ifndef CHEATMENU_H
#define CHEATMENU_H

#include <QApplication>
#include <QDialog>
#include <QSoundEffect>
#include <memory>

#include "../common/queue.h"

class Action;

QT_BEGIN_NAMESPACE
namespace Ui {
class CheatMenu;
}
QT_END_NAMESPACE

class CheatMenu: public QDialog {
    Q_OBJECT

public:
    /*
        @param action_queue: Action queue del cliente, utilizada para enviar los cheats
    */
    explicit CheatMenu(Queue<std::shared_ptr<Action>>& action_queue, QWidget* parent = nullptr);
    /*
        @brief Libera la memoria alocada por la ui
    */
    ~CheatMenu();

private:
    Ui::CheatMenu* ui;
    Queue<std::shared_ptr<Action>>& action_queue;
    QSoundEffect button_sound;
    /*
        @brief Carga los handlers de todos los botones del cheatmenu
    */
    void loadHandlers();
};

#endif  // CHEATMENU_H
