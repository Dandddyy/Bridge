#include "bot.h"

void Bot::botMove() {

    bool checkForEnd = false;

    const std::string* table = mainWindow->getTable();
    const int tableSize = mainWindow->getTableSize();

    int mv;
    char JJ;
    int finalmove[9];
    int finalmoveSize = 0;
    int checkWith[9];
    int checkWithSame[9];
    if(mainWindow->getJackchoose() == ""){
        JJ = table[tableSize - 1][1];
    }
    else{
        JJ = mainWindow->getJackchoose()[0];
    }
    for(int i = 0; i < 9; i++){
        checkWith[i] = cardsSize;
        checkWithSame[i] = cardsSize;
        finalmove[i] = cardsSize;
    }
    if(mainWindow->isSecMove() == 0){
        for(int i = 0; i < cardsSize; i++){

            if(cards[i][1] == JJ && cards[i][0] == '6'){
                checkWith[0] = i;
            }
            else if(cards[i][0] == table[tableSize - 1][0] && cards[i][0] == '6'){
                checkWithSame[0] = i;
            }
            if(cards[i][1] == JJ && cards[i][0] == '7'){
                checkWith[1] = i;
            }
            else if(cards[i][0] == table[tableSize - 1][0] && cards[i][0] == '7'){
                checkWithSame[1] = i;
            }
            if(cards[i][1] == JJ && cards[i][0] == '8'){
                checkWith[2] = i;
            }
            else if(cards[i][0] == table[tableSize - 1][0] && cards[i][0] == '8'){
                checkWithSame[2] = i;
            }
            if(cards[i][1] == JJ && cards[i][0] == '9'){
                checkWith[3] = i;
            }
            else if(cards[i][0] == table[tableSize - 1][0] && cards[i][0] == '9'){
                checkWithSame[3] = i;
            }
            if(cards[i][1] == JJ && cards[i][0] == '1'){
                checkWith[4] = i;
            }
            else if(cards[i][0] == table[tableSize - 1][0] && cards[i][0] == '1'){
                checkWithSame[4] = i;
            }
            if(cards[i][0] == 'J'){
                checkWith[5] = i;
            }
            if(cards[i][1] == JJ && cards[i][0] == 'Q'){
                checkWith[6] = i;
            }
            else if(cards[i][0] == table[tableSize - 1][0] && cards[i][0] == 'Q'){
                checkWithSame[6] = i;
            }
            if(cards[i][1] == JJ && cards[i][0] == 'K'){
                checkWith[7] = i;
            }
            else if(cards[i][0] == table[tableSize - 1][0] && cards[i][0] == 'K'){
                checkWithSame[7] = i;
            }
            if(cards[i][1] == JJ && cards[i][0] == 'A'){
                checkWith[8] = i;
            }
            else if(cards[i][0] == table[tableSize - 1][0] && cards[i][0] == 'A'){
                checkWithSame[8] = i;
            }
        }
        if(checkWith[6] < cardsSize && cards[checkWith[6]][1] == 'p' && mainWindow->isQSM() == true){
            finalmove[0] = checkWith[6];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWithSame[6] < cardsSize && cards[checkWithSame[6]][1] == 'p' && mainWindow->isQSM() == true){
            finalmove[0] = checkWithSame[6];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWith[0] < cardsSize && checkWith[8] < cardsSize){
            finalmove[0] = checkWith[0];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWith[8] < cardsSize){
            finalmove[0] = checkWith[8];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWithSame[8] < cardsSize){
            finalmove[0] = checkWithSame[8];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWith[0] < cardsSize && checkWith[2] < cardsSize){
            finalmove[0] = checkWith[0];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWith[2] < cardsSize){
            finalmove[0] = checkWith[2];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWithSame[2] < cardsSize){
            finalmove[0] = checkWithSame[2];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWith[0] < cardsSize && checkWith[7] < cardsSize){
            finalmove[0] = checkWith[0];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWith[0] < cardsSize && checkWith[6] < cardsSize){
            finalmove[0] = checkWith[0];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWith[0] < cardsSize && checkWith[4] < cardsSize){
            finalmove[0] = checkWith[0];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWith[7] < cardsSize){
            finalmove[0] = checkWith[7];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWithSame[7] < cardsSize){
            finalmove[0] = checkWithSame[7];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWith[6] < cardsSize){
            finalmove[0] = checkWith[6];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWithSame[6] < cardsSize){
            finalmove[0] = checkWithSame[6];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWith[4] < cardsSize){
            finalmove[0] = checkWith[4];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWithSame[4] < cardsSize){
            finalmove[0] = checkWithSame[4];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWith[0] < cardsSize && checkWith[1] < cardsSize){
            finalmove[0] = checkWith[0];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWith[1] < cardsSize){
            finalmove[0] = checkWith[1];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWithSame[1] < cardsSize){
            finalmove[0] = checkWithSame[1];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWith[0] < cardsSize && checkWith[3] < cardsSize){
            finalmove[0] = checkWith[0];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWith[3] < cardsSize){
            finalmove[0] = checkWith[3];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWithSame[3] < cardsSize){
            finalmove[0] = checkWithSame[3];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWith[5] < cardsSize){
            int sign[4];
            sign[0] = 0;
            sign[1] = 0;
            sign[2] = 0;
            sign[3] = 0;
            finalmove[0] = checkWith[5];
            finalmoveSize = 1;
            for(int i = 0; i < cardsSize; i++){
                if(cards[i][0] != 'J'){
                    if(cards[i][1] == 'c'){
                        sign[0]++;
                    }
                    else if(cards[i][1] == 'k'){
                        sign[1]++;
                    }
                    else if(cards[i][1] == 'b'){
                        sign[2]++;
                    }
                    else if(cards[i][1] == 'p'){
                        sign[3]++;
                    }
                }
            }
            int k = 0;
            for(int i = 0; i < 4; i++){
                if(sign[k] < sign[i]){
                    k = i;
                }
            }
            if(k == 0){
                mainWindow->setJackchoose("c");
                mainWindow->lable3Style("border-image: url(:/img/PNG-cards-1.3/chirva.png);");
            }
            else if(k == 1){
                mainWindow->setJackchoose("k");
                mainWindow->lable3Style("border-image: url(:/img/PNG-cards-1.3/kresti.png);");
            }
            else if(k == 2){
                mainWindow->setJackchoose("b");
                mainWindow->lable3Style("border-image: url(:/img/PNG-cards-1.3/bybna.png);");
            }
            else if(k == 3){
                mainWindow->setJackchoose("p");
                mainWindow->lable3Style("border-image: url(:/img/PNG-cards-1.3/piki.png);");
            }
        }
        else if(checkWith[0] < cardsSize){
            finalmove[0] = checkWith[0];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkWithSame[0] < cardsSize){
            finalmove[0] = checkWithSame[0];
            finalmoveSize = 1;
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();
        }
        else if(checkForTake == 0){
            mainWindow->RemoveWidgetBot(0);
            if(table[tableSize - 1][0] == '6'){
                checkForTake = 0;
            }
        }
        else{
            mainWindow->botNoChoice();
            checkForTake = 0;
        }
    }
    else if(mainWindow->isSecMove() == 1 && cardsSize > 0){
        for(int i = 0; i < cardsSize; i++){
            if(cards[i][0] == table[tableSize - 1][0]){
                finalmove[0] = i;
                finalmoveSize = 1;
                mainWindow->setJackchoose("");
                mainWindow->hideLable3();
                if(cards[i][0] == 'J'){
                    int sign[4];
                    sign[0] = 0;
                    sign[1] = 0;
                    sign[2] = 0;
                    sign[3] = 0;
                    for(int i = 0; i < cardsSize; i++){
                        if(cards[i][1] == 'c'){
                            sign[0]++;
                        }
                        else if(cards[i][1] == 'k'){
                            sign[1]++;
                        }
                        else if(cards[i][1] == 'b'){
                            sign[2]++;
                        }
                        else if(cards[i][1] == 'p'){
                            sign[3]++;
                        }
                    }
                    int k = 0;
                    for(int i = 0; i < 4; i++){
                        if(sign[k] < sign[i]){
                            k = i;
                        }
                    }
                    if(k == 0){
                        mainWindow->setJackchoose("c");
                        mainWindow->lable3Style("border-image: url(:/img/PNG-cards-1.3/chirva.png);");
                    }
                    else if(k == 1){
                        mainWindow->setJackchoose("k");
                        mainWindow->lable3Style("border-image: url(:/img/PNG-cards-1.3/kresti.png);");
                    }
                    else if(k == 2){
                        mainWindow->setJackchoose("b");
                        mainWindow->lable3Style("border-image: url(:/img/PNG-cards-1.3/bybna.png);");
                    }
                    else if(k == 3){
                        mainWindow->setJackchoose("p");
                        mainWindow->lable3Style("border-image: url(:/img/PNG-cards-1.3/piki.png);");
                    }
                }

            }
        }
    }
    if(finalmoveSize > 0 && finalmove[0] < cardsSize){
        mainWindow->cardSound();
        if(cards[finalmove[finalmoveSize - 1]][0] == 'J'){
            jackKol++;
        }
        else{
            jackKol = 0;
        }
        for(int i = 0; i < finalmoveSize; i++){
            mainWindow->setTable(cards[finalmove[i]]);
            delete buttons[cardsSize - 1];
            cards[finalmove[i]] = "";
            for(int j = finalmove[i]; j < cardsSize - 1; j++){
                cards[j] = cards[j + 1];
                cards[j + 1] = "";
            }
            if(finalmoveSize > 1 && i < finalmoveSize - 1){
                if(finalmove[i] < finalmove[i + 1]){
                    finalmove[i + 1]--;
                }
            }
            cardsSize--;
        }
        const std::string* newtable = mainWindow->getTable();
        const int newtableSize = mainWindow->getTableSize();
        if(cardsSize > 0 || newtable[newtableSize - 1][0] == '6'){
            checkForTake = 0;
            mv = mainWindow->isMove();
            mainWindow->secondmove();
            mainWindow->operation(mv);
        }
        if(newtable[newtableSize - 1][0] != '6'){
            checkForEnd = true;
            mainWindow->AutoSave();
            mainWindow->gameEnd();
        }
    }
    if(!checkForEnd){
        mainWindow->AutoSave();
    }
}

