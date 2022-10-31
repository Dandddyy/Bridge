#include "mainwindow.h"
#include <QMessageBox>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    playerCardsSize = 0;
    botCardsSize = 0;
    ColodCardsSize = 0;
    tableCardsSize = 0;

    ui->label->setVisible(false);
    ui->label_2->setVisible(false);

    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    ui->label_3->hide();
    ui->label_4->hide();

    checkForTake = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
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
    playerButtons[iter]->setIconSize(s);
    playerButtons[iter]->setFixedSize(s);
    QObject::connect(playerButtons[iter], &QPushButton::clicked, this, &MainWindow::onRemoveWidgetPlayer);
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
    botButtons[iter]->setIconSize(s);
    botButtons[iter]->setFixedSize(s);
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
    ColodButtons[iter]->setIconSize(s);
    ColodButtons[iter]->setFixedSize(s);
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
    TableButtons[iter]->setIconSize(s);
    TableButtons[iter]->setFixedSize(s);
}

void MainWindow::onRemoveWidgetPlayer()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    bool maincheck = false;
    bool mv;
    for(int i = 0; i < playerCardsSize; i++){
        bool check;
        if(Jackchoose == "" && move == 0){
            check = possibleMove(playerCards[i], tableCards[tableCardsSize - 1], secMove);
        }
        else if(move == 0){
            check = possibleMove(playerCards[i], "0" + Jackchoose, 0);
            Jackchoose = "";
        }
        if(button == playerButtons[i] && move == 0 && check == true){
            maincheck = true;
            ui->pushButton->hide();
            ui->pushButton_2->hide();
            ui->pushButton_3->hide();
            ui->pushButton_4->hide();
            ui->pushButton_5->hide();
            ui->label_3->hide();
            tableCards[tableCardsSize] = playerCards[i];
            onAddWidgetTable(tableCards[tableCardsSize], tableCardsSize);
            tableCardsSize++;
            delete playerButtons[i];
            playerCards[i] = "";
            mv = move;
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
        secondmove();
        operation(mv);
        checkForTake = 0;
    }
}

