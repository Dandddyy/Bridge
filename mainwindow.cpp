#include "mainwindow.h"
#include <QMessageBox>
#include "ui_mainwindow.h"
#include <fstream>
#include <QUrl>
#include <QScrollBar>
#include <QWheelEvent>
#include <QApplication>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    window = new optionwindow;

    ui->scrollArea->installEventFilter(this);

    ui->pushButton_9->hide();

    QString saveDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Saves";
    QDir().mkpath(saveDir);
    QDir().mkpath(saveDir);

    dirGame = saveDir + "/GameData.txt";
    dirOption = saveDir + "/OptionsData.txt";

    isInGame = false;
    isFromSet = false;
    isShuffl = false;

    setCentralWidget(ui->centralwidget);

    std::ifstream fin;
    fin.open(dirGame.toStdString());
    if(fin.is_open()){
        std::string getstr;
        fin >> getstr;
        if(getstr == "Can_Resume"){
             ui->pushButton_9->show();
             ui->pushButton_6->setGeometry(338, 160, 121, 71);
             ui->pushButton_6->setText("Start The\nNew Game");
             ui->pushButton_6->setStyleSheet("font-family: 'Segoe UI'; font-size: 12pt; border: 2px solid; border-color: black; border-radius: 20px; background-color: white; color: black; font: bold;");
        }
    }
    fin.close();

    connect(this, &MainWindow::QSMsignal2, window, &optionwindow::QSMslot2);

    fin.open(dirOption.toStdString());
    if(fin.is_open()){
        fin >> QSMode;
        fin >> PointsMode;
        fin >> difficulty;
        fin >> musicvol;
        fin >> soundvol;
        fin >> isFullscreen;
        if(QSMode == 0 || QSMode == 1){
            emit QSMsignal2(QSMode, PointsMode, difficulty, musicvol, soundvol, isFullscreen);
        }
    }
    else{
        QSMode = false;
        PointsMode = false;
        difficulty = "Middle";
        musicvol = 50;
        soundvol = 50;
        isFullscreen = true;
        emit QSMsignal2(QSMode, PointsMode, difficulty, musicvol, soundvol, isFullscreen);
    }
    fin.close();

    playerCardsSize = 0;
    botCardsSize = 0;
    ColodCardsSize = 0;
    tableCardsSize = 0;
    playerPoints = 0;
    botPoints = 0;
    Set = 1;

    ui->label->setVisible(false);
    ui->label_2->setVisible(false);

    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    ui->pushButton_10->hide();
    ui->pushButton_11->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_8->hide();
    ui->label_11->hide();
    ui->label_12->hide();
    ui->label_13->hide();
    ui->scrollArea->hide();

    checkForTake = 0;
    BcheckForTake = 0;

    PointsX = 1;
    ui->label_7->setText(QString::fromStdString("Points " + std::to_string(PointsX) + "x"));

    pJackKol = 0;
    bJackKol = 0;

    QImage im(":/img/PNG-cards-1.3/icon.png");
    ui->label_9->setScaledContents(true);
    ui->label_9->setPixmap(QPixmap::fromImage(im));

    QImage ima(":/img/PNG-cards-1.3/QSM.png");
    ui->label_11->setScaledContents(true);
    ui->label_11->setPixmap(QPixmap::fromImage(ima));

    QIcon ic(":/img/PNG-cards-1.3/kindpng_4637727.png");
    ui->pushButton_7->setIcon(ic);
    ui->pushButton_11->setIcon(ic);
    QIcon ic1(":/img/PNG-cards-1.3/exit-icon-4597.png");
    ui->pushButton_10->setIcon(ic1);
    ui->pushButton_8->setIcon(ic1);

    connect(window, &optionwindow::QSMsignal, this, &MainWindow::QSMslot);
    connect(window, &optionwindow::Pointssignal, this, &MainWindow::Pointsslot);
    connect(window, &optionwindow::Difficultysignal, this, &MainWindow::Difficultyslot);
    connect(window, &optionwindow::Musicsignal, this, &MainWindow::Musicslot);
    connect(window, &optionwindow::Soundsignal, this, &MainWindow::Soundslot);
    connect(window, &optionwindow::Displaysignal, this, &MainWindow::Displayslot);

    tempFilePathMenu = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/MenuMusic.mp3";
    QFileInfo checkFile1(tempFilePathMenu);
    if (!checkFile1.exists() || !checkFile1.isFile()) {
        QFile resourceFile(":/sd/Sound/MenuMusic.mp3");
        resourceFile.copy(tempFilePathMenu);
    }

    QString soundDir = QCoreApplication::applicationDirPath() + "/Sound";
    QDir().mkpath(soundDir);

    tempFilePathTheme = soundDir + "/MainTheme.mp3";
    QFileInfo checkFile2(tempFilePathTheme);
    if (!checkFile2.exists() || !checkFile2.isFile()) {
        QFile resourceFile(":/sd/Sound/MainTheme.mp3");
        resourceFile.copy(tempFilePathTheme);
    }

    tempFilePathClick = soundDir + "/click.wav";
    QFileInfo checkFile3(tempFilePathClick);
    if (!checkFile3.exists() || !checkFile3.isFile()) {
        QFile resourceFile(":/sd/Sound/click.wav");
        resourceFile.copy(tempFilePathClick);
    }

    tempFilePathCard = soundDir + "/card.wav";
    QFileInfo checkFile4(tempFilePathCard);
    if (!checkFile4.exists() || !checkFile4.isFile()) {
        QFile resourceFile(":/sd/Sound/card.wav");
        resourceFile.copy(tempFilePathCard);
    }

    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl::fromLocalFile(tempFilePathMenu));
    audioOutput->setVolume((musicvol / 100.0) * 0.1);
    player->setLoops(-1);
    player->play();

    soundplayer = new QSoundEffect;
    soundplayer->setVolume(soundvol / 100.0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->scrollArea && event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);

        int delta = wheelEvent->pixelDelta().x();
        if (delta == 0) {
            delta = wheelEvent->angleDelta().y() * 0.5;
        }
        ui->scrollArea->horizontalScrollBar()->setValue(
            ui->scrollArea->horizontalScrollBar()->value() - delta
            );

        return true;
    }

    return QMainWindow::eventFilter(obj, event);
}

class GameState {
private:
    bool Gmove;
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
    GameState(bool Gmove, bool QSMode, bool secMove, std::vector<std::string> botCard, int opponentCardsCount,
              std::string tableCard, std::string jackChoose, int discardedByBot,
              int discardedByBotScore, int initialOpponentCardsCount)
        : Gmove(Gmove), QSMode(QSMode), secMove(secMove), botCard(botCard), opponentCardsCount(opponentCardsCount), tableCard(tableCard),
        jackChoose(jackChoose), discardedByBot(discardedByBot),
        discardedByBotScore(discardedByBotScore), initialOpponentCardsCount(initialOpponentCardsCount) {}