int Bot::evaluateState(const GameState& state) {
    const int cardDiscardedWeight = 15;
    const int opponentCardCountWeight = 10;
    qDebug() << "state.cardsDiscardedByBot():" << state.cardsDiscardedByBot() << "\n";
    qDebug() << "state.opponentCardCountChange():" << state.opponentCardCountChange() << "\n";
    qDebug() << "state.cardsDiscardedByBotScore():" << state.cardsDiscardedByBotScore() << "\n";

    int score = 0;
    score += (cardDiscardedWeight * state.cardsDiscardedByBot()) + state.cardsDiscardedByBotScore();
    score += opponentCardCountWeight * state.opponentCardCountChange();

    return score;
}

int Bot::minimax(const GameState& state) {
    if (state.getMove() == 1) {
        qDebug() << "getMove:" << state.getMove() << "\n";
        return evaluateState(state);
    }
    else{

        std::vector<std::string> possibleMoves = state.getPossibleMoves();

        int bestValue = INT_MIN;

        if(possibleMoves.empty()){
            return evaluateState(state);
        }
        else{
            for (std::string move : possibleMoves) {
                GameState newState = state;
                newState = newState.simulateMove(move);
                bestValue = std::max(bestValue, minimax(newState));
            }
        }
        return bestValue;
    }
}

