#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <string>
#include <QDebug>

class GameState {
private:
    int Gmove;
    bool secMove;
    std::vector<std::string> botCard;
    int opponentCardsCount;
    std::string tableCard;
    std::string jackChoose;
    int discardedByBot;
    int discardedByBotScore;
    int initialOpponentCardsCount;
    bool QSMode;

public:
    GameState(int Gmove, bool QSMode, bool secMove, std::vector<std::string> botCard, int opponentCardsCount,
              std::string tableCard, std::string jackChoose, int discardedByBot,
              int discardedByBotScore, int initialOpponentCardsCount)
        : Gmove(Gmove), QSMode(QSMode), secMove(secMove), botCard(botCard), opponentCardsCount(opponentCardsCount), tableCard(tableCard),
        jackChoose(jackChoose), discardedByBot(discardedByBot),
        discardedByBotScore(discardedByBotScore), initialOpponentCardsCount(initialOpponentCardsCount) {}

    std::vector<std::string> getPossibleMoves() const;

    GameState simulateMove(const std::string& moveStd);

    void secondMove();

    void operation();

    int cardsDiscardedByBot() const { return discardedByBot; }
    int cardsDiscardedByBotScore() const { return discardedByBotScore; }
    int opponentCardCountChange() const { return opponentCardsCount - initialOpponentCardsCount; }
    bool getMove() const { return Gmove; }
};

#endif // GAMESTATE_H