    std::vector<std::string> getPossibleMoves() {
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

    GameState simulateMove(std::string moveStd) {
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

    void secondMove(){
        secMove = 0;
        Gmove = 0;
        if(tableCard[0] == '6'){
            Gmove = 1;
        }
        else{
            for(int i = 0; i < botCard.size(); i++){
                if(botCard[i][0] == tableCard[0]){
                    secMove = 1;
                    Gmove = 1;
                }
            }
        }
    }

    void operation(){
        if(tableCard == "Qp" && QSMode == true){
            secMove = 0;
            Gmove = 1;
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
                Gmove = 1;
                break;

            case '8':
                secMove = 0;
                Gmove = 1;
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

    int cardsDiscardedByBot() const { return discardedByBot; }
    int cardsDiscardedByBotScore() const { return discardedByBotScore; }
    int opponentCardCountChange() const { return initialOpponentCardsCount - opponentCardsCount; }
    bool getMove() const { return Gmove; }
};

int evaluateState(const GameState& state) {
    const int cardDiscardedWeight = 15;
    const int opponentCardCountWeight = 10;

    int score = 0;
    score += (cardDiscardedWeight * state.cardsDiscardedByBot()) + state.cardsDiscardedByBotScore();
    score += opponentCardCountWeight * state.opponentCardCountChange();

    return score;
}

int minimax(GameState state) {
    if (state.getMove() == 0) {
        qDebug() << "getMove:" << state.getMove() << "\n";
        return evaluateState(state);
    }

    std::vector<std::string> possibleMoves = state.getPossibleMoves();

    int bestValue = INT_MIN;

    if(possibleMoves.empty()){
        return evaluateState(state);
    }
    else{
        for (std::string move : possibleMoves) {
            GameState newState = state.simulateMove(move);
            bestValue = std::max(bestValue, minimax(newState));
        }
    }
    return bestValue;
}

void MainWindow::chooseBestMove() {

    if(Mmove){
        timer->start(1000);

        std::vector<std::string> vector(botCards, botCards + botCardsSize);

        GameState state = GameState(Mmove, QSMode, secMove, vector, playerCardsSize,
                                    tableCards[tableCardsSize - 1], Jackchoose, 0, 0, playerCardsSize);

        std::vector<std::string> possibleMoves = state.getPossibleMoves();
        std::string bestMove = "";
        int bestValue = INT_MIN;

        if(possibleMoves.empty() && BcheckForTake == 0){
            onRemoveWidgetColodBot();
            if(tableCards[tableCardsSize - 1][0] == '6'){
                BcheckForTake = 0;
            }
        }
        else if(possibleMoves.empty()){
            secMove = 0;
            Mmove = 0;
            ui->label_2->setText("Your turn");
            ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
            if(isFullscreen && isInGame){
                QString cs = ui->label_2->styleSheet();
                ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
            }
            BcheckForTake = 0;
            checkForTake = 0;
        }
        else{
            bool mv;
            int it = 0;

            for (std::string moveS : possibleMoves) {
                qDebug() << "moveS: " << moveS <<"\n";
                GameState newState = state.simulateMove(moveS);
                int moveValue = minimax(newState);
                qDebug() << "possibleMoves.size: " << possibleMoves.size() << ", moveVal:" << moveValue << "\n";
                if (moveValue > bestValue) {
                    bestValue = moveValue;
                    bestMove = moveS;
                }
            }

            for(int i = 0; i < botCardsSize; i++){
                if(bestMove == botCards[i]){
                    it = i;
                    qDebug() << "it: " << it << ", i:" << i << "\n";
                }
            }

            qDebug() << "----------------------------------\n";

            soundplayer->stop();
            soundplayer->setSource(QUrl::fromLocalFile(tempFilePathCard));
            soundplayer->setVolume(soundvol / 100.0);
            soundplayer->play();

            if(bestMove[0] == 'J'){
                bJackKol++;

                int sign[4];
                sign[0] = 0;
                sign[1] = 0;
                sign[2] = 0;
                sign[3] = 0;

                for(int i = 0; i < botCardsSize; i++){
                    if(botCards[i][0] != 'J'){
                        if(botCards[i][1] == 'c'){
                            sign[0]++;
                        }
                        else if(botCards[i][1] == 'k'){
                            sign[1]++;
                        }
                        else if(botCards[i][1] == 'b'){
                            sign[2]++;
                        }
                        else if(botCards[i][1] == 'p'){
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
                    Jackchoose = "c";
                    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/chirva.png);");
                    ui->label_3->show();
                }
                else if(k == 1){
                    Jackchoose = "k";
                    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/kresti.png);");
                    ui->label_3->show();
                }
                else if(k == 2){
                    Jackchoose = "b";
                    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/bybna.png);");
                    ui->label_3->show();
                }
                else if(k == 3){
                    Jackchoose = "p";
                    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/piki.png);");
                    ui->label_3->show();
                }
            }
            else{
                bJackKol = 0;
                Jackchoose = "";
                ui->label_3->hide();
            }

            tableCards[tableCardsSize] = botCards[it];
            onAddWidgetTable(tableCards[tableCardsSize], tableCardsSize);
            tableCardsSize++;
            delete botButtons[it];
            botCards[it] = "";
            for(int j = it; j < botCardsSize - 1; j++){
                onAddWidgetBot(j);
                delete botButtons[j + 1];
                botCards[j] = botCards[j + 1];
                botCards[j + 1] = "";
            }
            botCardsSize--;
            if(botCardsSize > 0 || tableCards[tableCardsSize - 1][0] == '6'){
                mv = Mmove;
                secondmove();
                operation(mv);
            }
            if(tableCards[tableCardsSize - 1][0] != '6'){
                gameEnd();
            }
        }
        AutoSave();
    }
    else{
        timer->start(2000);
    }
}

void MainWindow::botMove()
{
    if(Mmove == 1){
        timer->start(1000);
        bool mv;
        char JJ;
        int finalmove[9];
        int finalmoveSize = 0;
        int checkWith[9];
        int checkWithSame[9];
        if(Jackchoose == ""){
            JJ = tableCards[tableCardsSize - 1][1];
        }
        else{
            JJ = Jackchoose[0];
        }
        for(int i = 0; i < 9; i++){
            checkWith[i] = botCardsSize;
            checkWithSame[i] = botCardsSize;
            finalmove[i] = botCardsSize;
        }
        if(secMove == 0){
            for(int i = 0; i < botCardsSize; i++){

                if(botCards[i][1] == JJ && botCards[i][0] == '6'){
                    checkWith[0] = i;
                }
                else if(botCards[i][0] == tableCards[tableCardsSize - 1][0] && botCards[i][0] == '6'){
                    checkWithSame[0] = i;
                }
                if(botCards[i][1] == JJ && botCards[i][0] == '7'){
                    checkWith[1] = i;
                }
                else if(botCards[i][0] == tableCards[tableCardsSize - 1][0] && botCards[i][0] == '7'){
                    checkWithSame[1] = i;
                }
                if(botCards[i][1] == JJ && botCards[i][0] == '8'){
                    checkWith[2] = i;
                }
                else if(botCards[i][0] == tableCards[tableCardsSize - 1][0] && botCards[i][0] == '8'){
                    checkWithSame[2] = i;
                }
                if(botCards[i][1] == JJ && botCards[i][0] == '9'){
                    checkWith[3] = i;
                }
                else if(botCards[i][0] == tableCards[tableCardsSize - 1][0] && botCards[i][0] == '9'){
                    checkWithSame[3] = i;
                }
                if(botCards[i][1] == JJ && botCards[i][0] == '1'){
                    checkWith[4] = i;
                }
                else if(botCards[i][0] == tableCards[tableCardsSize - 1][0] && botCards[i][0] == '1'){
                    checkWithSame[4] = i;
                }
                if(botCards[i][0] == 'J'){
                    checkWith[5] = i;
                }
                if(botCards[i][1] == JJ && botCards[i][0] == 'Q'){
                    checkWith[6] = i;
                }
                else if(botCards[i][0] == tableCards[tableCardsSize - 1][0] && botCards[i][0] == 'Q'){
                    checkWithSame[6] = i;
                }
                if(botCards[i][1] == JJ && botCards[i][0] == 'K'){
                    checkWith[7] = i;
                }
                else if(botCards[i][0] == tableCards[tableCardsSize - 1][0] && botCards[i][0] == 'K'){
                    checkWithSame[7] = i;
                }
                if(botCards[i][1] == JJ && botCards[i][0] == 'A'){
                    checkWith[8] = i;
                }
                else if(botCards[i][0] == tableCards[tableCardsSize - 1][0] && botCards[i][0] == 'A'){
                    checkWithSame[8] = i;
                }
            }
            if(checkWith[6] < botCardsSize && botCards[checkWith[6]][1] == 'p' && QSMode == true){
                finalmove[0] = checkWith[6];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWithSame[6] < botCardsSize && botCards[checkWithSame[6]][1] == 'p' && QSMode == true){
                finalmove[0] = checkWithSame[6];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWith[0] < botCardsSize && checkWith[8] < botCardsSize){
                finalmove[0] = checkWith[0];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWith[8] < botCardsSize){
                finalmove[0] = checkWith[8];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWithSame[8] < botCardsSize){
                finalmove[0] = checkWithSame[8];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWith[0] < botCardsSize && checkWith[2] < botCardsSize){
                finalmove[0] = checkWith[0];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWith[2] < botCardsSize){
                finalmove[0] = checkWith[2];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWithSame[2] < botCardsSize){
                finalmove[0] = checkWithSame[2];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWith[0] < botCardsSize && checkWith[7] < botCardsSize){
                finalmove[0] = checkWith[0];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWith[0] < botCardsSize && checkWith[6] < botCardsSize){
                finalmove[0] = checkWith[0];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWith[0] < botCardsSize && checkWith[4] < botCardsSize){
                finalmove[0] = checkWith[0];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWith[7] < botCardsSize){
                finalmove[0] = checkWith[7];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWithSame[7] < botCardsSize){
                finalmove[0] = checkWithSame[7];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWith[6] < botCardsSize){
                finalmove[0] = checkWith[6];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWithSame[6] < botCardsSize){
                finalmove[0] = checkWithSame[6];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWith[4] < botCardsSize){
                finalmove[0] = checkWith[4];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWithSame[4] < botCardsSize){
                finalmove[0] = checkWithSame[4];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWith[0] < botCardsSize && checkWith[1] < botCardsSize){
                finalmove[0] = checkWith[0];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWith[1] < botCardsSize){
                finalmove[0] = checkWith[1];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWithSame[1] < botCardsSize){
                finalmove[0] = checkWithSame[1];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWith[0] < botCardsSize && checkWith[3] < botCardsSize){
                finalmove[0] = checkWith[0];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWith[3] < botCardsSize){
                finalmove[0] = checkWith[3];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWithSame[3] < botCardsSize){
                finalmove[0] = checkWithSame[3];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWith[5] < botCardsSize){
                int sign[4];
                sign[0] = 0;
                sign[1] = 0;
                sign[2] = 0;
                sign[3] = 0;
                finalmove[0] = checkWith[5];
                finalmoveSize = 1;
                for(int i = 0; i < botCardsSize; i++){
                    if(botCards[i][0] != 'J'){
                        if(botCards[i][1] == 'c'){
                            sign[0]++;
                        }
                        else if(botCards[i][1] == 'k'){
                            sign[1]++;
                        }
                        else if(botCards[i][1] == 'b'){
                            sign[2]++;
                        }
                        else if(botCards[i][1] == 'p'){
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
                    Jackchoose = "c";
                    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/chirva.png);");
                    ui->label_3->show();
                }
                else if(k == 1){
                    Jackchoose = "k";
                    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/kresti.png);");
                    ui->label_3->show();
                }
                else if(k == 2){
                    Jackchoose = "b";
                    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/bybna.png);");
                    ui->label_3->show();
                }
                else if(k == 3){
                    Jackchoose = "p";
                    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/piki.png);");
                    ui->label_3->show();
                }
            }
            else if(checkWith[0] < botCardsSize){
                finalmove[0] = checkWith[0];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(checkWithSame[0] < botCardsSize){
                finalmove[0] = checkWithSame[0];
                finalmoveSize = 1;
                Jackchoose = "";
                ui->label_3->hide();
            }
            else if(BcheckForTake == 0){
                onRemoveWidgetColodBot();
                if(tableCards[tableCardsSize - 1][0] == '6'){
                    BcheckForTake = 0;
                }
            }
            else{
                secMove = 0;
                Mmove = 0;
                ui->label_2->setText("Your turn");
                ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
                if(isFullscreen && isInGame){
                    QString cs = ui->label_2->styleSheet();
                    ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
                }
                BcheckForTake = 0;
                checkForTake = 0;
            }
        }
        else if(secMove == 1 && botCardsSize > 0){
            for(int i = 0; i < botCardsSize; i++){
                if(botCards[i][0] == tableCards[tableCardsSize - 1][0]){
                    finalmove[0] = i;
                    finalmoveSize = 1;
                    Jackchoose = "";
                    ui->label_3->hide();
                    if(botCards[i][0] == 'J'){
                        int sign[4];
                        sign[0] = 0;
                        sign[1] = 0;
                        sign[2] = 0;
                        sign[3] = 0;
                        for(int i = 0; i < botCardsSize; i++){
                            if(botCards[i][1] == 'c'){
                                sign[0]++;
                            }
                            else if(botCards[i][1] == 'k'){
                                sign[1]++;
                            }
                            else if(botCards[i][1] == 'b'){
                                sign[2]++;
                            }
                            else if(botCards[i][1] == 'p'){
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
                            Jackchoose = "c";
                            ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/chirva.png);");
                            ui->label_3->show();
                        }
                        else if(k == 1){
                            Jackchoose = "k";
                            ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/kresti.png);");
                            ui->label_3->show();
                        }
                        else if(k == 2){
                            Jackchoose = "b";
                            ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/bybna.png);");
                            ui->label_3->show();
                        }
                        else if(k == 3){
                            Jackchoose = "p";
                            ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/piki.png);");
                            ui->label_3->show();
                        }
                    }

                }
            }
        }
        if(finalmoveSize > 0 && finalmove[0] < botCardsSize){
            soundplayer->stop();
            soundplayer->setSource(QUrl::fromLocalFile(tempFilePathCard));
            soundplayer->setVolume(soundvol / 100.0);
            soundplayer->play();
            if(botCards[finalmove[finalmoveSize - 1]][0] == 'J'){
                bJackKol++;
            }
            else{
                bJackKol = 0;
            }
            for(int i = 0; i < finalmoveSize; i++){
                tableCards[tableCardsSize] = botCards[finalmove[i]];
                onAddWidgetTable(tableCards[tableCardsSize], tableCardsSize);
                tableCardsSize++;
                delete botButtons[finalmove[i]];
                botCards[finalmove[i]] = "";
                for(int j = finalmove[i]; j < botCardsSize - 1; j++){
                    onAddWidgetBot(j);
                    delete botButtons[j + 1];
                    botCards[j] = botCards[j + 1];
                    botCards[j + 1] = "";
                }
                if(finalmoveSize > 1 && i < finalmoveSize - 1){
                    if(finalmove[i] < finalmove[i + 1]){
                        finalmove[i + 1]--;
                    }
                }
                botCardsSize--;
            }
            if(botCardsSize > 0 || tableCards[tableCardsSize - 1][0] == '6'){
                mv = Mmove;
                secondmove();
                operation(mv);
            }
            if(tableCards[tableCardsSize - 1][0] != '6'){
                gameEnd();
            }
        }
        AutoSave();
    }
    else{
        timer->start(2000);
    }
}