void MainWindow::onRemoveWidgetColod()
{
    if(move == 0 && checkForTake == 0){
        if(tableCards[tableCardsSize - 1][0] == '6' || secMove == 0){
            playerCards[playerCardsSize] = ColodCards[ColodCardsSize - 1];
            onAddWidgetPlayer(playerCards[playerCardsSize], playerCardsSize);
            playerCardsSize++;
            ColodCards[ColodCardsSize - 1] = "";
            delete ColodButtons[ColodCardsSize - 1];
            ColodCardsSize--;
            std::string kol = std::to_string(ColodCardsSize);
            ui->label->setText(QString::fromStdString(kol));
            if(tableCards[tableCardsSize - 1][0] != '6'){
                ui->pushButton->show();
                checkForTake = 1;
            }
            if(ColodCardsSize == 0){
                onAddWidgetColod(0);
                ColodCardsSize++;
                ui->label_4->show();
            }
        }
    }
    else if(move == 1 && checkForTake == 1){
        botCards[botCardsSize] = ColodCards[ColodCardsSize - 1];
        onAddWidgetBot(botCardsSize);
        botCardsSize++;
        ColodCards[ColodCardsSize - 1] = "";
        delete ColodButtons[ColodCardsSize - 1];
        ColodCardsSize--;
        std::string kol = std::to_string(ColodCardsSize);
        ui->label->setText(QString::fromStdString(kol));
        if(ColodCardsSize == 0){
            onAddWidgetColod(0);
            ColodCardsSize++;
            ui->label_4->show();
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
        if(tableCards[tableCardsSize - 1][0] == 'A'){
            secMove = 0;
            move = 0;
            ui->label_2->setText("Your turn");
        }
        else if(tableCards[tableCardsSize - 1][0] == '8'){
            if(ColodCardsSize >= 2){
                secMove = 0;
                move = 0;
                ui->label_2->setText("Your turn");
                botCards[botCardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetBot(botCardsSize);
                botCardsSize++;
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodCardsSize--;
                botCards[botCardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetBot(botCardsSize);
                botCardsSize++;
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodCardsSize--;
                std::string kol = std::to_string(ColodCardsSize);
                ui->label->setText(QString::fromStdString(kol));
                if(ColodCardsSize == 0){
                    onAddWidgetColod(0);
                    ColodCardsSize++;
                    ui->label_4->show();
                }
            }
            else{

            }
        }
        else if(tableCards[tableCardsSize - 1][0] == '7'){
            if(ColodCardsSize >= 1){
                botCards[botCardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetBot(botCardsSize);
                botCardsSize++;
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodCardsSize--;
                std::string kol = std::to_string(ColodCardsSize);
                ui->label->setText(QString::fromStdString(kol));
                if(ColodCardsSize == 0){
                    onAddWidgetColod(0);
                    ColodCardsSize++;
                    ui->label_4->show();
                }
            }
            else{

            }
        }
    }
    else{
        if(tableCards[tableCardsSize - 1][0] == 'A'){
            secMove = 0;
            move = 1;
            ui->label_2->setText("Enemy turn");
        }
        else if(tableCards[tableCardsSize - 1][0] == '8'){
            if(ColodCardsSize >= 2){
                secMove = 0;
                move = 1;
                ui->label_2->setText("Enemy turn");
                playerCards[playerCardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetPlayer(playerCards[playerCardsSize], playerCardsSize);
                playerCardsSize++;
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodCardsSize--;
                playerCards[playerCardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetPlayer(playerCards[playerCardsSize], playerCardsSize);
                playerCardsSize++;
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodCardsSize--;
                std::string kol = std::to_string(ColodCardsSize);
                ui->label->setText(QString::fromStdString(kol));
                if(ColodCardsSize == 0){
                    onAddWidgetColod(0);
                    ColodCardsSize++;
                    ui->label_4->show();
                }
            }
            else{

            }
        }
        else if(tableCards[tableCardsSize - 1][0] == '7'){
            if(ColodCardsSize >= 1){
                playerCards[playerCardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetPlayer(playerCards[playerCardsSize], playerCardsSize);
                playerCardsSize++;
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodCardsSize--;
                std::string kol = std::to_string(ColodCardsSize);
                ui->label->setText(QString::fromStdString(kol));
                if(ColodCardsSize == 0){
                    onAddWidgetColod(0);
                    ColodCardsSize++;
                    ui->label_4->show();
                }
            }
            else{

            }
        }
    }
}

void MainWindow::secondmove()
{
    if(move == 0){
        secMove = 0;
        move = 1;
        ui->label_2->setText("Enemy turn");
        for(int i = 0; i < playerCardsSize; i++){
            if(playerCards[i][0] == tableCards[tableCardsSize - 1][0] || tableCards[tableCardsSize - 1][0] == '6' || tableCards[tableCardsSize - 1][0] == 'J'){
                secMove = 1;
                move = 0;
                ui->label_2->setText("Your turn");
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
        move = 0;
        ui->label_2->setText("Your turn");
        for(int i = 0; i < botCardsSize; i++){
            if(botCards[i][0] == tableCards[tableCardsSize - 1][0] || tableCards[tableCardsSize - 1][0] == '6'){
                secMove = 1;
                move = 1;
                ui->label_2->setText("Enemy turn");
            }
        }
    }
}

void MainWindow::Start()
{

    srand(time(NULL));
    move = rand() % 2;

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
        if(i == 4 && move == 0){
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
            if(i == 4 && move == 0 && tableCards[0] == cardNames[it][jt]){
                check1 = false;
            }
            if(check1 == true){
                check = true;
            }
        }
        if(i == 4 && move == 1){
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
        std::string kol = std::to_string(ColodCardsSize);
        ui->label->setText(QString::fromStdString(kol));

        onAddWidgetColod(i);
    }
}


void MainWindow::on_actionStart_the_game_triggered()
{
    ui->label->setVisible(true);
    ui->label_2->setVisible(true);

    playerCardsSize = 0;
    botCardsSize = 0;
    ColodCardsSize = 0;
    tableCardsSize = 0;

    Start();
    bool mv = move;
    secondmove();
    operation(mv);
}


void MainWindow::on_actionQuit_2_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Quit", "Are you sure?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        QApplication::quit();
    }
}


void MainWindow::on_pushButton_clicked()
{
    move = 1;
    checkForTake = 0;
    ui->label_2->setText("Enemy turn");
    ui->pushButton->hide();
}


void MainWindow::on_pushButton_2_clicked()
{
    Jackchoose = "c";
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    move = 1;
    ui->label_2->setText("Enemy turn");
    secMove = 0;
    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/chirva.png);");
    ui->label_3->show();
}


void MainWindow::on_pushButton_3_clicked()
{
    Jackchoose = "k";
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    move = 1;
    ui->label_2->setText("Enemy turn");
    secMove = 0;
    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/kresti.png);");
    ui->label_3->show();
}


void MainWindow::on_pushButton_4_clicked()
{
    Jackchoose = "b";
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    move = 1;
    ui->label_2->setText("Enemy turn");
    secMove = 0;
    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/bybna.png);");
    ui->label_3->show();
}


void MainWindow::on_pushButton_5_clicked()
{
    Jackchoose = "p";
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    move = 1;
    ui->label_2->setText("Enemy turn");
    secMove = 0;
    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/piki.png);");
    ui->label_3->show();
}