void Bot::chooseBestMove() {

    bool checkForEnd = false;

    const std::string* table = mainWindow->getTable();
    const int tableSize = mainWindow->getTableSize();

    std::vector<std::string> vectorBot(cards, cards + cardsSize);
    int count = std::min(tableSize, 4);
    std::vector<std::string> vectorTable(table + (tableSize - count), table + tableSize);


    GameState state = GameState(mainWindow->isMove(), mainWindow->isQSM(), mainWindow->isBridge(), mainWindow->isPoints(), mainWindow->getPointsX(),
                                points, mainWindow->isSecMove(), vectorBot, mainWindow->getPlayercardsSize(),vectorTable,mainWindow->getJackchoose(),
                                0, 0, mainWindow->getPlayercardsSize(),mainWindow->getPlayersCount(), passCount, jackKol);

    std::vector<std::string> possibleMoves = state.getPossibleMoves();
    std::string bestMove = "";
    int bestValue = INT_MIN;

    if(possibleMoves.empty() && checkForTake == 0){
        mainWindow->RemoveWidgetBot(0);
        if(table[tableSize - 1][0] == '6'){
            checkForTake = 0;
        }
    }
    else if(possibleMoves.empty()){
        mainWindow->botNoChoice();
        checkForTake = 0;
        jackKol = 0;
    }
    else{
        int mv;
        int it = 0;

        for (std::string moveS : possibleMoves) {
            qDebug() << "moveS: " << moveS <<"\n";
            qDebug() << "cards: ";
            for (int i = 0; i < cardsSize; i++){
                qDebug() << cards[i] << " ";
            }
            GameState newState = state;
            newState = newState.simulateMove(moveS);
            int moveValue = minimax(newState);
            qDebug() << "possibleMoves.size: " << possibleMoves.size() << ", moveVal:" << moveValue << "\n";
            if (moveValue > bestValue) {
                bestValue = moveValue;
                bestMove = moveS;
            }
        }

        if(bestMove != "click_end" && bestMove != "bridge" && bestMove != "take"){
            for(int i = 0; i < cardsSize; i++){
                if(bestMove == cards[i]){
                    it = i;
                    qDebug() << "it: " << it << ", i:" << i << "\n";
                }
            }

            qDebug() << "----------------------------------\n";

            mainWindow->cardSound();

            if(bestMove[0] == 'J'){
                jackKol++;

                int sign[4];
                sign[0] = 0;
                sign[1] = 0;
                sign[2] = 0;
                sign[3] = 0;

                for(int i = 0; i < cardsSize; i++){
                    if(cards[i][0] != 'J'){
                        if(cards[i][1] == 'c'){
                            sign[0]++;
                        }
                        else if(cards[i][1] == 'k'){
                            sign[1]++;
                        }
                        else if(cards[i][1] == 'b'){
                            sign[2]++;
                        }
                        else if(cards[i][1] == 'p'){
                            sign[3]++;
                        }
                    }
                }
                int k = 0;
                for(int i = 0; i < 4; i++){
                    if(sign[k] < sign[i]){
                        k = i;
                    }
                }
                if(k == 0){
                    mainWindow->setJackchoose("c");
                    mainWindow->lable3Style("border-image: url(:/img/PNG-cards-1.3/chirva.png);");
                }
                else if(k == 1){
                    mainWindow->setJackchoose("k");
                    mainWindow->lable3Style("border-image: url(:/img/PNG-cards-1.3/kresti.png);");
                }
                else if(k == 2){
                    mainWindow->setJackchoose("b");
                    mainWindow->lable3Style("border-image: url(:/img/PNG-cards-1.3/bybna.png);");
                }
                else if(k == 3){
                    mainWindow->setJackchoose("p");
                    mainWindow->lable3Style("border-image: url(:/img/PNG-cards-1.3/piki.png);");
                }
            }
            else{
                jackKol = 0;
                mainWindow->setJackchoose("");
                mainWindow->hideLable3();
            }

            mainWindow->setTable(cards[it]);
            const std::string* newtable = mainWindow->getTable();
            const int newtableSize = mainWindow->getTableSize();
            delete buttons[cardsSize - 1];
            cards[it] = "";
            for(int j = it; j < cardsSize - 1; j++){
                cards[j] = cards[j + 1];
                cards[j + 1] = "";
            }
            cardsSize--;

            if(mainWindow->isBridge())
                mainWindow->setBridge(false);

            if(cardsSize > 0 || newtable[newtableSize - 1][0] == '6'){
                checkForTake = 0;
                mv = mainWindow->isMove();
                mainWindow->secondmove();
                mainWindow->operation(mv);
            }
            if(newtable[newtableSize - 1][0] != '6' && !mainWindow->isBridge()){
                checkForEnd = true;
                mainWindow->AutoSave();
                mainWindow->gameEnd();
            }
        }
        else if(bestMove == "click_end"){
            mainWindow->endClicked();
            mainWindow->setBridge(false);
        }
        else if(bestMove == "take"){
            mainWindow->RemoveWidgetBot(0);
            if(table[tableSize - 1][0] == '6'){
                checkForTake = 0;
            }
        }
        else{
            checkForEnd = true;
            mainWindow->AutoSave();
            mainWindow->gameEnd();
        }
    }
    if(!checkForEnd){
        mainWindow->AutoSave();
    }
}