void MainWindow::onAddWidgetPlayer(std::string card, int iter)
{
    std::string img;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 9; j++){
            if(card == cardNames[i][j]){
                img = links[i][j];
            }
        }
    }
    QPixmap pix(QString::fromStdString(img));
    QIcon ButtonIcon(pix);
    QHBoxLayout* layout = qobject_cast<QHBoxLayout*>( ui->horizontalLayoutWidget->layout());
    playerButtons[iter] = new QPushButton("", ui->horizontalLayoutWidget);
    layout->insertWidget(0,playerButtons[iter]);
    playerButtons[iter]->setIcon(ButtonIcon);
    QSize s(70,101);
    if(isFullscreen)
        s = QSize(70*scaleFactor, 101*scaleFactor);

    playerButtons[iter]->setIconSize(s);
    playerButtons[iter]->setFixedSize(s);
    playerButtons[iter]->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "background: none;"
        "}"
        );
    QObject::connect(playerButtons[iter], &QPushButton::clicked, this, &MainWindow::onRemoveWidgetPlayer);
    if(isFullscreen)
        ui->scrollAreaWidgetContents->setMinimumSize(QSize((70*scaleFactor)*playerCardsSize,0));
    else
        ui->scrollAreaWidgetContents->setMinimumSize(QSize(70*playerCardsSize,0));
}

void MainWindow::onAddWidgetBot(int iter)
{
    QPixmap pix(":/img/PNG-cards-1.3/card_back.jfif");
    QIcon ButtonIcon(pix);
    QHBoxLayout* layout = qobject_cast<QHBoxLayout*>( ui->horizontalLayoutWidget_2->layout());
    botButtons[iter] = new QPushButton("", ui->horizontalLayoutWidget_2);
    layout->insertWidget(0,botButtons[iter]);
    botButtons[iter]->setIcon(ButtonIcon);
    QSize s(70,101);
    if(isFullscreen)
        s = QSize(70*scaleFactor, 101*scaleFactor);

    botButtons[iter]->setIconSize(s);
    botButtons[iter]->setFixedSize(s);
    botButtons[iter]->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "background: none;"
        "}"
        );
}

void MainWindow::onAddWidgetColod(int iter)
{
    QPixmap pix(":/img/PNG-cards-1.3/card_back.jfif");
    QIcon ButtonIcon(pix);
    QHBoxLayout* layout = qobject_cast<QHBoxLayout*>( ui->horizontalLayoutWidget_3->layout());
    ColodButtons[iter] = new QPushButton("", ui->horizontalLayoutWidget_3);
    layout->insertWidget(0,ColodButtons[iter]);
    ColodButtons[iter]->setIcon(ButtonIcon);
    QSize s(70,101);
    if(isFullscreen)
        s = QSize(70*scaleFactor, 101*scaleFactor);

    ColodButtons[iter]->setIconSize(s);
    ColodButtons[iter]->setFixedSize(s);
    ColodButtons[iter]->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "background: none;"
        "}"
        );
    QObject::connect(ColodButtons[iter], &QPushButton::clicked, this, &MainWindow::onRemoveWidgetColod);
}

void MainWindow::onAddWidgetTable(std::string card, int iter)
{
    std::string img;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 9; j++){
            if(card == cardNames[i][j]){
                img = links[i][j];
            }
        }
    }
    QPixmap pix(QString::fromStdString(img));
    QIcon ButtonIcon(pix);
    QHBoxLayout* layout = qobject_cast<QHBoxLayout*>( ui->horizontalLayoutWidget_4->layout());
    TableButtons[iter] = new QPushButton("", ui->horizontalLayoutWidget_4);
    layout->insertWidget(0,TableButtons[iter]);
    TableButtons[iter]->setIcon(ButtonIcon);
    QSize s(70,101);
    if(isFullscreen)
        s = QSize(70*scaleFactor, 101*scaleFactor);

    TableButtons[iter]->setIconSize(s);
    TableButtons[iter]->setFixedSize(s);
    TableButtons[iter]->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "background: none;"
        "}"
        );
}

void MainWindow::onRemoveWidgetPlayer()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    bool maincheck = false;
    bool mv;
    for(int i = 0; i < playerCardsSize; i++){
        bool check;
        if(Jackchoose == "" && Mmove == 0){
            check = possibleMove(playerCards[i], tableCards[tableCardsSize - 1], secMove);
        }
        else if(Mmove == 0){
            check = possibleMove(playerCards[i], "0" + Jackchoose, 0);
        }
        if(button == playerButtons[i] && Mmove == 0 && check == true){
            soundplayer->stop();
            soundplayer->setSource(QUrl::fromLocalFile(tempFilePathCard));
            soundplayer->setVolume(soundvol / 100.0);
            soundplayer->play();
            Jackchoose = "";
            maincheck = true;
            ui->pushButton->hide();
            ui->pushButton_2->hide();
            ui->pushButton_3->hide();
            ui->pushButton_4->hide();
            ui->pushButton_5->hide();
            ui->label_3->hide();
            if(playerCards[i][0] == 'J'){
                pJackKol++;
            }
            else{
                pJackKol = 0;
            }
            tableCards[tableCardsSize] = playerCards[i];
            onAddWidgetTable(tableCards[tableCardsSize], tableCardsSize);
            tableCardsSize++;
            delete playerButtons[i];
            playerCards[i] = "";
            mv = Mmove;
            for(int j = i; j < playerCardsSize - 1; j++){
                onAddWidgetPlayer(playerCards[j + 1], j);
                delete playerButtons[j + 1];
                playerCards[j] = playerCards[j + 1];
                playerCards[j + 1] = "";
            }
            i = playerCardsSize;
            playerCardsSize--;
        }
    }
    if(maincheck == true){
        ui->label_3->hide();
        checkForTake = 0;
        BcheckForTake = 0;
        if(playerCardsSize > 0){
            secondmove();
            operation(mv);
        }
        if(tableCards[tableCardsSize - 1][0] != '6'){
            gameEnd();
        }
    }

    if(isFullscreen)
        ui->scrollAreaWidgetContents->setMinimumSize(QSize((70*scaleFactor)*playerCardsSize,0));
    else
        ui->scrollAreaWidgetContents->setMinimumSize(QSize(70*playerCardsSize,0));

    AutoSave();
}

void MainWindow::onRemoveWidgetColod()
{
    if(Mmove == 0 && checkForTake == 0){
        if(tableCards[tableCardsSize - 1][0] == '6' || secMove == 0){
            if(ColodCardsSize == 0){
                shuffling();
            }

            soundplayer->stop();
            soundplayer->setSource(QUrl::fromLocalFile(tempFilePathCard));
            soundplayer->setVolume(soundvol / 100.0);
            soundplayer->play();

            playerCards[playerCardsSize] = ColodCards[ColodCardsSize - 1];
            onAddWidgetPlayer(playerCards[playerCardsSize], playerCardsSize);
            playerCardsSize++;
            ColodCards[ColodCardsSize - 1] = "";
            delete ColodButtons[ColodCardsSize - 1];
            ColodCardsSize--;
            ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));
            if(tableCards[tableCardsSize - 1][0] != '6'){
                ui->pushButton->show();
                checkForTake = 1;
            }
            if(ColodCardsSize == 0){
                onAddWidgetColod(0);
                isShuffl = true;
                ui->label_4->setText(QString::fromStdString("^ \nShuffling (" + std::to_string(PointsX + 1) + "x)"));
                ui->label_4->show();
            }
        }

        if(isFullscreen)
            ui->scrollAreaWidgetContents->setMinimumSize(QSize((70*scaleFactor)*playerCardsSize,0));
        else
            ui->scrollAreaWidgetContents->setMinimumSize(QSize(70*playerCardsSize,0));

        AutoSave();
    }
}

void MainWindow::onRemoveWidgetColodBot(){
    if(Mmove == 1 && BcheckForTake == 0){
        if(tableCards[tableCardsSize - 1][0] == '6' || secMove == 0){
            if(ColodCardsSize == 0){
                shuffling();
            }

            soundplayer->stop();
            soundplayer->setSource(QUrl::fromLocalFile(tempFilePathCard));
            soundplayer->setVolume(soundvol / 100.0);
            soundplayer->play();

            botCards[botCardsSize] = ColodCards[ColodCardsSize - 1];
            onAddWidgetBot(botCardsSize);
            botCardsSize++;
            ColodCards[ColodCardsSize - 1] = "";
            delete ColodButtons[ColodCardsSize - 1];
            ColodCardsSize--;
            ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));
            if(tableCards[tableCardsSize - 1][0] != '6'){
                BcheckForTake = 1;
            }
            if(ColodCardsSize == 0){
                onAddWidgetColod(0);
                isShuffl = true;
                ui->label_4->setText(QString::fromStdString("^ \nShuffling (" + std::to_string(PointsX + 1) + "x)"));
                ui->label_4->show();
            }
        }
    }
    AutoSave();
}

void MainWindow::shuffling()
{
    isShuffl = false;
    soundplayer->stop();
    soundplayer->setSource(QUrl::fromLocalFile(tempFilePathCard));
    soundplayer->setVolume(soundvol / 100.0);
    soundplayer->play();

    delete ColodButtons[0];

    for(int i = 0; i < tableCardsSize - 1; i++){
        int ran = rand() % (tableCardsSize - 1);
        if(tableCards[ran] == ""){
            i--;
        }
        else{
            ColodCards[ColodCardsSize] = tableCards[ran];
            onAddWidgetColod(ColodCardsSize);
            ColodCardsSize++;
            tableCards[ran] = "";
        }
    }
    tableCards[0] = tableCards[tableCardsSize - 1];
    tableCards[tableCardsSize - 1] = "";
    for(int i = 0; i < tableCardsSize; i++){
        delete TableButtons[i];
    }
    onAddWidgetTable(tableCards[0], 0);
    tableCardsSize = 1;
    PointsX++;
    ui->label_7->setText(QString::fromStdString("Points " + std::to_string(PointsX) + "x"));
    ui->label_4->hide();
}

