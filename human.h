#ifndef HUMAN_H
#define HUMAN_H

#include "player.h"

class Human : public Player {

public:
    explicit Human(std::string name, MainWindow* mw) : Player(name, mw) {}


};

#endif // HUMAN_H
