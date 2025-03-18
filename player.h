#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include "mainwindow.h"

class Player {
protected:
    std::string name;
    MainWindow* mainWindow;
    std::string cards[36];
    int cardsSize;
    QPushButton* buttons[36] = {nullptr};
    QWidget* layoutWidget = nullptr;
    QLabel* label = nullptr;
    bool checkForTake;
    int points;
    int jackKol;
    int passCount;
    bool inGame;
public:
    explicit Player(std::string name, MainWindow* mw) : name(name), mainWindow(mw), cardsSize(0), checkForTake(false), points(0), jackKol(0), passCount(0), inGame(true) {}

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
        cardsSize = 0;
    }

    virtual void botMove() {}

    virtual void chooseBestMove() {}

    virtual std::string getDifficulty() { return ""; }

    virtual void setDifficulty(std::string param) {}

    void setInGame(bool param) { inGame = param; }

    bool isInGame() const { return inGame; }

    void setPass(int param) { passCount = param; }

    int getPass() const { return passCount; }

    void addCard(std::string card) { cards[cardsSize++] = card; }

    std::string getName() const { return name; }

    void setName(std::string param) { name = param; }

    int getPoints() const { return points; }

    void setPoints(int param) { points = param; }

    int getCardsSize() const { return cardsSize; }

    void setCardsSize(int param) { cardsSize = param; }

    bool getCheckForTake() const { return checkForTake; }

    void setCheckForTake(bool param) { checkForTake = param; }

    int getJackKol() { return jackKol; }

    void setJackKol(int param) { jackKol = param; }

    QWidget* &getLayout() {
        return layoutWidget;
    }

    QLabel* &getLabel() {
        return label;
    }

    std::string (&getCards())[36] {
        return cards;
    }

    QPushButton* (&getButtons())[36] {
        return buttons;
    }

};

#endif // PLAYER_H
