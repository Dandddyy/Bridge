#ifndef HUMAN_H
#define HUMAN_H

#include "player.h"

class Human : public Player {

public:
    explicit Human(int id, MainWindow* mw) : Player(id, mw) {}


};

#endif // HUMAN_H
