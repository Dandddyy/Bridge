#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <QDebug>
#include <QPushButton>
#include "mainwindow.h"

class Player {
protected:
    int id;
    std::string name;
    MainWindow* mainWindow;
    std::string cards[36];
    int cardsSize;
    QPushButton* buttons[36] = {nullptr};
    bool checkForTake;
    int points;
    int jackKol;
public:
    explicit Player(int id, MainWindow* mw) : id(id), mainWindow(mw), cardsSize(0), checkForTake(false), points(0), jackKol(0) {}

    virtual ~Player() {
        for (int i = 0; i < cardsSize; i++) {
            delete buttons[i];
            buttons[i] = nullptr;
        }
    }

    void deleteButtons() {
        for(int i = 0; i < cardsSize; i++){
            delete buttons[i];
            buttons[i] = nullptr;
            cards[i] = "";
        }
    }

    int getPoints() const { return points; }

    void setPoints(int param) { points = param; }

    int getCardsSize() const { return cardsSize; }

    void setCardsSize(int param) { cardsSize = param; }

    bool getCheckForTake() const { return checkForTake; }

    void setCheckForTake(bool param) { checkForTake = param; }

    int getJackKol() { return jackKol; }

    void setJackKol(int param) { jackKol = param; }

    std::string (&getCards())[36] {
        return cards;
    }

    QPushButton* (&getButtons())[36] {
        return buttons;
    }

};

#endif // PLAYER_H
