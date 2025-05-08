#ifndef HUMAN_H
#define HUMAN_H

#include "player.h"

class Human : public Player {

public:
    explicit Human(std::string name, MainWindow* mw) : Player(name, mw) {}

    void parseCard(std::string card) override;
};

#endif // HUMAN_H
