#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <string>
#include <QDebug>

class GameState {
private:
    int Gmove;
    bool QSMode;
    bool Bridge;
    bool PointsMode;
    int PointsX;
    int Points;
    bool secMove;
    std::vector<std::string> botCard;
    int opponentCardsCount;
    std::vector<std::string> tableCard;
    std::string jackChoose;
    int discardedByBot;
    int discardedByBotScore;
    int initialOpponentCardsCount;
    int playerCount;
    int Pass;
    int jackKol;

    void secondMove();

    void operation(int mv);

public:
    GameState(int Gmove, bool QSMode, bool Bridge, bool PointsMode, int PointsX, int Points, bool secMove, std::vector<std::string> botCard,
              int opponentCardsCount,std::vector<std::string> tableCard, std::string jackChoose, int discardedByBot,
              int discardedByBotScore,int initialOpponentCardsCount, int playerCount, int Pass, int jackKol)
        : Gmove(Gmove), QSMode(QSMode), Bridge(Bridge), PointsMode(PointsMode), PointsX(PointsX), Points(Points), secMove(secMove),
        botCard(botCard),opponentCardsCount(opponentCardsCount), tableCard(tableCard),jackChoose(jackChoose), discardedByBot(discardedByBot),
        discardedByBotScore(discardedByBotScore), initialOpponentCardsCount(initialOpponentCardsCount), playerCount(playerCount),
        Pass(Pass), jackKol(jackKol) {}

    std::vector<std::string> getPossibleMoves() const;

    GameState simulateMove(const std::string& moveStd);

    int cardsDiscardedByBot() const { return discardedByBot; }
    int cardsDiscardedByBotScore() const { return discardedByBotScore; }
    int opponentCardCountChange() const { return opponentCardsCount - initialOpponentCardsCount; }
    int getMove() const { return Gmove; }
};

#endif // GAMESTATE_H