void MainWindow::gameEnd()
{
    if(playerCardsSize == 0 || botCardsSize == 0){
        if(playerCardsSize == 0){
            if(pJackKol != 0){
                playerPoints -= ((20 * pJackKol) * PointsX);
            }
            for(int i = 0; i < botCardsSize; i++){
                if(botCards[i][0] == 'A'){
                    botPoints += (15 * PointsX);
                }
                else if(botCards[i][0] == 'J'){
                    botPoints += (20 * PointsX);
                }
                else if(QSMode && botCards[i] == "Qp"){
                    botPoints += (50 * PointsX);
                }
                else if(botCards[i][0] == 'K' || botCards[i][0] == 'Q' || botCards[i][0] == '1'){
                    botPoints += (10 * PointsX);
                }
            }
        }
        else if(botCardsSize == 0){
            if(bJackKol != 0){
                botPoints -= ((20 * bJackKol) * PointsX);
            }
            for(int i = 0; i < playerCardsSize; i++){
                if(playerCards[i][0] == 'A'){
                    playerPoints += (15 * PointsX);
                }
                else if(playerCards[i][0] == 'J'){
                    playerPoints += (20 * PointsX);
                }
                else if(QSMode && playerCards[i] == "Qp"){
                    playerPoints += (50 * PointsX);
                }
                else if(playerCards[i][0] == 'K' || playerCards[i][0] == 'Q' || playerCards[i][0] == '1'){
                    playerPoints += (10 * PointsX);
                }
            }
        }

        if(botPoints == 225 && PointsMode)
            botPoints = 0;
        else if(botPoints == 125)
            botPoints = 0;

        if(playerPoints == 225 && PointsMode)
            playerPoints = 0;
        else if(playerPoints == 125)
            playerPoints = 0;

        for(int i = 0; i < botCardsSize; i++){
            delete botButtons[i];
            botCards[i] = "";
        }
        for(int i = 0; i < playerCardsSize; i++){
            delete playerButtons[i];
            playerCards[i] = "";
        }
        if(ColodCardsSize == 0){
            delete ColodButtons[0];
        }
        for(int i = 0; i < ColodCardsSize; i++){
            delete ColodButtons[i];
            ColodCards[i] = "";
        }
        for(int i = 0; i < tableCardsSize; i++){
            delete TableButtons[i];
            tableCards[i] = "";
        }
        botCardsSize = 0;
        playerCardsSize = 0;
        ColodCardsSize = 0;
        tableCardsSize = 0;

        checkForTake = 0;
        BcheckForTake = 0;

        PointsX = 1;
        ui->label->setVisible(false);
        ui->label_2->setVisible(false);

        ui->pushButton->hide();
        ui->pushButton_2->hide();
        ui->pushButton_3->hide();
        ui->pushButton_4->hide();
        ui->pushButton_5->hide();
        ui->pushButton_10->hide();
        ui->pushButton_11->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->label_5->hide();
        ui->label_6->hide();
        ui->label_7->hide();
        ui->label_8->hide();
        ui->label_11->hide();
        ui->label_12->hide();
        ui->label_13->hide();
        ui->scrollArea->hide();
        ui->label_7->setText(QString::fromStdString("Points " + std::to_string(PointsX) + "x"));

        pJackKol = 0;
        bJackKol = 0;

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Quit");
        msgBox.setText("Are you sure?");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setStyleSheet(massBoxStyle);

        if((playerPoints > 125 && !PointsMode) || (playerPoints > 225 && PointsMode)){
            isInGame = false;

            delete timer;
            msgBox.setWindowTitle("Game over");
            msgBox.setText(QString::fromStdString("Unfortunately. You lost!\nComputer points: " + std::to_string(botPoints) + "\nYour points: " + std::to_string(playerPoints)));
            msgBox.exec();
            soundplayer->stop();
            soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
            soundplayer->setVolume(soundvol / 100.0);
            soundplayer->play();
            botPoints = 0;
            playerPoints = 0;
            ui->pushButton_6->show();
            ui->pushButton_7->show();
            ui->pushButton_8->show();
            ui->label_11->hide();
            ui->label_12->hide();
            ui->label_13->hide();
            ui->scrollArea->hide();
            ui->label_9->show();
            ui->label_10->show();
            ui->line->show();
            ui->line_2->show();
            std::ofstream fout;
            fout.open(dirGame.toStdString());
            fout.close();
            ui->pushButton_6->setGeometry(338, 160, 251, 71);

            player->stop();
            player->setSource(QUrl::fromLocalFile(tempFilePathMenu));
            audioOutput->setVolume((musicvol / 100.0) * 0.1);
            player->setLoops(-1);
            player->play();

            Resizing();
        }
        else if((botPoints > 125 && !PointsMode) || (botPoints > 225 && PointsMode)){
            isInGame = false;

            delete timer;
            msgBox.setWindowTitle("Game over");
            msgBox.setText(QString::fromStdString("Congratulations! You won!!\nComputer points: " + std::to_string(botPoints) + "\nYour points: " + std::to_string(playerPoints)));
            msgBox.exec();
            soundplayer->stop();
            soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
            soundplayer->setVolume(soundvol / 100.0);
            soundplayer->play();
            botPoints = 0;
            playerPoints = 0;
            ui->pushButton_6->show();
            ui->pushButton_7->show();
            ui->pushButton_8->show();
            ui->label_11->hide();
            ui->label_12->hide();
            ui->label_13->hide();
            ui->scrollArea->hide();
            ui->label_9->show();
            ui->label_10->show();
            ui->line->show();
            ui->line_2->show();
            std::ofstream fout;
            fout.open(dirGame.toStdString());
            fout.close();
            ui->pushButton_6->setGeometry(338, 160, 251, 71);

            player->stop();
            player->setSource(QUrl::fromLocalFile(tempFilePathMenu));
            audioOutput->setVolume((musicvol / 100.0) * 0.1);
            player->setLoops(-1);
            player->play();

            Resizing();
        }
        else{
            delete timer;
            msgBox.setWindowTitle("Set");
            msgBox.setText(QString::fromStdString("Set!!!\nComputer points: " + std::to_string(botPoints) + "\nYour points: " + std::to_string(playerPoints)));
            msgBox.exec();
            ui->label_5->setText(QString::fromStdString("Points: " + std::to_string(playerPoints)));
            ui->label_6->setText(QString::fromStdString("Points: " + std::to_string(botPoints)));
            ui->label->setVisible(true);
            ui->label_2->setVisible(true);
            ui->label_5->setVisible(true);
            ui->label_6->setVisible(true);
            ui->label_7->setVisible(true);
            ui->label_8->setVisible(true);
            ui->pushButton_10->setVisible(true);
            ui->pushButton_11->setVisible(true);

            if(QSMode)
                ui->label_11->setVisible(true);

            ui->label_12->show();
            ui->label_13->show();
            ui->scrollArea->show();

            Set++;
            ui->label_8->setText(QString::fromStdString("Set: " + std::to_string(Set)));

            Start();
            bool mv = Mmove;
            if(Mmove == 1 && tableCards[0][0] == 'J'){
                int sign[4];
                sign[0] = 0;
                sign[1] = 0;
                sign[2] = 0;
                sign[3] = 0;
                for(int i = 0; i < botCardsSize; i++){
                    if(botCards[i][0] != 'J'){
                        if(botCards[i][1] == 'c'){
                            sign[0]++;
                        }
                        else if(botCards[i][1] == 'k'){
                            sign[1]++;
                        }
                        else if(botCards[i][1] == 'b'){
                            sign[2]++;
                        }
                        else if(botCards[i][1] == 'p'){
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
                    Jackchoose = "c";
                    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/chirva.png);");
                    ui->label_3->show();
                }
                else if(k == 1){
                    Jackchoose = "k";
                    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/kresti.png);");
                    ui->label_3->show();
                }
                else if(k == 2){
                    Jackchoose = "b";
                    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/bybna.png);");
                    ui->label_3->show();
                }
                else if(k == 3){
                    Jackchoose = "p";
                    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/piki.png);");
                    ui->label_3->show();
                }
            }
            secondmove();
            operation(mv);

            timer = new QTimer();
            if(difficulty == "Middle"){
                connect(timer, SIGNAL(timeout()), this, SLOT(botMove()));
            }
            else if(difficulty == "Hard"){
                connect(timer, SIGNAL(timeout()), this, SLOT(chooseBestMove()));
            }
            timer->start(2000);
        }
    }
}

bool MainWindow::possibleMove(std::string p, std::string t, bool ndmove)
{
    bool check = false;
    if(ndmove == 0){
        if(p[0] == 'J'){
            check = true;
        }
        else if(p[0] == t[0]){
            check = true;
        }
        else{
            if(p[1] == t[1]){
                check = true;
            }
            else{
                check = false;
            }
        }
    }
    else{
        if(t[0] == '6'){
            if(p[1] == t[1] || p[0] == '6' || p[0] == 'J'){
                check = true;
            }
            else{
                check = false;
            }
        }
        else if(p[0] == t[0]){
            check = true;
        }
        else{
            check = false;
        }
    }
    return check;
}

void MainWindow::operation(bool mv)
{
    if(mv == 0){
        if(tableCards[tableCardsSize - 1] == "Qp" && QSMode == true){
            secMove = 0;
            Mmove = 0;
            ui->label_2->setText("Your turn");
            ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
            if(isFullscreen && isInGame){
                QString cs = ui->label_2->styleSheet();
                ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
            }
            ui->pushButton->hide();
            for(int i = 0; i < 5; i++){
                if(ColodCardsSize == 0){
                    if(i != 0){
                        onAddWidgetColod(0);
                    }
                    shuffling();
                }
                botCards[botCardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetBot(botCardsSize);
                botCardsSize++;
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodCardsSize--;
            }
            if(ColodCardsSize == 0){
                onAddWidgetColod(0);
                isShuffl = true;
                ui->label_4->setText(QString::fromStdString("^ \nShuffling (" + std::to_string(PointsX + 1) + "x)"));
                ui->label_4->show();
            }
            ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));
        }
        else {
            char card = tableCards[tableCardsSize - 1][0];

            switch (card) {
            case 'A':
                secMove = 0;
                Mmove = 0;
                ui->label_2->setText("Your turn");
                ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
                if(isFullscreen && isInGame){
                    QString cs = ui->label_2->styleSheet();
                    ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
                }
                ui->pushButton->hide();
                break;

            case '8':
                if (ColodCardsSize == 0) {
                    shuffling();
                }

                secMove = 0;
                Mmove = 0;
                ui->label_2->setText("Your turn");
                ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
                if(isFullscreen && isInGame){
                    QString cs = ui->label_2->styleSheet();
                    ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
                }
                ui->pushButton->hide();

                botCards[botCardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetBot(botCardsSize);
                botCardsSize++;
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodCardsSize--;

                if (ColodCardsSize == 0) {
                    onAddWidgetColod(0);
                    isShuffl = true;
                    ui->label_4->setText(QString::fromStdString("^ \nShuffling (" + std::to_string(PointsX + 1) + "x)"));
                    ui->label_4->show();
                }

                if (ColodCardsSize == 0) {
                    shuffling();
                }

                botCards[botCardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetBot(botCardsSize);
                botCardsSize++;
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodCardsSize--;

                ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));

                if (ColodCardsSize == 0) {
                    onAddWidgetColod(0);
                    isShuffl = true;
                    ui->label_4->setText(QString::fromStdString("^ \nShuffling (" + std::to_string(PointsX + 1) + "x)"));
                    ui->label_4->show();
                }
                break;

            case '7':
                if (ColodCardsSize == 0) {
                    shuffling();
                }

                botCards[botCardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetBot(botCardsSize);
                botCardsSize++;
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodCardsSize--;

                ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));

                if (ColodCardsSize == 0) {
                    onAddWidgetColod(0);
                    isShuffl = true;
                    ui->label_4->setText(QString::fromStdString("^ \nShuffling (" + std::to_string(PointsX + 1) + "x)"));
                    ui->label_4->show();
                }
                break;
            }
        }
    }
    else{
        if(tableCards[tableCardsSize - 1] == "Qp" && QSMode == true){
            secMove = 0;
            Mmove = 1;
            ui->label_2->setText("Enemy turn");
            ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
            if(isFullscreen && isInGame){
                QString cs = ui->label_2->styleSheet();
                ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
            }
            BcheckForTake = 0;
            for(int i = 0; i < 5; i++){
                if(ColodCardsSize == 0){
                    if(i != 0){
                        onAddWidgetColod(0);
                    }
                    shuffling();
                }
                playerCards[playerCardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetPlayer(playerCards[playerCardsSize], playerCardsSize);
                playerCardsSize++;
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodCardsSize--;
            }
            if(ColodCardsSize == 0){
                onAddWidgetColod(0);
                isShuffl = true;
                ui->label_4->setText(QString::fromStdString("^ \nShuffling (" + std::to_string(PointsX + 1) + "x)"));
                ui->label_4->show();
            }
            ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));
        }
        else {
            char card = tableCards[tableCardsSize - 1][0];

            switch (card) {
            case 'A':
                secMove = 0;
                Mmove = 1;
                ui->label_2->setText("Enemy turn");
                ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
                if(isFullscreen && isInGame){
                    QString cs = ui->label_2->styleSheet();
                    ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
                }
                BcheckForTake = 0;
                break;

            case '8':
                if (ColodCardsSize == 0) {
                    shuffling();
                }

                BcheckForTake = 0;
                secMove = 0;
                Mmove = 1;
                ui->label_2->setText("Enemy turn");
                ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
                if(isFullscreen && isInGame){
                    QString cs = ui->label_2->styleSheet();
                    ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
                }

                playerCards[playerCardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetPlayer(playerCards[playerCardsSize], playerCardsSize);
                playerCardsSize++;
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodCardsSize--;

                if (ColodCardsSize == 0) {
                    onAddWidgetColod(0);
                    isShuffl = true;
                    ui->label_4->setText(QString::fromStdString("^ \nShuffling (" + std::to_string(PointsX + 1) + "x)"));
                    ui->label_4->show();
                }

                if (ColodCardsSize == 0) {
                    shuffling();
                }

                playerCards[playerCardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetPlayer(playerCards[playerCardsSize], playerCardsSize);
                playerCardsSize++;
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodCardsSize--;

                ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));

                if (ColodCardsSize == 0) {
                    onAddWidgetColod(0);
                    isShuffl = true;
                    ui->label_4->setText(QString::fromStdString("^ \nShuffling (" + std::to_string(PointsX + 1) + "x)"));
                    ui->label_4->show();
                }
                break;

            case '7':
                if (ColodCardsSize == 0) {
                    shuffling();
                }

                playerCards[playerCardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetPlayer(playerCards[playerCardsSize], playerCardsSize);
                playerCardsSize++;
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodCardsSize--;

                std::string kol = std::to_string(ColodCardsSize);
                ui->label->setText(QString::fromStdString(kol));

                if (ColodCardsSize == 0) {
                    onAddWidgetColod(0);
                    isShuffl = true;
                    ui->label_4->setText(QString::fromStdString("^ \nShuffling (" + std::to_string(PointsX + 1) + "x)"));
                    ui->label_4->show();
                }
                break;
            }
        }
        if(isFullscreen)
            ui->scrollAreaWidgetContents->setMinimumSize(QSize((70*scaleFactor)*playerCardsSize,0));
        else
            ui->scrollAreaWidgetContents->setMinimumSize(QSize(70*playerCardsSize,0));
    }
}

