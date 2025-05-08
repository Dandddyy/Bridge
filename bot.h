#ifndef BOT_H
#define BOT_H

#include "human.h"
#include "player.h"
#include "gamestate.h"


class Bot : public Player {

private:
    std::string difficulty;

    void applyBestMove(const std::string& bestMove);

public:

    explicit Bot(std::string name, MainWindow* mw) : Player(name, mw) {}

    Bot(Human& human) : Player(human.getName(), human.getMainWindow()) {
        cardsSize = human.getCardsSize();
        for (int i = 0; i < cardsSize; ++i) {
            cards[i] = human.getCards()[i];
        }

        checkForTake = human.getCheckForTake();
        points = human.getPoints();
        jackKol = human.getJackKol();
        passCount = human.getPass();
        inGame = human.isInGame();
        id = human.getId();

        layoutWidget = human.getLayout();
        label = human.getLabel();
    }

    void setDifficulty(std::string param) override { difficulty = param; }

    std::string getDifficulty() override { return difficulty; }

    void botMove() override;

    int evaluateState(const GameState& state);

    int minimax(const GameState& state);

    void chooseBestMove() override;
};

#endif // BOT_H
