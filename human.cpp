#include "human.h"

void Human::parseCard(std::string card)
{
    for(int i = 0; i < cardsSize; i++){
        if(card == cards[i]){
            if(cards[i][0] == 'J'){
                jackKol++;
            }
            else{
                jackKol = 0;
            }
            mainWindow->setJackchoose("");
            mainWindow->hideLable3();

            mainWindow->setTable(cards[i]);
            const std::string* newtable = mainWindow->getTable();
            const int newtableSize = mainWindow->getTableSize();
            delete buttons[cardsSize - 1];
            cards[i] = "";
            for(int j = i; j < cardsSize - 1; j++){
                cards[j] = cards[j + 1];
                cards[j + 1] = "";
            }
            cardsSize--;

            if(mainWindow->isBridge())
                mainWindow->setBridge(false);

            if(cardsSize > 0 || newtable[newtableSize - 1][0] == '6'){
                checkForTake = 0;
                int mv = mainWindow->isMove();
                mainWindow->secondmove();
                mainWindow->operation(mv);
            }
            mainWindow->AutoSave();
            mainWindow->gameChange();
            if(newtable[newtableSize - 1][0] != '6' && !mainWindow->isBridge()){
                mainWindow->gameEnd();
            }
            break;
        }
    }
}