void MainWindow::secondmove()
{
    if(Mmove == 0){
        secMove = 0;
        Mmove = 1;
        ui->label_2->setText("Enemy turn");
        ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
        if(isFullscreen && isInGame){
            QString cs = ui->label_2->styleSheet();
            ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
        }
        for(int i = 0; i < playerCardsSize; i++){
            if(playerCards[i][0] == tableCards[tableCardsSize - 1][0] || tableCards[tableCardsSize - 1][0] == '6' || tableCards[tableCardsSize - 1][0] == 'J'){
                secMove = 1;
                Mmove = 0;
                ui->label_2->setText("Your turn");
                ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
                if(isFullscreen && isInGame){
                    QString cs = ui->label_2->styleSheet();
                    ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
                }
                if(tableCards[tableCardsSize - 1][0] != '6' && tableCards[tableCardsSize - 1][0] != 'J'){
                    ui->pushButton->show();
                }
                else if(tableCards[tableCardsSize - 1][0] == 'J'){
                    ui->pushButton_2->show();
                    ui->pushButton_3->show();
                    ui->pushButton_4->show();
                    ui->pushButton_5->show();
                }
            }
        }
    }
    else{
        secMove = 0;
        Mmove = 0;
        ui->label_2->setText("Your turn");
        ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
        if(isFullscreen && isInGame){
            QString cs = ui->label_2->styleSheet();
            ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
        }
        if(tableCards[tableCardsSize - 1][0] == '6'){
            Mmove = 1;
            ui->label_2->setText("Enemy turn");
            ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
            if(isFullscreen && isInGame){
                QString cs = ui->label_2->styleSheet();
                ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
            }
            BcheckForTake = 0;
        }
        else{
            for(int i = 0; i < botCardsSize; i++){
                if(botCards[i][0] == tableCards[tableCardsSize - 1][0]){
                    secMove = 1;
                    Mmove = 1;
                    ui->label_2->setText("Enemy turn");
                    ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
                    if(isFullscreen && isInGame){
                        QString cs = ui->label_2->styleSheet();
                        ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
                    }
                }
            }
        }
    }
}

void MainWindow::Start()
{

    srand(time(NULL));
    Mmove = rand() % 2;

    int it;
    int jt;
    bool check = false;
    for(int i = 0; i < 5; i++){
        while(check == false){
            bool check1 = true;
            it = rand() % 4;
            jt = rand() % 9;
            for(int j = 0; j < playerCardsSize; j++){
                if(cardNames[it][jt] == playerCards[j] || cardNames[it][jt] == botCards[j]){
                    check1 = false;
                }
            }
            if(check1 == true){
                check = true;
            }
        }
        if(i == 4 && Mmove == 0){
            tableCards[0] = cardNames[it][jt];
            tableCardsSize++;

            onAddWidgetTable(tableCards[0], 0);
        }
        else{
            playerCards[i] = cardNames[it][jt];
            playerCardsSize++;

            onAddWidgetPlayer(playerCards[i], i);
        }
        check = false;

        while(check == false){
            bool check1 = true;
            it = rand() % 4;
            jt = rand() % 9;
            for(int j = 0; j < playerCardsSize; j++){
                if(cardNames[it][jt] == playerCards[j] || cardNames[it][jt] == botCards[j]){
                    check1 = false;
                }
            }
            if(i == 4 && Mmove == 0 && tableCards[0] == cardNames[it][jt]){
                check1 = false;
            }
            if(check1 == true){
                check = true;
            }
        }
        if(i == 4 && Mmove == 1){
            tableCards[0] = cardNames[it][jt];
            tableCardsSize++;

            onAddWidgetTable(tableCards[0], 0);
        }
        else{
            botCards[i] = cardNames[it][jt];
            botCardsSize++;

            onAddWidgetBot(i);
        }
        check = false;
    }

    for(int i = 0; i < 26; i++){
        while(check == false){
            bool check1 = true;
            it = rand() % 4;
            jt = rand() % 9;
            if(5 > ColodCardsSize){
                for(int j = 0; j < 5; j++){
                    if(cardNames[it][jt] == playerCards[j] || cardNames[it][jt] == botCards[j] || cardNames[it][jt] == ColodCards[j]){
                        check1 = false;
                    }
                }
            }
            else{
                for(int j = 0; j < ColodCardsSize; j++){
                    if(cardNames[it][jt] == playerCards[j] || cardNames[it][jt] == botCards[j] || cardNames[it][jt] == ColodCards[j]){
                        check1 = false;
                    }
                }
            }
            if(tableCards[0] == cardNames[it][jt]){
                check1 = false;
            }
            if(check1 == true){
                check = true;
            }
        }
        ColodCards[i] = cardNames[it][jt];
        ColodCardsSize++;
        ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));

        onAddWidgetColod(i);

        check = false;
    }
}

void MainWindow::AutoSave()
{
    if(playerCardsSize > 0 && botCardsSize > 0){
        std::ofstream fout;
        fout.open(dirGame.toStdString());

        if(fout.is_open()){

            fout << "Can_Resume\n";

            fout << playerCardsSize << "\n";
            for(int i = 0; i < playerCardsSize; i++){
                fout << playerCards[i] << "\n";
            }

            fout << botCardsSize << "\n";
            for(int i = 0; i < botCardsSize; i++){
                fout << botCards[i] << "\n";
            }

            fout << ColodCardsSize << "\n";
            for(int i = 0; i < ColodCardsSize; i++){
                fout << ColodCards[i] << "\n";
            }

            fout << tableCardsSize << "\n";
            for(int i = 0; i < tableCardsSize; i++){
                fout << tableCards[i] << "\n";
            }

            fout << Mmove << "\n";
            fout << secMove << "\n";
            if(Jackchoose == "")
                fout << "empty" << "\n";
            else
                fout << Jackchoose << "\n";
            fout << checkForTake << "\n";
            fout << BcheckForTake << "\n";
            fout << playerPoints << "\n";
            fout << botPoints << "\n";
            fout << PointsX << "\n";
            fout << pJackKol << "\n";
            fout << bJackKol << "\n";
            fout << Set << "\n";
            fout << ui->pushButton->isVisible() << "\n";
            fout << ui->pushButton_2->isVisible() << "\n";
            fout << ui->label_3->isVisible() << "\n";
            fout << QSMode << "\n";
            fout << PointsMode << "\n";
            fout << difficulty;
        }
        fout.close();
    }
}

void MainWindow::OptionsSave()
{
    std::ofstream fout;
    fout.open(dirOption.toStdString());

    if(fout.is_open()){
        fout << QSMode << "\n";
        fout << PointsMode << "\n";
        fout << difficulty << "\n";
        fout << musicvol << "\n";
        fout << soundvol << "\n";
        fout << isFullscreen;
    }
    fout.close();
}

