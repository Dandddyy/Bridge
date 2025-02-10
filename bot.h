#ifndef BOT_H
#define BOT_H

#include "player.h"
#include "gamestate.h"


class Bot : public Player {

private:
    std::string difficulty;

public:

    explicit Bot(int id, MainWindow* mw) : Player(id, mw) {}

    void botMove();

    int evaluateState(const GameState& state);

    int minimax(const GameState& state);

    void chooseBestMove();
};

#endif // BOT_H
