#ifndef BOT_H
#define BOT_H

#include "player.h"
#include "gamestate.h"


class Bot : public Player {

private:
    std::string difficulty;

public:

    explicit Bot(std::string name, MainWindow* mw) : Player(name, mw) {}

    void setDifficulty(std::string param) override { difficulty = param; }

    std::string getDifficulty() override { return difficulty; }

    void botMove() override;

    int evaluateState(const GameState& state);

    int minimax(const GameState& state);

    void chooseBestMove() override;
};

#endif // BOT_H