void MainWindow::Resizing(QString label2Str)
{
    if(isInGame){
        if(isFullscreen){
            showFullScreen();
            QSize windowSize = this->size();
            ui->horizontalLayoutWidget_2->resize(ui->scrollArea->width(), ui->scrollArea->height());
            scaleFactor = (windowSize.height() / 420) * 0.8;
            scaleWidget(ui->label);
            scaleFontnRadiusWidget(ui->label, 8, 3);
            scaleWidget(ui->label_2);
            scaleFontnRadiusWidget(ui->label_2, 8, 3);
            scaleWidget(ui->label_3);
            scaleWidget(ui->label_4);
            scaleFontnRadiusWidget(ui->label_4, 8, 3);
            scaleWidget(ui->label_5);
            scaleFontnRadiusWidget(ui->label_5, 8, 0);
            scaleWidget(ui->label_6);
            scaleFontnRadiusWidget(ui->label_6, 8, 0);
            scaleWidget(ui->label_7);
            scaleFontnRadiusWidget(ui->label_7, 8, 3);
            scaleWidget(ui->label_8);
            scaleFontnRadiusWidget(ui->label_8, 8, 3);
            scaleWidget(ui->label_11);
            scaleWidget(ui->label_12);
            ui->label_12->setStyleSheet(QString("font-weight: %1; font-family: 'Segoe UI Black'; font-size: %2pt; color: #c5c5c5").arg(900 * scaleFactor).arg(11 * scaleFactor));
            scaleWidget(ui->label_13);
            if(difficulty == "Middle"){
                ui->label_13->setStyleSheet(QString("font-weight: %1; font-size: %2pt; font-family: 'Segoe UI Black'; color: orange").arg(900 * scaleFactor).arg(11 * scaleFactor));
            }
            else if(difficulty == "Hard"){
                ui->label_13->setStyleSheet(QString("font-weight: %1; font-size: %2pt; font-family: 'Segoe UI Black'; color: red").arg(900 * scaleFactor).arg(11 * scaleFactor));
            }
            scaleWidget(ui->pushButton);
            scaleFontnRadiusWidget(ui->pushButton, 8, 3);
            scaleWidget(ui->pushButton_2);
            scaleWidget(ui->pushButton_3);
            scaleWidget(ui->pushButton_4);
            scaleWidget(ui->pushButton_5);
            scaleWidget(ui->pushButton_10);
            scaleFontnRadiusWidget(ui->pushButton_10, 0, 10);
            scaleWidget(ui->pushButton_11);
            scaleFontnRadiusWidget(ui->pushButton_11, 0, 10);
            QSize picSize = ui->pushButton_10->iconSize();
            ui->pushButton_10->setIconSize(QSize(picSize.width()*scaleFactor, picSize.height()*scaleFactor));
            picSize = ui->pushButton_11->iconSize();
            ui->pushButton_11->setIconSize(QSize(picSize.width()*scaleFactor, picSize.height()*scaleFactor));
            scaleWidget(ui->horizontalLayoutWidget_2);
            scaleWidget(ui->horizontalLayoutWidget_3);
            scaleWidget(ui->horizontalLayoutWidget_4);
            scaleWidget(ui->scrollArea);
            scaleWidget(ui->horizontalLayoutWidget);
            QSize s(70*scaleFactor,101*scaleFactor);
            for(int i = 0; i < botCardsSize; i++){
                botButtons[i]->setIconSize(s);
                botButtons[i]->setFixedSize(s);
            }
            for(int i = 0; i < ColodCardsSize; i++){
                ColodButtons[i]->setIconSize(s);
                ColodButtons[i]->setFixedSize(s);
            }
            if(isShuffl){
                ColodButtons[0]->setIconSize(s);
                ColodButtons[0]->setFixedSize(s);
            }
            for(int i = 0; i < tableCardsSize; i++){
              TableButtons[i]->setIconSize(s);
              TableButtons[i]->setFixedSize(s);
            }
            for(int i = 0; i < playerCardsSize; i++){
                playerButtons[i]->setIconSize(s);
                playerButtons[i]->setFixedSize(s);
            }
            ui->centralwidget->resize(windowSize);
            WidgetsLocation(windowSize);
        }
        else{
            setUiGeo();
            if(Mmove == 1)
                ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
            else
                ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");

            if(label2Str != "")
                ui->label_2->setText(label2Str);

            ui->scrollAreaWidgetContents->setMinimumSize(QSize(70*playerCardsSize,0));
        }
    }
    else if(!isFromSet){
        setUiGeo();
    }
    if(window->isVisible()){
        window->raise();
    }

    isFromSet = false;
}

void MainWindow::WidgetsLocation(QSize windowSize)
{
    QPoint curWidgetPos = ui->label_2->pos();
    int timePos = (windowSize.height() / 2) - (ui->label_2->height() / 2);
    ui->label_2->move(curWidgetPos.x(), timePos);

    curWidgetPos = ui->pushButton->pos();
    ui->pushButton->move(curWidgetPos.x(), timePos + ui->pushButton->height() + (9 * scaleFactor));

    curWidgetPos = ui->label_8->pos();
    timePos = timePos - 4 - ui->label_8->height();
    ui->label_8->move(curWidgetPos.x(), timePos);

    curWidgetPos = ui->label_13->pos();
    timePos = timePos - 5 - ui->label_13->height();
    ui->label_13->move(curWidgetPos.x(), timePos);

    curWidgetPos = ui->label_12->pos();
    ui->label_12->move(ui->label_13->pos().x() + ui->label_13->width() + (4 * scaleFactor), timePos);

    curWidgetPos = ui->label_11->pos();
    ui->label_11->move(ui->label_12->pos().x() + ui->label_12->width() + (3 * scaleFactor), timePos);

    ui->pushButton_10->move((windowSize.width() - 20) - ui->pushButton_10->width(), 20);
    ui->pushButton_11->move((windowSize.width() - 20) - ui->pushButton_11->width(), 20 + ui->pushButton_10->height() + (10 * scaleFactor));

    ui->scrollArea->move((windowSize.width() / 2) - (ui->scrollArea->width() / 2), windowSize.height() - 20 - ui->scrollArea->height());
    ui->scrollAreaWidgetContents->setMinimumSize(QSize((70*scaleFactor)*playerCardsSize,0));

    ui->horizontalLayoutWidget_2->move((windowSize.width() / 2) - (ui->horizontalLayoutWidget_2->width() / 2), 20);

    ui->label_5->move(ui->label_5->pos().x(), ui->scrollArea->pos().y() - 3 - ui->label_5->height());
    ui->label_6->move(ui->label_6->pos().x(), ui->horizontalLayoutWidget_2->pos().y() + 3 + ui->horizontalLayoutWidget_2->height());

    ui->horizontalLayoutWidget_3->move((windowSize.width() / 3) * 2, (windowSize.height() / 2) - (ui->horizontalLayoutWidget_3->height() / 2));
    ui->label_7->move(ui->horizontalLayoutWidget_3->pos().x(), ui->horizontalLayoutWidget_3->pos().y() - ui->label_7->height() - (9 * scaleFactor));
    ui->label->move(ui->horizontalLayoutWidget_3->pos().x() + ui->horizontalLayoutWidget_3->width() + (9 * scaleFactor), (windowSize.height() / 2) - (ui->label->height() / 2));
    ui->label_4->move(ui->label_7->pos().x(), ui->horizontalLayoutWidget_3->pos().y() + ui->horizontalLayoutWidget_3->height() + (2 * scaleFactor));
    ui->horizontalLayoutWidget_4->move((windowSize.width() / 3), (windowSize.height() / 2) - (ui->horizontalLayoutWidget_4->height() / 2));
    ui->pushButton_2->move(ui->horizontalLayoutWidget_4->pos().x() + (ui->horizontalLayoutWidget_4->width() / 2) - (ui->pushButton_2->width() * 2), ui->horizontalLayoutWidget_4->pos().y() + ui->horizontalLayoutWidget_4->height() + (9 * scaleFactor));
    ui->pushButton_3->move(ui->pushButton_2->pos().x() + ui->pushButton_2->width(), ui->horizontalLayoutWidget_4->pos().y() + ui->horizontalLayoutWidget_4->height() + (9 * scaleFactor));
    ui->pushButton_4->move(ui->pushButton_3->pos().x() + ui->pushButton_3->width(), ui->horizontalLayoutWidget_4->pos().y() + ui->horizontalLayoutWidget_4->height() + (9 * scaleFactor));
    ui->pushButton_5->move(ui->pushButton_4->pos().x() + ui->pushButton_4->width(), ui->horizontalLayoutWidget_4->pos().y() + ui->horizontalLayoutWidget_4->height() + (9 * scaleFactor));
    ui->label_3->move(ui->horizontalLayoutWidget_4->pos().x() + ui->horizontalLayoutWidget_4->width() + (9 * scaleFactor), ui->horizontalLayoutWidget_4->pos().y() + (ui->horizontalLayoutWidget_4->height() / 2) - (ui->label_3->height() / 2));
}

void MainWindow::scaleWidget(QWidget* widget) {
    if (!widget) return;

    QSize currentSize = widget->size();

    int newWidth = static_cast<int>(currentSize.width() * scaleFactor);
    int newHeight = static_cast<int>(currentSize.height() * scaleFactor);

    widget->resize(newWidth, newHeight);
}

void MainWindow::scaleFontnRadiusWidget(QWidget *widget, int fontWidget, int borderRadius)
{
    if (!widget) return;

    QString currentStyleSheet = widget->styleSheet();

    int newFontSize = static_cast<int>(fontWidget * scaleFactor);
    int newBorderRadius = static_cast<int>(borderRadius * scaleFactor);

    QString newStyleSheet = currentStyleSheet + QString(" font-size: %1pt; border-radius: %2px;")
                                                    .arg(newFontSize)
                                                    .arg(newBorderRadius);
    widget->setStyleSheet(newStyleSheet);
}

void MainWindow::setUiGeo()
{
    showNormal();
    this->resize(savedMainWindowSize);
    this->move(savedMainWindowPoint);

    ui->label->setGeometry(savedlabel1Geo);
    ui->label_2->setGeometry(savedlabel2Geo);
    ui->label_3->setGeometry(savedlabel3Geo);
    ui->label_4->setGeometry(savedlabel4Geo);
    ui->label_5->setGeometry(savedlabel5Geo);
    ui->label_6->setGeometry(savedlabel6Geo);
    ui->label_7->setGeometry(savedlabel7Geo);
    ui->label_8->setGeometry(savedlabel8Geo);
    ui->label_11->setGeometry(savedlabel11Geo);
    ui->label_12->setGeometry(savedlabel12Geo);
    ui->label_13->setGeometry(savedlabel13Geo);
    ui->pushButton->setGeometry(savedButton1Geo);
    ui->pushButton_2->setGeometry(savedButton2Geo);
    ui->pushButton_3->setGeometry(savedButton3Geo);
    ui->pushButton_4->setGeometry(savedButton4Geo);
    ui->pushButton_5->setGeometry(savedButton5Geo);
    ui->pushButton_10->setGeometry(savedButton10Geo);
    ui->pushButton_11->setGeometry(savedButton11Geo);
    ui->label->setStyleSheet(savedLabelStyle);
    ui->label_2->setStyleSheet(savedLabel2Style);
    ui->label_4->setStyleSheet(savedLabel4Style);
    ui->label_5->setStyleSheet(savedLabel5Style);
    ui->label_6->setStyleSheet(savedLabel6Style);
    ui->label_7->setStyleSheet(savedLabel7Style);
    ui->label_8->setStyleSheet(savedLabel8Style);
    ui->label_12->setStyleSheet(savedLabel12Style);
    ui->pushButton->setStyleSheet(savedButtonStyle);
    ui->pushButton_10->setStyleSheet(savedButton10Style);
    ui->pushButton_11->setStyleSheet(savedButton11Style);
    if(difficulty == "Middle"){
        ui->label_13->setStyleSheet(QString("font-weight: %1; font-size: %2pt; font-family: 'Segoe UI Black'; color: orange").arg(900).arg(11));
    }
    else if(difficulty == "Hard"){
        ui->label_13->setStyleSheet(QString("font-weight: %1; font-size: %2pt; font-family: 'Segoe UI Black'; color: red").arg(900).arg(11));
    }
    ui->centralwidget->resize(savedMainWindowSize);
    ui->horizontalLayoutWidget_2->setGeometry(savedLayout2Geo);
    ui->horizontalLayoutWidget_3->setGeometry(savedLayout3Geo);
    ui->horizontalLayoutWidget_4->setGeometry(savedLayout4Geo);
    ui->horizontalLayoutWidget->setGeometry(savedLayoutGeo);
    ui->scrollArea->setGeometry(savedScroll);
    QSize s(70,101);
    for(int i = 0; i < botCardsSize; i++){
        botButtons[i]->setIconSize(s);
        botButtons[i]->setFixedSize(s);
    }
    for(int i = 0; i < ColodCardsSize; i++){
        ColodButtons[i]->setIconSize(s);
        ColodButtons[i]->setFixedSize(s);
    }
    if(isShuffl){
        ColodButtons[0]->setIconSize(s);
        ColodButtons[0]->setFixedSize(s);
    }
    for(int i = 0; i < tableCardsSize; i++){
        TableButtons[i]->setIconSize(s);
        TableButtons[i]->setFixedSize(s);
    }
    for(int i = 0; i < playerCardsSize; i++){
        playerButtons[i]->setIconSize(s);
        playerButtons[i]->setFixedSize(s);
    }

    ui->pushButton_10->setIconSize(savedIcon10);
    ui->pushButton_11->setIconSize(savedIcon11);
}

