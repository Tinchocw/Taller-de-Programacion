#include "cheatmenu.h"

#include "./ui_cheatmenu.h"

#include "Action.h"

CheatMenu::CheatMenu(Queue<std::shared_ptr<Action>>& action_queue, QWidget* parent):
        QDialog(parent), ui(new Ui::CheatMenu), action_queue(action_queue) {
    ui->setupUi(this);
    loadHandlers();
    button_sound.setSource(QUrl::fromLocalFile(":/sounds/button.wav"));
}

void CheatMenu::loadHandlers() {
    connect(ui->ww3Button, &QPushButton::clicked, this, [this]() {
        this->action_queue.push(std::make_shared<WW3Cheat>());
        this->button_sound.play();
    });

    connect(ui->noWindButton, &QPushButton::clicked, this, [this]() {
        this->action_queue.push(std::make_shared<NoWindCheat>());
        this->button_sound.play();
    });

    connect(ui->infiniteAmmoButton, &QPushButton::clicked, this, [this]() {
        this->action_queue.push(std::make_shared<InfiniteAmmoCheat>());
        this->button_sound.play();
    });

    connect(ui->supplyRunButton, &QPushButton::clicked, this, [this]() {
        this->action_queue.push(std::make_shared<SupplyRunCheat>());
        this->button_sound.play();
    });

    connect(ui->inmortalWormButton, &QPushButton::clicked, this, [this]() {
        this->action_queue.push(std::make_shared<ImmortalWorm>());
        this->button_sound.play();
    });

    connect(ui->multipleJumpButton, &QPushButton::clicked, this, [this]() {
        this->action_queue.push(std::make_shared<MultipleJumpCheat>());
        this->button_sound.play();
    });

    connect(ui->infiniteTurnButton, &QPushButton::clicked, this, [this]() {
        this->action_queue.push(std::make_shared<InfiniteTurnCheat>());
        this->button_sound.play();
    });

    connect(ui->everyone1HPButton, &QPushButton::clicked, this, [this]() {
        this->action_queue.push(std::make_shared<Everyone1HPCheat>());
        this->button_sound.play();
    });
}

CheatMenu::~CheatMenu() { delete ui; }
