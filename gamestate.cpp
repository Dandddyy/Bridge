#include "gamestate.h"

std::vector<std::string> GameState::getPossibleMoves() const {
    std::vector<std::string> moves;
    if(!secMove){
        for(int i = 0; i < botCard.size(); i++){
            if(jackChoose == ""){
                if(tableCard[1] == botCard[i][1] || tableCard[0] == botCard[i][0] || botCard[i][0] == 'J'){
                    moves.push_back(botCard[i]);
                }
            }
            else{
                if(jackChoose[0] == botCard[i][1] || tableCard[0] == botCard[i][0] || botCard[i][0] == 'J'){
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

    bool chek = false;

    for(std::string move : moves){
        if(move[0] == 'J'){
            chek = true;
        }
    }

    if(Pass > 0 || (jackKol > 0 && chek))
        moves.push_back("click_end");

    return moves;
}

GameState GameState::simulateMove(const std::string& moveStd) {
    if(moveStd == "click_end"){
        if(jackKol > 0){
            Gmove = 1;
            jackKol = 0;
        }
        else{
            if(tableCard == "Qp" && QSMode){
                Pass = 0;
                opponentCardsCount += 5;
                discardedByBotScore += 50;
                Gmove = 1;
            }
            else if(tableCard[0] == 'A'){
                Pass = 0;
                discardedByBotScore += (5 * Pass);
                Gmove = 1;
            }
            else if(tableCard[0] == '8'){
                Pass = 0;
                opponentCardsCount += (2 * Pass);
                Gmove = 1;
            }
            secMove = 0;
            jackKol = 0;
        }
    }
    else{
        tableCard = moveStd;
        qDebug() << "botSize before: " << botCard.size() << "\n";
        botCard.erase(std::remove(botCard.begin(), botCard.end(), moveStd), botCard.end());
        qDebug() << "botSize after: " << botCard.size() << "\n";
        qDebug() << "tableCard: " << tableCard << "\n";

        int mv = Gmove;
        secondMove();
        operation(mv);
    }

    return GameState(Gmove, QSMode, secMove, botCard, opponentCardsCount, tableCard, jackChoose,
                     discardedByBot + 1, discardedByBotScore, initialOpponentCardsCount, playerCount, Pass, jackKol);
}

void GameState::secondMove(){
    int mv = Gmove;
    secMove = 0;
    Gmove = 1;
    if(tableCard[0] == '6'){
        Gmove = mv;
    }
    else{
        for(int i = 0; i < botCard.size(); i++){
            if(botCard[i][0] == tableCard[0]){
                secMove = 1;
                Gmove = mv;
            }
        }
    }
}

void GameState::operation(int mv){
    if(tableCard == "Qp" && QSMode == true){
        if(!secMove || playerCount == 2){
            secMove = 0;
            if(playerCount == 2)
                Gmove = mv;
            else
                Gmove = 1;
            opponentCardsCount += 5;
            discardedByBotScore += 50;
        }
        else{
            Pass = 1;
        }
        jackKol = 0;
    }
    else if(tableCard[0] == 'Q'){
        Pass = 0;
        discardedByBotScore += 10;
        jackKol = 0;
    }
    else {
        char card = tableCard[0];

        switch (card) {
        case 'A':
            if(playerCount == Pass + 2){
                secMove = 0;
                Gmove = mv;
                discardedByBotScore += 5;
                jackKol = 0;
                Pass = 0;
                break;
            }
            else if(secMove){
                Pass++;
                jackKol = 0;
                break;
            }
            else{
                Gmove = 1;
                for(int i = 0; i < Pass + 1; i++){
                    discardedByBotScore += 5;
                }
                Pass = 0;
                jackKol = 0;
                break;
            }
            break;

        case '8':
            if(playerCount == Pass + 2){
                secMove = 0;
                Gmove = mv;
                opponentCardsCount += 2;
                jackKol = 0;
                Pass = 0;
                break;
            }
            else if(secMove){
                Pass++;
                jackKol = 0;
                break;
            }
            else{
                Gmove = 1;
                for(int i = 0; i < Pass + 1; i++){
                    opponentCardsCount += 2;
                }
                Pass = 0;
                jackKol = 0;
                break;
            }
            break;

        case '7':
            opponentCardsCount += 1;
            jackKol = 0;
            break;

        case 'J':
            if(botCard.size() == 0){
                discardedByBotScore += 100;
            }
            else{
                discardedByBotScore += (0 - (opponentCardsCount * 3) - (jackKol * 2));
            }
            jackKol++;
            break;

        case 'K':
        case '1':
            discardedByBotScore += 10;
            jackKol = 0;
            break;
        }
    }
}