void MainWindow::SaveWindowGeometry()
{
    savedMainWindowPoint = this->pos();
    savedMainWindowSize = this->size();
    savedlabel1Geo = ui->label->geometry();
    savedlabel2Geo = ui->label_2->geometry();
    savedlabel3Geo = ui->label_3->geometry();
    savedlabel4Geo = ui->label_4->geometry();
    savedlabel5Geo = ui->label_5->geometry();
    savedlabel6Geo = ui->label_6->geometry();
    savedlabel7Geo = ui->label_7->geometry();
    savedlabel8Geo = ui->label_8->geometry();
    savedlabel11Geo = ui->label_11->geometry();
    savedlabel12Geo = ui->label_12->geometry();
    savedlabel13Geo = ui->label_13->geometry();
    savedButton1Geo = ui->pushButton->geometry();
    savedButton2Geo = ui->pushButton_2->geometry();
    savedButton3Geo = ui->pushButton_3->geometry();
    savedButton4Geo = ui->pushButton_4->geometry();
    savedButton5Geo = ui->pushButton_5->geometry();
    savedButton10Geo = ui->pushButton_10->geometry();
    savedButton11Geo = ui->pushButton_11->geometry();
    savedLabelStyle = ui->label->styleSheet();
    savedLabel2Style = ui->label_2->styleSheet();
    savedLabel4Style = ui->label_4->styleSheet();
    savedLabel5Style = ui->label_5->styleSheet();
    savedLabel6Style = ui->label_6->styleSheet();
    savedLabel7Style = ui->label_7->styleSheet();
    savedLabel8Style = ui->label_8->styleSheet();
    savedLabel12Style = ui->label_12->styleSheet();
    savedButtonStyle = ui->pushButton->styleSheet();
    savedButton10Style = ui->pushButton_10->styleSheet();
    savedButton11Style = ui->pushButton_11->styleSheet();
    savedLayout2Geo = ui->horizontalLayoutWidget_2->geometry();
    savedLayout3Geo = ui->horizontalLayoutWidget_3->geometry();
    savedLayout4Geo = ui->horizontalLayoutWidget_4->geometry();
    savedLayoutGeo = ui->horizontalLayoutWidget->geometry();
    savedScroll = ui->scrollArea->geometry();
    savedIcon10 = ui->pushButton_10->iconSize();
    savedIcon11 = ui->pushButton_11->iconSize();
}

void MainWindow::on_pushButton_clicked()
{
    soundplayer->stop();
    soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
    soundplayer->setVolume(soundvol / 100.0);
    soundplayer->play();
    Mmove = 1;
    secMove = 0;
    checkForTake = 0;
    BcheckForTake = 0;
    ui->label_2->setText("Enemy turn");
    ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
    if(isFullscreen && isInGame){
        QString cs = ui->label_2->styleSheet();
        ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
    }
    ui->pushButton->hide();
}


void MainWindow::on_pushButton_2_clicked()
{
    soundplayer->stop();
    soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
    soundplayer->setVolume(soundvol / 100.0);
    soundplayer->play();
    Jackchoose = "c";
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    Mmove = 1;
    ui->label_2->setText("Enemy turn");
    ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
    if(isFullscreen && isInGame){
        QString cs = ui->label_2->styleSheet();
        ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
    }
    secMove = 0;
    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/chirva.png);");
    ui->label_3->show();
}


void MainWindow::on_pushButton_3_clicked()
{
    soundplayer->stop();
    soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
    soundplayer->setVolume(soundvol / 100.0);
    soundplayer->play();
    Jackchoose = "k";
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    Mmove = 1;
    ui->label_2->setText("Enemy turn");
    ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
    if(isFullscreen && isInGame){
        QString cs = ui->label_2->styleSheet();
        ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
    }
    secMove = 0;
    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/kresti.png);");
    ui->label_3->show();
}


void MainWindow::on_pushButton_4_clicked()
{
    soundplayer->stop();
    soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
    soundplayer->setVolume(soundvol / 100.0);
    soundplayer->play();
    Jackchoose = "b";
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    Mmove = 1;
    ui->label_2->setText("Enemy turn");
    ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
    if(isFullscreen && isInGame){
        QString cs = ui->label_2->styleSheet();
        ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
    }
    secMove = 0;
    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/bybna.png);");
    ui->label_3->show();
}


void MainWindow::on_pushButton_5_clicked()
{
    soundplayer->stop();
    soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
    soundplayer->setVolume(soundvol / 100.0);
    soundplayer->play();
    Jackchoose = "p";
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    Mmove = 1;
    ui->label_2->setText("Enemy turn");
    ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
    if(isFullscreen && isInGame){
        QString cs = ui->label_2->styleSheet();
        ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
    }
    secMove = 0;
    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/piki.png);");
    ui->label_3->show();
}


void MainWindow::on_pushButton_6_clicked()
{
    isInGame = true;

    soundplayer->stop();
    soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
    soundplayer->setVolume(soundvol / 100.0);
    soundplayer->play();
    player->stop();
    player->setSource(QUrl::fromLocalFile(tempFilePathTheme));
    audioOutput->setVolume((musicvol / 100.0) * 0.5);
    player->setLoops(-1);
    player->play();

    window->hide();
    ui->pushButton_6->hide();
    ui->pushButton_7->hide();
    ui->pushButton_8->hide();
    ui->pushButton_9->hide();
    ui->label_9->hide();
    ui->label_10->hide();
    ui->line->hide();
    ui->line_2->hide();

    ui->label_5->setText(QString::fromStdString("Points: " + std::to_string(playerPoints)));
    ui->label_6->setText(QString::fromStdString("Points: " + std::to_string(botPoints)));
    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    ui->label_5->setVisible(true);
    ui->label_6->setVisible(true);
    ui->label_7->setVisible(true);
    Set = 1;
    ui->label_8->setText(QString::fromStdString("Set: " + std::to_string(Set)));
    ui->label_8->setVisible(true);
    ui->pushButton_10->setVisible(true);
    ui->pushButton_11->setVisible(true);

    for(int i = 0; i < botCardsSize; i++){
        delete botButtons[i];
        botCards[i] = "";
    }
    for(int i = 0; i < playerCardsSize; i++){
        delete playerButtons[i];
        playerCards[i] = "";
    }
    for(int i = 0; i < ColodCardsSize; i++){
        delete ColodButtons[i];
        ColodCards[i] = "";
    }
    for(int i = 0; i < tableCardsSize; i++){
        delete TableButtons[i];
        tableCards[i] = "";
    }

    playerCardsSize = 0;
    botCardsSize = 0;
    ColodCardsSize = 0;
    tableCardsSize = 0;
    playerPoints = 0;
    botPoints = 0;

    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    ui->label_3->hide();
    ui->label_4->hide();

    checkForTake = 0;
    BcheckForTake = 0;

    PointsX = 1;
    ui->label_7->setText(QString::fromStdString("Points " + std::to_string(PointsX) + "x"));

    pJackKol = 0;
    bJackKol = 0;

    Start();
    bool mv = Mmove;
    if(Mmove == 1 && tableCards[0][0] == 'J'){
        int sign[4];
        sign[0] = 0;
        sign[1] = 0;
        sign[2] = 0;
        sign[3] = 0;
        for(int i = 0; i < botCardsSize; i++){
            if(botCards[i][0] != 'J'){
                if(botCards[i][1] == 'c'){
                    sign[0]++;
                }
                else if(botCards[i][1] == 'k'){
                    sign[1]++;
                }
                else if(botCards[i][1] == 'b'){
                    sign[2]++;
                }
                else if(botCards[i][1] == 'p'){
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
            Jackchoose = "c";
            ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/chirva.png);");
            ui->label_3->show();
        }
        else if(k == 1){
            Jackchoose = "k";
            ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/kresti.png);");
            ui->label_3->show();
        }
        else if(k == 2){
            Jackchoose = "b";
            ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/bybna.png);");
            ui->label_3->show();
        }
        else if(k == 3){
            Jackchoose = "p";
            ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/piki.png);");
            ui->label_3->show();
        }
    }
    ui->label_12->show();
    ui->label_13->show();
    ui->scrollArea->show();
    ui->label_13->setText(QString::fromStdString(difficulty));
    if(difficulty == "Middle"){
        ui->label_13->setText("M");
        ui->label_13->setStyleSheet("font-weight: 900; font-size: 11pt; font-family: 'Segoe UI Black'; color: orange");
    }
    else if(difficulty == "Hard"){
        ui->label_13->setText("H");
        ui->label_13->setStyleSheet("font-weight: 900; font-size: 11pt; font-family: 'Segoe UI Black'; color: red");
    }
    if(QSMode)
        ui->label_11->show();
    if(PointsMode)
        ui->label_12->setText("225");
    else
        ui->label_12->setText("125");
    secondmove();
    operation(mv);

    timer = new QTimer();
    if(difficulty == "Middle"){
        connect(timer, SIGNAL(timeout()), this, SLOT(botMove()));
    }
    else if(difficulty == "Hard"){
        connect(timer, SIGNAL(timeout()), this, SLOT(chooseBestMove()));
    }
    timer->start(2000);

    AutoSave();

    SaveWindowGeometry();

    Resizing();
}


