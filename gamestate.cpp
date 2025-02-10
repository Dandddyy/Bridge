#include "gamestate.h"

std::vector<std::string> GameState::getPossibleMoves() const {
    std::vector<std::string> moves;
    if(!secMove){
        for(int i = 0; i < botCard.size(); i++){
            if(jackChoose == ""){
                if(tableCard[1] == botCard[i][1] || tableCard[0] == botCard[i][0]){
                    moves.push_back(botCard[i]);
                }
            }
            else{
                if(jackChoose[0] == botCard[i][1] || tableCard[0] == botCard[i][0]){
                    moves.push_back(botCard[i]);
                }
            }
        }
    }
    else{
        for(int i = 0; i < botCard.size(); i++){
            if(tableCard[0] == botCard[i][0]){
                moves.push_back(botCard[i]);
            }
        }
    }

    return moves;
}

GameState GameState::simulateMove(const std::string& moveStd) {
    tableCard = moveStd;
    qDebug() << "botSize before: " << botCard.size() << "\n";
    botCard.erase(std::remove(botCard.begin(), botCard.end(), moveStd), botCard.end());
    qDebug() << "botSize after: " << botCard.size() << "\n";
    qDebug() << "tableCard: " << tableCard << "\n";

    if(botCard.size() > 0 || tableCard[0] == '6'){
        secondMove();
        operation();
    }

    return GameState(Gmove, QSMode, secMove, botCard, opponentCardsCount, tableCard, jackChoose,
                     discardedByBot + 1, discardedByBotScore, initialOpponentCardsCount);
}

void GameState::secondMove(){
    secMove = 0;
    Gmove = 1;
    if(tableCard[0] == '6'){
        Gmove = 2;
    }
    else{
        for(int i = 0; i < botCard.size(); i++){
            if(botCard[i][0] == tableCard[0]){
                secMove = 1;
                Gmove = 2;
            }
        }
    }
}

void GameState::operation(){
    if(tableCard == "Qp" && QSMode == true){
        secMove = 0;
        Gmove = 2;
        opponentCardsCount += 5;
        discardedByBotScore += 50;
    }
    else if(tableCard[0] == 'Q'){
        discardedByBotScore += 10;
    }
    else {
        char card = tableCard[0];

        switch (card) {
        case 'A':
            secMove = 0;
            Gmove = 2;
            discardedByBotScore += 5;
            break;

        case '8':
            secMove = 0;
            Gmove = 2;
            opponentCardsCount += 2;
            break;

        case '7':
            opponentCardsCount += 1;
            break;

        case 'J':
            discardedByBotScore += -1;
            break;

        case 'K':
        case '1':
            discardedByBotScore += 10;
            break;
        }
    }
}
