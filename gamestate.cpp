#include "gamestate.h"

std::vector<std::string> GameState::getPossibleMoves() const {
    std::vector<std::string> moves;
    if(!secMove){
        for(int i = 0; i < botCard.size(); i++){
            if(jackChoose == ""){
                if(tableCard[tableCard.size() - 1][1] == botCard[i][1] || tableCard[tableCard.size() - 1][0] == botCard[i][0] || botCard[i][0] == 'J'){
                    moves.push_back(botCard[i]);
                }
            }
            else{
                if(jackChoose[0] == botCard[i][1] || tableCard[tableCard.size() - 1][0] == botCard[i][0] || botCard[i][0] == 'J'){
                    moves.push_back(botCard[i]);
                }
            }
        }
    }
    else{
        for(int i = 0; i < botCard.size(); i++){
            if(tableCard[tableCard.size() - 1][0] == botCard[i][0]){
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

    if(Pass > 0 || (jackKol > 0 && chek) || (Bridge && secMove)){
        if(Bridge)
            moves.push_back("bridge");

        moves.push_back("click_end");
    }
    else if(Bridge && !secMove){
        moves.push_back("bridge");
        moves.push_back("take");
    }

    return moves;
}

GameState GameState::simulateMove(const std::string& moveStd) {
    if(moveStd == "bridge"){
        int p = 0;
        for(std::string card : botCard){
            switch (card[0]) {
            case 'A':
                p += 15;
                break;
            case 'K':
                p += 10;
                break;
            case '1':
                p += 10;
                break;
            case 'J':
                p += 20;
                break;
            case 'Q':
                if(card[1] == 'p' && QSMode)
                    p += 50;
                else
                    p += 10;

                break;
            default:
                break;
            }
        }
        p *= PointsX;
        p += Points;
        if((p == 225 && PointsMode) || (p == 125 && !PointsMode) || p == Points)
            discardedByBotScore += 1000;
        else if (((p < 225 && PointsMode) || (p < 125 && !PointsMode)) &&
                 ((PointsMode && ((p - Points) < (225 - Points)/2)) ||
                  (!PointsMode && ((p - Points) < (125 - Points)/2))))
            discardedByBotScore += 100;
        else
            discardedByBotScore -= 100;

        if(jackKol > 0){
            Gmove = 1;
            jackKol = 0;
        }
        else{
            if(tableCard[tableCard.size() - 1] == "Qp" && QSMode){
                opponentCardsCount += 5;
                discardedByBotScore += 50;
            }
            else if(tableCard[tableCard.size() - 1][0] == 'A'){
                discardedByBotScore += (5 * Pass);
            }
            else if(tableCard[tableCard.size() - 1][0] == '8'){
                opponentCardsCount += (2 * Pass);
            }
            Pass = 0;
            Gmove = 1;
        }
    }
    else if(moveStd == "click_end"){
        if(jackKol > 0){
            Gmove = 1;
            jackKol = 0;
        }
        else{
            if(tableCard[tableCard.size() - 1] == "Qp" && QSMode){
                opponentCardsCount += 5;
                discardedByBotScore += 50;
            }
            else if(tableCard[tableCard.size() - 1][0] == 'A'){
                discardedByBotScore += (5 * Pass);
            }
            else if(tableCard[tableCard.size() - 1][0] == '8'){
                opponentCardsCount += (2 * Pass);
            }
            Pass = 0;
            Gmove = 1;
        }
        Bridge = false;
    }
    else if(moveStd == "take"){
        Pass = 0;
        Gmove = 1;
        Bridge = false;
    }
    else{
        tableCard.push_back(moveStd);
        qDebug() << "botSize before: " << botCard.size() << "\n";
        botCard.erase(std::remove(botCard.begin(), botCard.end(), moveStd), botCard.end());
        qDebug() << "botSize after: " << botCard.size() << "\n";
        qDebug() << "tableCard: " << tableCard << "\n";

        if(Bridge)
            Bridge = false;

        int mv = Gmove;
        secondMove();
        operation(mv);
    }

    return GameState(Gmove, QSMode, Bridge, PointsMode, PointsX, Points, secMove, botCard, opponentCardsCount, tableCard, jackChoose,
                     discardedByBot + 1, discardedByBotScore, initialOpponentCardsCount, playerCount, Pass, jackKol);
}

void GameState::secondMove(){
    int mv = Gmove;
    secMove = 0;
    Gmove = 1;

    if(tableCard.size() > 3 && tableCard[tableCard.size() - 1][0] != '6' && tableCard[tableCard.size() - 1][0] == tableCard[tableCard.size() - 2][0]
            && tableCard[tableCard.size() - 1][0] == tableCard[tableCard.size() - 3][0]
        && tableCard[tableCard.size() - 1][0] == tableCard[tableCard.size() - 4][0] && botCard.size() > 0){
        Bridge = true;
    }

    if(tableCard[tableCard.size() - 1][0] == '6'){
        Gmove = mv;
    }
    else{
        for(int i = 0; i < botCard.size(); i++){
            if(botCard[i][0] == tableCard[tableCard.size() - 1][0] || Bridge){
                secMove = 1;
                Gmove = mv;
                break;
            }
        }
    }
}

void GameState::operation(int mv){
    if(tableCard[tableCard.size() - 1] == "Qp" && QSMode == true){
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
    else if(tableCard[tableCard.size() - 1][0] == 'Q'){
        Pass = 0;
        discardedByBotScore += 10;
        jackKol = 0;
    }
    else {
        char card = tableCard[tableCard.size() - 1][0];

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