void MainWindow::on_pushButton_8_clicked()
{
    soundplayer->stop();
    soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
    soundplayer->setVolume(soundvol / 100.0);
    soundplayer->play();
    QMessageBox YesNomsgBox(this);
    YesNomsgBox.setWindowTitle("Quit");
    YesNomsgBox.setText("Are you sure?");
    YesNomsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    YesNomsgBox.setDefaultButton(QMessageBox::No);
    YesNomsgBox.setStyleSheet(massBoxStyle);

    int reply = YesNomsgBox.exec();
    soundplayer->stop();
    soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
    soundplayer->setVolume(soundvol / 100.0);
    soundplayer->play();

    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void MainWindow::on_pushButton_9_clicked()
{
    isInGame = true;

    soundplayer->stop();
    soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
    soundplayer->setVolume(soundvol / 100.0);
    soundplayer->play();
    player->stop();
    player->setSource(QUrl::fromLocalFile(tempFilePathTheme));
    audioOutput->setVolume((musicvol / 100.0) * 0.5);
    player->setLoops(-1);
    player->play();

    window->hide();
    ui->pushButton_6->hide();
    ui->pushButton_7->hide();
    ui->pushButton_8->hide();
    ui->pushButton_9->hide();
    ui->label_9->hide();
    ui->label_10->hide();
    ui->line->hide();
    ui->line_2->hide();

    ui->label_5->setText(QString::fromStdString("Points: " + std::to_string(playerPoints)));
    ui->label_6->setText(QString::fromStdString("Points: " + std::to_string(botPoints)));
    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    ui->label_5->setVisible(true);
    ui->label_6->setVisible(true);
    ui->label_7->setVisible(true);
    Set = 1;
    ui->label_8->setText(QString::fromStdString("Set: " + std::to_string(Set)));
    ui->label_8->setVisible(true);
    ui->pushButton_10->setVisible(true);
    ui->pushButton_11->setVisible(true);

    for(int i = 0; i < botCardsSize; i++){
        delete botButtons[i];
        botCards[i] = "";
    }
    for(int i = 0; i < playerCardsSize; i++){
        delete playerButtons[i];
        playerCards[i] = "";
    }
    for(int i = 0; i < ColodCardsSize; i++){
        delete ColodButtons[i];
        ColodCards[i] = "";
    }
    for(int i = 0; i < tableCardsSize; i++){
        delete TableButtons[i];
        tableCards[i] = "";
    }

    playerCardsSize = 0;
    botCardsSize = 0;
    ColodCardsSize = 0;
    tableCardsSize = 0;
    playerPoints = 0;
    botPoints = 0;

    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    ui->label_3->hide();
    ui->label_4->hide();

    checkForTake = 0;
    BcheckForTake = 0;

    PointsX = 1;
    ui->label_7->setText(QString::fromStdString("Points " + std::to_string(PointsX) + "x"));

    pJackKol = 0;
    bJackKol = 0;

    std::ifstream fin;
    fin.open(dirGame.toStdString());

    if(fin.is_open()){

        std::string bucket;
        fin >> bucket;

        fin >> playerCardsSize;
        for(int i=0; i < playerCardsSize; i++){
            fin >> playerCards[i];
            onAddWidgetPlayer(playerCards[i], i);
        }

        fin >> botCardsSize;
        for(int i=0; i < botCardsSize; i++){
            fin >> botCards[i];
            onAddWidgetBot(i);
        }

        fin >> ColodCardsSize;
        ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));
        for(int i=0; i < ColodCardsSize; i++){
            fin >> ColodCards[i];
            onAddWidgetColod(i);
        }

        fin >> tableCardsSize;
        for(int i=0; i < tableCardsSize; i++){
            fin >> tableCards[i];
            onAddWidgetTable(tableCards[i], i);
        }

        fin >> Mmove;
        if(!Mmove){
            ui->label_2->setText("Your turn");

            ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
            if(isFullscreen && isInGame){
                QString cs = ui->label_2->styleSheet();
                ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
            }
        }
        else{
            ui->label_2->setText("Enemy turn");
            ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px; color: white; font: bold;");
            if(isFullscreen && isInGame){
                QString cs = ui->label_2->styleSheet();
                ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
            }
        }
        fin >> secMove;
        fin >> Jackchoose;
        if(Jackchoose == "empty")
            Jackchoose = "";
        fin >> checkForTake;
        fin >> BcheckForTake;
        fin >> playerPoints;
        ui->label_5->setText(QString::fromStdString("Points: " + std::to_string(playerPoints)));
        fin >> botPoints;
        ui->label_6->setText(QString::fromStdString("Points: " + std::to_string(botPoints)));
        fin >> PointsX;
        ui->label_7->setText(QString::fromStdString("Points " + std::to_string(PointsX) + "x"));
        fin >> pJackKol;
        fin >> bJackKol;
        fin >> Set;
        ui->label_8->setText(QString::fromStdString("Set: " + std::to_string(Set)));

        bool cheaker;
        fin >> cheaker;

        if(cheaker)
            ui->pushButton->show();
        else
            ui->pushButton->hide();

        fin >> cheaker;

        if(cheaker){
            ui->pushButton_2->show();
            ui->pushButton_3->show();
            ui->pushButton_4->show();
            ui->pushButton_5->show();
        }
        else{
            ui->pushButton_2->hide();
            ui->pushButton_3->hide();
            ui->pushButton_4->hide();
            ui->pushButton_5->hide();
        }

        fin >> cheaker;
         if(cheaker){
             if(Jackchoose == "c"){
                 ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/chirva.png);");
             }
             if(Jackchoose == "k"){
                 ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/kresti.png);");
             }
             if(Jackchoose == "b"){
                 ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/bybna.png);");
             }
             if(Jackchoose == "p"){
                 ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/piki.png);");
             }
             ui->label_3->show();
         }
         fin >> QSMode;
         fin >> PointsMode;
         fin >> difficulty;
    }
    fin.close();

    if(ColodCardsSize == 0){
        onAddWidgetColod(0);
        isShuffl = true;
        ui->label_4->setText(QString::fromStdString("^ \nShuffling (" + std::to_string(PointsX + 1) + "x)"));
        ui->label_4->show();
    }

    ui->label_12->show();
    ui->label_13->show();
    ui->scrollArea->show();
    ui->label_13->setText(QString::fromStdString(difficulty));
    if(difficulty == "Middle"){
        ui->label_13->setText("M");
        ui->label_13->setStyleSheet("font-weight: 900; font-size: 11pt; font-family: 'Segoe UI Black'; color: orange");
    }
    else if(difficulty == "Hard"){
        ui->label_13->setText("H");
        ui->label_13->setStyleSheet("font-weight: 900; font-size: 11pt; font-family: 'Segoe UI Black'; color: red");
    }
    if(QSMode)
        ui->label_11->show();
    if(PointsMode)
        ui->label_12->setText("225");
    else
        ui->label_12->setText("125");

    timer = new QTimer();
    if(difficulty == "Middle"){
        connect(timer, SIGNAL(timeout()), this, SLOT(botMove()));
    }
    else if(difficulty == "Hard"){
        connect(timer, SIGNAL(timeout()), this, SLOT(chooseBestMove()));
    }
    timer->start(2000);

    if(isFullscreen)
        ui->scrollAreaWidgetContents->setMinimumSize(QSize((70*scaleFactor)*playerCardsSize,0));
    else
        ui->scrollAreaWidgetContents->setMinimumSize(QSize(70*playerCardsSize,0));

    SaveWindowGeometry();

    Resizing();
}

void MainWindow::on_pushButton_7_clicked()
{
    soundplayer->stop();
    soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
    soundplayer->setVolume(soundvol / 100.0);
    soundplayer->play();
    window->hide();
    window->setWindowTitle("Options");
    window->setWindowIcon(QIcon(":/img/PNG-cards-1.3/kindpng_4637727.png"));
    window->show();
}

void MainWindow::QSMslot(QString text)
{
    if(text == "On"){
        QSMode = true;
    }
    else{
        QSMode = false;
    }
    OptionsSave();
}

void MainWindow::Pointsslot(QString text)
{
    if(text == "225"){
        PointsMode = true;
    }
    else{
        PointsMode = false;
    }
    OptionsSave();
}

void MainWindow::Difficultyslot(QString text)
{
    difficulty = text.toStdString();
    OptionsSave();
}

void MainWindow::Musicslot(int val)
{
    QUrl currentSource = player->source();
    int currentSourceSize = currentSource.toString().size();

    if(!currentSource.isEmpty()){
        musicvol = val;
        if(currentSource.toString()[currentSourceSize - 5] == 'c'){
            audioOutput->setVolume((musicvol / 100.0) * 0.1);
        }
        else{
            audioOutput->setVolume((musicvol / 100.0) * 0.5);
        }

        OptionsSave();
    }
}

void MainWindow::Soundslot(int val)
{
    soundvol = val;
    soundplayer->setVolume(soundvol / 100.0);
    OptionsSave();
}

void MainWindow::Displayslot(bool val)
{
    isFullscreen = val;
    isFromSet = true;
    OptionsSave();
    QString lStr = ui->label_2->text();
    Resizing(lStr);
}

void MainWindow::on_pushButton_10_clicked()
{   
    soundplayer->stop();
    soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
    soundplayer->setVolume(soundvol / 100.0);
    soundplayer->play();
    QMessageBox YesNomsgBox(this);
    YesNomsgBox.setWindowTitle("Quit");
    YesNomsgBox.setText("Are you sure?");
    YesNomsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    YesNomsgBox.setDefaultButton(QMessageBox::No);
    YesNomsgBox.setStyleSheet(massBoxStyle);

    int reply = YesNomsgBox.exec();
    soundplayer->stop();
    soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
    soundplayer->setVolume(soundvol / 100.0);
    soundplayer->play();

    if (reply == QMessageBox::Yes) {
        isInGame = false;

        for(int i = 0; i < botCardsSize; i++){
            delete botButtons[i];
            botCards[i] = "";
        }
        for(int i = 0; i < playerCardsSize; i++){
            delete playerButtons[i];
            playerCards[i] = "";
        }
        if(ColodCardsSize == 0){
            delete ColodButtons[0];
        }
        for(int i = 0; i < ColodCardsSize; i++){
            delete ColodButtons[i];
            ColodCards[i] = "";
        }
        for(int i = 0; i < tableCardsSize; i++){
            delete TableButtons[i];
            tableCards[i] = "";
        }
        botCardsSize = 0;
        playerCardsSize = 0;
        ColodCardsSize = 0;
        tableCardsSize = 0;

        checkForTake = 0;
        BcheckForTake = 0;

        PointsX = 1;
        ui->label->setVisible(false);
        ui->label_2->setVisible(false);

        ui->pushButton->hide();
        ui->pushButton_2->hide();
        ui->pushButton_3->hide();
        ui->pushButton_4->hide();
        ui->pushButton_5->hide();
        ui->pushButton_10->hide();
        ui->pushButton_11->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->label_5->hide();
        ui->label_6->hide();
        ui->label_7->hide();
        ui->label_8->hide();
        ui->label_11->hide();
        ui->label_12->hide();
        ui->label_13->hide();
        ui->scrollArea->hide();
        ui->label_7->setText(QString::fromStdString("Points " + std::to_string(PointsX) + "x"));

        pJackKol = 0;
        bJackKol = 0;

        delete timer;
        botPoints = 0;
        playerPoints = 0;
        ui->pushButton_6->show();
        ui->pushButton_7->show();
        ui->pushButton_8->show();
        ui->pushButton_9->show();
        ui->pushButton_6->setGeometry(338, 160, 121, 71);
        ui->pushButton_6->setText("Start The\nNew Game");
        ui->pushButton_6->setStyleSheet("font-family: 'Segoe UI'; font-size: 12pt; border: 2px solid; border-color: black; border-radius: 20px; background-color: white; color: black; font: bold;");
        ui->label_11->hide();
        ui->label_12->hide();
        ui->label_13->hide();
        ui->scrollArea->hide();
        ui->label_9->show();
        ui->label_10->show();
        ui->line->show();
        ui->line_2->show();

        player->stop();
        player->setSource(QUrl::fromLocalFile(tempFilePathMenu));
        audioOutput->setVolume((musicvol / 100.0) * 0.1);
        player->setLoops(-1);
        player->play();

        Resizing();
    }
}

void MainWindow::on_pushButton_11_clicked()
{
    soundplayer->stop();
    soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
    soundplayer->setVolume(soundvol / 100.0);
    soundplayer->play();
    window->hide();
    window->setWindowTitle("Options");
    window->setWindowIcon(QIcon(":/img/PNG-cards-1.3/kindpng_4637727.png"));
    window->show();
}
