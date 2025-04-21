#include "mainwindow.h"
#include <QMessageBox>
#include "ui_mainwindow.h"
#include <fstream>
#include <QUrl>
#include <QScrollBar>
#include <QWheelEvent>
#include <QApplication>
#include "player.h"
#include "human.h"
#include "bot.h"
#include "gameserver.h"
#include "gameclient.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    window = new optionwindow;

    ui->scrollArea->installEventFilter(this);

    QString saveDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Saves";
    QDir().mkpath(saveDir);
    QDir().mkpath(saveDir);

    dirGame = saveDir + "/GameData.txt";
    dirName = saveDir + "/Name.txt";
    dirOption = saveDir + "/OptionsData.txt";

    setCentralWidget(ui->centralwidget);

    connect(this, &MainWindow::QSMsignal2, window, &optionwindow::QSMslot2);

    std::ifstream fin;
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

    ColodCardsSize = 0;
    tableCardsSize = 0;
    Set = 1;

    ui->label->setVisible(false);
    ui->label_2->setVisible(false);

    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    ui->pushButton_6->hide();
    ui->pushButton_10->hide();
    ui->pushButton_11->hide();
    ui->pushButton_14->hide();
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
    ui->pushButton_15->hide();
    ui->pushButton_16->hide();
    ui->pushButton_17->hide();
    ui->pushButton_18->hide();
    ui->pushButton_19->hide();
    ui->label_20->hide();
    ui->pushButton_20->hide();
    ui->pushButton_27->hide();
    ui->lineEdit->hide();
    ui->lineEdit->setPlaceholderText("Name");
    ui->lineEdit->setMaxLength(10);
    ui->lineEdit_3->setMaxLength(10);
    ui->comboBox_2->hide();
    ui->comboBox_3->hide();
    ui->comboBox_4->hide();
    ui->comboBox_5->hide();
    ui->comboBox_6->hide();
    ui->comboBox_7->hide();
    ui->pushButton_21->hide();
    ui->pushButton_22->hide();
    ui->pushButton_23->hide();
    ui->pushButton_24->hide();
    ui->pushButton_25->hide();
    ui->pushButton_26->hide();
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
    ui->label_14->hide();
    ui->label_15->hide();
    ui->label_21->hide();
    ui->label_22->hide();
    ui->lineEdit_2->hide();
    ui->lineEdit_3->hide();
    ui->pushButton_28->hide();
    ui->label_23->hide();
    ui->label_24->hide();
    ui->label_25->hide();
    ui->label_26->hide();
    ui->horizontalLayoutWidget->lower();
    ui->horizontalLayoutWidget_2->lower();
    ui->horizontalLayoutWidget_3->lower();
    ui->horizontalLayoutWidget_4->lower();
    ui->verticalLayoutWidget->lower();
    ui->verticalLayoutWidget_2->lower();

    PointsX = 1;
    ui->label_7->setText(QString::fromStdString("Points " + std::to_string(PointsX) + "x"));

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
    QIcon ic2(":/img/PNG-cards-1.3/left.png");
    ui->pushButton_19->setIcon(ic2);
    QIcon ic3(":/img/PNG-cards-1.3/create.png");
    ui->pushButton_12->setIcon(ic3);
    ui->pushButton_12->setIconSize(QSize(22,22));
    QIcon ic4(":/img/PNG-cards-1.3/connect.png");
    ui->pushButton_13->setIcon(ic4);
    ui->pushButton_13->setIconSize(QSize(21,21));
    QIcon ic5(":/img/PNG-cards-1.3/fast-forward.png");
    ui->pushButton_14->setIcon(ic5);
    QIcon ic6(":/img/PNG-cards-1.3/kick.png");
    ui->pushButton_21->setIcon(ic6);
    ui->pushButton_22->setIcon(ic6);
    ui->pushButton_23->setIcon(ic6);
    QIcon ic7(":/img/PNG-cards-1.3/swap.png");
    ui->pushButton_24->setIcon(ic7);
    ui->pushButton_25->setIcon(ic7);
    ui->pushButton_26->setIcon(ic7);
    QIcon ic8(":/img/PNG-cards-1.3/copy.png");
    ui->pushButton_27->setIcon(ic8);

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

    playersCount = 1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

QLabel* MainWindow::getLabel()
{
    if(ui->comboBox_2->currentIndex() == 1 && ui->label_17->text() == ""){
        ui->pushButton_21->show();
        ui->label_17->show();
        startCheck(0);
        return ui->label_17;
    }
    else if(ui->comboBox_5->currentIndex() == 1 && ui->label_18->text() == ""){
        ui->pushButton_22->show();
        ui->label_18->show();
        startCheck(0);
        return ui->label_18;
    }
    else{
        ui->pushButton_23->show();
        ui->label_19->show();
        startCheck(0);
        return ui->label_19;
    }
}

void MainWindow::ClientDisconnected(QLabel *label)
{
    if(ui->label_17 == label){
        ui->label_17->hide();
        ui->label_17->setText("");
        ui->pushButton_21->hide();
    }
    else if(ui->label_18 == label){
        ui->label_18->hide();
        ui->label_18->setText("");
        ui->pushButton_22->hide();
    }
    else if(ui->label_19 == label){
        ui->label_19->hide();
        ui->label_19->setText("");
        ui->pushButton_23->hide();
    }
    startCheck(0);
}

void MainWindow::connectionError(const QString &error)
{
    cleaner(isShuffl);
    ui->lineEdit->hide();

    ui->comboBox_2->hide();
    ui->comboBox_3->hide();
    ui->comboBox_5->hide();
    ui->comboBox_4->hide();
    ui->comboBox_7->hide();
    ui->comboBox_6->hide();
    ui->label_25->hide();
    ui->label_26->hide();
    ui->label_17->setText("");
    ui->label_18->setText("");
    ui->label_19->setText("");

    ui->pushButton_15->hide();
    ui->pushButton_16->hide();
    ui->pushButton_17->hide();
    ui->pushButton_24->hide();
    ui->pushButton_25->hide();
    ui->pushButton_26->hide();
    ui->label_23->hide();
    ui->label_24->hide();

    ui->pushButton_11->show();
    ui->pushButton_11->move(630, 20);
    ui->label_21->show();
    ui->label_22->show();
    ui->lineEdit_2->show();
    ui->lineEdit_3->show();
    ui->pushButton_28->show();
    ui->pushButton_19->show();

    if(movie){
        movie->stop();
        delete movie;
        movie = nullptr;
    }

    QMessageBox msgBox(this);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    foreach (QPushButton *button, msgBox.findChildren<QPushButton*>()) {
        button->setCursor(Qt::PointingHandCursor);
    }
    msgBox.setWindowTitle("Error");
    msgBox.setStyleSheet(massBoxStyle);
    msgBox.setText(error);
    msgBox.exec();
    clickedSound();
}

void MainWindow::fromHostMessage(const QString &message)
{
    QMessageBox msgBox(this);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    foreach (QPushButton *button, msgBox.findChildren<QPushButton*>()) {
        button->setCursor(Qt::PointingHandCursor);
    }
    msgBox.setWindowTitle("Message");
    msgBox.setStyleSheet(massBoxStyle);
    msgBox.setText(message);
    msgBox.exec();
    clickedSound();
}

QJsonObject MainWindow::hubJson(QLabel *label)
{
    QJsonObject json;
    json["big1"] = ui->pushButton_15->pos().x();
    json["big2"] = ui->pushButton_16->pos().x();
    json["big3"] = ui->pushButton_17->pos().x();
    json["uper1"] = ui->comboBox_2->currentIndex();
    json["uper2"] = ui->comboBox_5->currentIndex();
    json["uper3"] = ui->comboBox_7->currentIndex();
    if(label == ui->label_17){
        json["lower1"] = 2;
        json["lower2"] = ui->comboBox_4->currentIndex();
        json["lower3"] = ui->comboBox_6->currentIndex();
    }
    else if(label == ui->label_18){
        json["lower1"] = ui->comboBox_3->currentIndex();
        json["lower2"] = 2;
        json["lower3"] = ui->comboBox_6->currentIndex();
    }
    else{
        json["lower1"] = ui->comboBox_3->currentIndex();
        json["lower2"] = ui->comboBox_4->currentIndex();
        json["lower3"] = 2;
    }
    json["label1"] = ui->label_17->text();
    json["label2"] = ui->label_18->text();
    json["label3"] = ui->label_19->text();
    json["host"] = ui->lineEdit->text();
    json["points"] = PointsMode;
    json["QSM"] = QSMode;
    return json;
}

void MainWindow::parseHub(const QJsonObject &json)
{
    ui->pushButton_15->hide();
    ui->pushButton_16->hide();
    ui->pushButton_17->hide();
    ui->pushButton_18->hide();
    ui->pushButton_19->hide();
    ui->label_20->hide();
    ui->pushButton_20->hide();
    ui->pushButton_27->hide();
    ui->lineEdit->hide();
    ui->comboBox_2->hide();
    ui->comboBox_3->hide();
    ui->comboBox_4->hide();
    ui->comboBox_5->hide();
    ui->comboBox_6->hide();
    ui->comboBox_7->hide();
    ui->pushButton_21->hide();
    ui->pushButton_22->hide();
    ui->pushButton_23->hide();
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
    ui->pushButton_24->hide();
    ui->pushButton_25->hide();
    ui->pushButton_26->hide();
    ui->label_23->hide();
    ui->label_24->hide();

    ui->lineEdit->setDisabled(true);

    ui->comboBox_2->setDisabled(true);
    ui->comboBox_3->setDisabled(true);
    ui->comboBox_5->setDisabled(true);
    ui->comboBox_4->setDisabled(true);
    ui->comboBox_7->setDisabled(true);
    ui->comboBox_6->setDisabled(true);
    ui->label_17->setText("");
    ui->label_18->setText("");
    ui->label_19->setText("");

    ui->pushButton_15->setDisabled(true);
    ui->pushButton_16->setDisabled(true);
    ui->pushButton_17->setDisabled(true);
    ui->pushButton_15->show();
    ui->pushButton_16->show();
    ui->pushButton_17->show();
    ui->lineEdit->show();
    ui->label_25->show();
    ui->label_26->show();
    ui->pushButton_19->show();
    ui->pushButton_11->show();
    ui->pushButton_11->move(630, 20);

    ui->label_17->move(314, 61);
    ui->label_18->move(94, 221);
    ui->label_19->move(534, 221);

    if(movie){
        movie->stop();
        delete movie;
        movie = nullptr;
    }

    if(json["lower1"].toInt() == 2){
        QIcon ic4(":/img/PNG-cards-1.3/cross.png");
        ui->pushButton_15->setIcon(ic4);
        ui->pushButton_15->move(257, 30);
        ui->comboBox_2->setCurrentIndex(1);
        ui->comboBox_2->show();
        ui->label_17->setText(json["host"].toString());
        ui->label_17->show();
        ui->pushButton_24->show();

        hubParseHelper(ui->pushButton_17, QPoint(530, 190), QPoint(477, 190), json["big2"].toInt(), ui->pushButton_26, ui->comboBox_7,
                       ui->comboBox_6, ui->label_19, json["uper2"].toInt(), json["lower2"].toInt(), json["label2"].toString());
        hubParseHelper(ui->pushButton_16, QPoint(90, 190), QPoint(37, 190), json["big3"].toInt(), ui->pushButton_25, ui->comboBox_5,
                       ui->comboBox_4, ui->label_18, json["uper3"].toInt(), json["lower3"].toInt(), json["label3"].toString());

        ui->lineEdit->setText(json["label1"].toString());
    }
    else if(json["lower2"].toInt() == 2){
        QIcon ic4(":/img/PNG-cards-1.3/cross.png");
        ui->pushButton_17->setIcon(ic4);
        ui->pushButton_17->move(477, 190);
        ui->comboBox_7->setCurrentIndex(1);
        ui->comboBox_7->show();
        ui->label_19->setText(json["host"].toString());
        ui->label_19->show();
        ui->pushButton_26->show();

        hubParseHelper(ui->pushButton_16, QPoint(90, 190), QPoint(37, 190), json["big1"].toInt(), ui->pushButton_25, ui->comboBox_5,
                       ui->comboBox_4, ui->label_18, json["uper1"].toInt(), json["lower1"].toInt(), json["label1"].toString());
        hubParseHelper(ui->pushButton_15, QPoint(310, 30), QPoint(257, 30), json["big3"].toInt(), ui->pushButton_24, ui->comboBox_2,
                       ui->comboBox_3, ui->label_17, json["uper3"].toInt(), json["lower3"].toInt(), json["label3"].toString());

        ui->lineEdit->setText(json["label2"].toString());
    }
    else{
        QIcon ic4(":/img/PNG-cards-1.3/cross.png");
        ui->pushButton_16->setIcon(ic4);
        ui->pushButton_16->move(37, 190);
        ui->comboBox_5->setCurrentIndex(1);
        ui->comboBox_5->show();
        ui->label_18->setText(json["host"].toString());
        ui->label_18->show();
        ui->pushButton_25->show();

        hubParseHelper(ui->pushButton_15, QPoint(310, 30), QPoint(257, 30), json["big2"].toInt(), ui->pushButton_24, ui->comboBox_2,
                       ui->comboBox_3, ui->label_17, json["uper2"].toInt(), json["lower2"].toInt(), json["label2"].toString());
        hubParseHelper(ui->pushButton_17, QPoint(530, 190), QPoint(477, 190), json["big1"].toInt(), ui->pushButton_26, ui->comboBox_7,
                       ui->comboBox_6, ui->label_19, json["uper1"].toInt(), json["lower1"].toInt(), json["label1"].toString());

        ui->lineEdit->setText(json["label3"].toString());
    }
    if(json["points"].toBool())
        ui->label_25->setText("Point Limit: 225");
    else
        ui->label_25->setText("Point Limit: 125");

    if(json["QSM"].toBool())
        ui->label_26->setText("Queen of Spades Mode: On");
    else
        ui->label_26->setText("Queen of Spades Mode: Off");

    ui->comboBox_2->blockSignals(false);
    ui->comboBox_3->blockSignals(false);
    ui->comboBox_4->blockSignals(false);
    ui->comboBox_5->blockSignals(false);
    ui->comboBox_6->blockSignals(false);
    ui->comboBox_7->blockSignals(false);
}

void MainWindow::parseSwap(QLabel *label, QString &button)
{
    ui->comboBox_4->blockSignals(true);
    ui->comboBox_5->blockSignals(true);
    ui->comboBox_6->blockSignals(true);
    ui->comboBox_7->blockSignals(true);

    if(label == ui->label_17){
        if(button == "1"){
            swapCombobox(ui->comboBox_5, ui->comboBox_7, ui->comboBox_4, ui->comboBox_6, ui->pushButton_16, ui->pushButton_17, ui->label_18,
                         ui->label_19, ui->pushButton_22, ui->pushButton_23, ui->pushButton_25, ui->pushButton_26, 90, 190, 37, 190, 530, 190, 477, 190);
        }
        else if(button == "2"){
            swapCombobox(ui->comboBox_7, ui->comboBox_2, ui->comboBox_6, ui->comboBox_3, ui->pushButton_17, ui->pushButton_15, ui->label_19,
                         ui->label_17, ui->pushButton_23, ui->pushButton_21, ui->pushButton_26, ui->pushButton_24, 530, 190, 477, 190, 310, 30, 257, 30);
        }
        else if(button == "3"){
            swapCombobox(ui->comboBox_5, ui->comboBox_2, ui->comboBox_4, ui->comboBox_3, ui->pushButton_16, ui->pushButton_15, ui->label_18,
                         ui->label_17, ui->pushButton_22, ui->pushButton_21, ui->pushButton_25, ui->pushButton_24, 90, 190, 37, 190, 310, 30, 257, 30);
        }
    }
    else if(label == ui->label_18){
        if(button == "1"){
            swapCombobox(ui->comboBox_5, ui->comboBox_7, ui->comboBox_4, ui->comboBox_6, ui->pushButton_16, ui->pushButton_17, ui->label_18,
                         ui->label_19, ui->pushButton_22, ui->pushButton_23, ui->pushButton_25, ui->pushButton_26, 90, 190, 37, 190, 530, 190, 477, 190);
        }
        else if(button == "2"){
            swapCombobox(ui->comboBox_5, ui->comboBox_2, ui->comboBox_4, ui->comboBox_3, ui->pushButton_16, ui->pushButton_15, ui->label_18,
                         ui->label_17, ui->pushButton_22, ui->pushButton_21, ui->pushButton_25, ui->pushButton_24, 90, 190, 37, 190, 310, 30, 257, 30);
        }
        else if(button == "3"){
            swapCombobox(ui->comboBox_5, ui->comboBox_7, ui->comboBox_4, ui->comboBox_6, ui->pushButton_16, ui->pushButton_17, ui->label_18,
                         ui->label_19, ui->pushButton_22, ui->pushButton_23, ui->pushButton_25, ui->pushButton_26, 90, 190, 37, 190, 530, 190, 477, 190);
            swapCombobox(ui->comboBox_5, ui->comboBox_2, ui->comboBox_4, ui->comboBox_3, ui->pushButton_16, ui->pushButton_15, ui->label_18,
                         ui->label_17, ui->pushButton_22, ui->pushButton_21, ui->pushButton_25, ui->pushButton_24, 90, 190, 37, 190, 310, 30, 257, 30);
        }
    }
    else if(label == ui->label_19){
        if(button == "1"){
            swapCombobox(ui->comboBox_5, ui->comboBox_7, ui->comboBox_4, ui->comboBox_6, ui->pushButton_16, ui->pushButton_17, ui->label_18,
                         ui->label_19, ui->pushButton_22, ui->pushButton_23, ui->pushButton_25, ui->pushButton_26, 90, 190, 37, 190, 530, 190, 477, 190);
        }
        else if(button == "2"){
            swapCombobox(ui->comboBox_5, ui->comboBox_7, ui->comboBox_4, ui->comboBox_6, ui->pushButton_16, ui->pushButton_17, ui->label_18,
                         ui->label_19, ui->pushButton_22, ui->pushButton_23, ui->pushButton_25, ui->pushButton_26, 90, 190, 37, 190, 530, 190, 477, 190);
            swapCombobox(ui->comboBox_7, ui->comboBox_2, ui->comboBox_6, ui->comboBox_3, ui->pushButton_17, ui->pushButton_15, ui->label_19,
                         ui->label_17, ui->pushButton_23, ui->pushButton_21, ui->pushButton_26, ui->pushButton_24, 530, 190, 477, 190, 310, 30, 257, 30);
        }
        else if(button == "3"){
            swapCombobox(ui->comboBox_7, ui->comboBox_2, ui->comboBox_6, ui->comboBox_3, ui->pushButton_17, ui->pushButton_15, ui->label_19,
                         ui->label_17, ui->pushButton_23, ui->pushButton_21, ui->pushButton_26, ui->pushButton_24, 530, 190, 477, 190, 310, 30, 257, 30);
        }
    }

    ui->comboBox_4->blockSignals(false);
    ui->comboBox_5->blockSignals(false);
    ui->comboBox_6->blockSignals(false);
    ui->comboBox_7->blockSignals(false);

    server->HubChanged();
}

void MainWindow::hubParseHelper(QPushButton *bigButton, QPoint firstCords, QPoint secondCords, int jsonBig, QPushButton *swap, QComboBox *upper,
                    QComboBox *lower, QLabel *label, int jsonUpper, int jsonLower, const QString &jsonLabel)
{
    if(jsonBig == 90 || jsonBig == 530 || jsonBig == 310){
        QIcon ic4(":/img/PNG-cards-1.3/plus.png");
        bigButton->setIcon(ic4);
        bigButton->move(firstCords);
    }
    else{
        QIcon ic4(":/img/PNG-cards-1.3/cross.png");
        bigButton->setIcon(ic4);
        bigButton->move(secondCords);
        swap->show();
        upper->setCurrentIndex(jsonUpper);
        upper->show();
        if(jsonUpper == 0){
            lower->setCurrentIndex(jsonLower);
            lower->show();
        }
        else{
            if(jsonLabel != ""){
                label->setText(jsonLabel);
                label->show();
            }
        }
    }
}

void MainWindow::startCheck(int index)
{
    if(index == 1){
        ui->pushButton_18->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 20px; color: white; font: bold;");
        ui->pushButton_18->setDisabled(true);
    }
    else if((ui->comboBox_2->currentIndex() == 0 || !ui->pushButton_21->isHidden() || ui->pushButton_15->pos().x() == 310) &&
               (ui->comboBox_5->currentIndex() == 0 || !ui->pushButton_22->isHidden() || ui->pushButton_16->pos().x() == 90) &&
               (ui->comboBox_7->currentIndex() == 0 || !ui->pushButton_23->isHidden() || ui->pushButton_17->pos().x() == 530) &&
               ui->lineEdit->text() != ""){
        ui->pushButton_18->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 2px solid; border-color: black; border-radius: 20px; background-color: white; color: black; font: bold;");
        ui->pushButton_18->setDisabled(false);
    }
    else{
        ui->pushButton_18->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 20px; color: white; font: bold;");
        ui->pushButton_18->setDisabled(true);
    }
}

int MainWindow::getPlayercardsSize() const
{
    int playerSize = 32;
    for(Player* playe : players){
        if(playe->getCardsSize() < playerSize || playe->isInGame())
            playerSize = playe->getCardsSize();
    }
    return playerSize;
}

void MainWindow::hideLable3()
{
    ui->label_3->hide();
}

void MainWindow::lable3Style(QString param)
{
    ui->label_3->setStyleSheet(param);
    ui->label_3->show();
}

void MainWindow::botNoChoice()
{
    secMove = 0;
    Mmove = nextOne(Mmove);
    if(Mmove == 1){
        ui->label_2->setText("Your turn");
        ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
    }
    else {
        ui->label_2->setText(QString::fromStdString(players[Mmove - 1]->getName()) + "\nturn");
        ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
    }
    if(isFullscreen && isInGame){
        QString cs = ui->label_2->styleSheet();
        ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
    }

    for(Player* playe : players){
        playe->setCheckForTake(0);
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
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

void MainWindow::pushButtonBridge()
{
    if(Bridge){
        ui->pushButton->show();
        ui->pushButton_6->show();
        if(isFullscreen){
            ui->pushButton_6->resize(QSize(81 * scaleFactor, 16 * scaleFactor));
            ui->pushButton_6->move(QPoint(ui->pushButton->pos().x(), ui->pushButton->pos().y() - (5 * scaleFactor)));
            ui->pushButton->resize(QSize(ui->pushButton->size().width(), ui->pushButton->size().height() - (15 * scaleFactor)));
            ui->pushButton->move(QPoint(ui->pushButton->pos().x(), ui->pushButton->pos().y() + (14 * scaleFactor)));
        }
        else{
            ui->pushButton_6->resize(QSize(81, 16));
            ui->pushButton_6->move(QPoint(ui->pushButton->pos().x(), ui->pushButton->pos().y() - 5));
            ui->pushButton->resize(QSize(ui->pushButton->size().width(), ui->pushButton->size().height() - 15));
            ui->pushButton->move(QPoint(ui->pushButton->pos().x(), ui->pushButton->pos().y() + 14));
            qDebug() << ui->pushButton->geometry();
        }
    }
    else{
        ui->pushButton->hide();
        ui->pushButton_6->hide();
        if(isFullscreen){
            ui->pushButton->resize(QSize(ui->pushButton->size().width(), ui->pushButton->size().height() + (15 * scaleFactor)));
            ui->pushButton->move(QPoint(ui->pushButton->pos().x(), ui->pushButton->pos().y() - (14 * scaleFactor)));
        }
        else{
            ui->pushButton->resize(QSize(ui->pushButton->size().width(), ui->pushButton->size().height() + 15));
            ui->pushButton->move(QPoint(ui->pushButton->pos().x(), ui->pushButton->pos().y() - 14));
        }
    }
}

void MainWindow::botMove()
{
    if(dynamic_cast<Bot*>(players[Mmove - 1])){
        if(speedUp){
            timer->start(100);
        }
        else{
            timer->start(1000);
        }

        if(players[Mmove - 1]->getDifficulty() == "Hard"){
            players[Mmove - 1]->chooseBestMove();
        }
        else{
            players[Mmove - 1]->botMove();
        }
        for(int i = 0; i < players[1]->getCardsSize(); i++){
            qDebug() << players[1]->getCards()[i] << " ";
        }
        qDebug() << "\n";
    }
    else{
        if(speedUp){
            timer->start(100);
        }
        else{
            timer->start(2000);
        }
    }
}

void MainWindow::onAddWidgetPlayer(Player* pl, std::string card, int iter)
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
    QHBoxLayout* layout = qobject_cast<QHBoxLayout*>( pl->getLayout()->layout());
    QPushButton* (&playerButtons)[36] = pl->getButtons();
    playerButtons[iter] = new QPushButton("", pl->getLayout());
    layout->insertWidget(0,playerButtons[iter]);
    playerButtons[iter]->setIcon(ButtonIcon);
    QSize s(70,101);
    if(isFullscreen)
        s = QSize(70*scaleFactor, 101*scaleFactor);

    playerButtons[iter]->setIconSize(s);
    playerButtons[iter]->setFixedSize(s);
    playerButtons[iter]->setCursor(Qt::PointingHandCursor);
    playerButtons[iter]->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "background: none;"
        "}"
        );
    QObject::connect(playerButtons[iter], &QPushButton::clicked, this, &MainWindow::onRemoveWidgetPlayer);
    if(isFullscreen)
        ui->scrollAreaWidgetContents->setMinimumSize(QSize((70*scaleFactor)*(iter + 1),0));
    else
        ui->scrollAreaWidgetContents->setMinimumSize(QSize(70*(iter + 1),0));
}

void MainWindow::onAddWidgetBot(Player* pl, int iter)
{
    bool checker = false;
    if(players.size() > 2){
        if((pl == players[2] && players.size() == 3) || ((pl == players[1] || pl == players[3]) && players.size() == 4)){
            checker = true;
        }
    }
    QPushButton* (&botButtons)[36] = pl->getButtons();
    if(checker){
        QPixmap pix(":/img/PNG-cards-1.3/card_back_reverse.png");
        QIcon ButtonIcon(pix);
        QVBoxLayout* layout = qobject_cast<QVBoxLayout*>( pl->getLayout()->layout());
        botButtons[iter] = new QPushButton("", pl->getLayout());
        layout->insertWidget(0,botButtons[iter]);
        botButtons[iter]->setIcon(ButtonIcon);
        layout->invalidate();
        layout->update();

        QSize s(101,70);
        if(isFullscreen)
            s = QSize(101*scaleFactor, 70*scaleFactor);

        botButtons[iter]->setIconSize(s);
        botButtons[iter]->setFixedSize(s);

        botButtons[iter]->setStyleSheet(
            "QPushButton {"
            "border: none;"
            "background: none;"
            "}"
            );
    }
    else{
        QPixmap pix(":/img/PNG-cards-1.3/card_back.jfif");
        QIcon ButtonIcon(pix);
        QHBoxLayout* layout = qobject_cast<QHBoxLayout*>( pl->getLayout()->layout());
        botButtons[iter] = new QPushButton("", pl->getLayout());
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
    ColodButtons[iter]->setCursor(Qt::PointingHandCursor);
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
    if(dynamic_cast<Human*>(players[Mmove - 1])){
        QPushButton* button = qobject_cast<QPushButton*>(sender());
        bool maincheck = false;
        int mv;
        int playerCardsSize = players[Mmove - 1]->getCardsSize();
        std::string (&playerCards)[36] = players[Mmove - 1]->getCards();
        QPushButton* (&playerButtons)[36] = players[Mmove - 1]->getButtons();

        for(int i = 0; i < playerCardsSize; i++){
            bool check;
            if(Jackchoose == ""){
                check = possibleMove(playerCards[i], tableCards[tableCardsSize - 1], secMove);
            }
            else{
                check = possibleMove(playerCards[i], "0" + Jackchoose, 0);
            }
            if(button == playerButtons[i] && check == true){
                cardSound();

                if(Bridge){
                    Bridge = false;
                    pushButtonBridge();
                }

                Jackchoose = "";
                maincheck = true;
                ui->pushButton->hide();
                ui->pushButton_2->hide();
                ui->pushButton_3->hide();
                ui->pushButton_4->hide();
                ui->pushButton_5->hide();
                ui->label_3->hide();

                if(playerCards[i][0] == 'J'){
                    players[Mmove - 1]->setJackKol(players[Mmove - 1]->getJackKol() + 1);
                }
                else{
                    players[Mmove - 1]->setJackKol(0);
                }
                tableCards[tableCardsSize] = playerCards[i];
                onAddWidgetTable(tableCards[tableCardsSize], tableCardsSize);
                tableCardsSize++;
                delete playerButtons[i];
                playerButtons[i] = nullptr;
                playerCards[i] = "";
                mv = Mmove;
                for(int j = i; j < playerCardsSize - 1; j++){
                    onAddWidgetPlayer(players[Mmove - 1], playerCards[j + 1], j);
                    delete playerButtons[j + 1];
                    playerButtons[j + 1] = nullptr;
                    playerCards[j] = playerCards[j + 1];
                    playerCards[j + 1] = "";
                }
                players[Mmove - 1]->setCardsSize(playerCardsSize - 1);
                break;
            }
        }
        if(maincheck == true){
            ui->label_3->hide();
            for(Player* playe : players){
                playe->setCheckForTake(0);
            }
            if(playerCardsSize > 0){
                secondmove();
                operation(mv);
            }
            if(tableCards[tableCardsSize - 1][0] != '6' && !Bridge){
                gameEnd();
            }
        }

        if(isFullscreen && Mmove == 1)
            ui->scrollAreaWidgetContents->setMinimumSize(QSize((70*scaleFactor)*playerCardsSize,0));
        else if(Mmove == 1)
            ui->scrollAreaWidgetContents->setMinimumSize(QSize(70*playerCardsSize,0));

        AutoSave();
    }
}

void MainWindow::onRemoveWidgetColod()
{   
    if(Mmove == 1 && players[0]->getCheckForTake() == 0){

        int playerCardsSize = players[0]->getCardsSize();

        if(tableCards[tableCardsSize - 1][0] == '6' || secMove == 0){

            if(Bridge){
                Bridge = false;
                pushButtonBridge();
            }

            std::string (&playerCards)[36] = players[0]->getCards();

            if(ColodCardsSize == 0){
                shuffling();
            }

            cardSound();

            playerCards[playerCardsSize] = ColodCards[ColodCardsSize - 1];
            onAddWidgetPlayer(players[0], playerCards[playerCardsSize], playerCardsSize);
            players[0]->setCardsSize(++playerCardsSize);
            ColodCards[ColodCardsSize - 1] = "";
            delete ColodButtons[ColodCardsSize - 1];
            ColodButtons[ColodCardsSize - 1] = nullptr;
            ColodCardsSize--;
            ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));
            if(tableCards[tableCardsSize - 1][0] != '6'){
                ui->pushButton->show();
                players[0]->setCheckForTake(1);
            }
            if(ColodCardsSize == 0){
                onAddWidgetColod(0);
                isShuffl = true;
                ui->label_4->setText(QString::fromStdString("Shuffling (" + std::to_string(PointsX + 1) + "x)"));
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
    if(Mmove != 1 && players[Mmove - 1]->getCheckForTake() == 0){
        if(tableCards[tableCardsSize - 1][0] == '6' || secMove == 0){

            int botCardsSize = players[Mmove - 1]->getCardsSize();

            if(ColodCardsSize == 0){
                shuffling();
            }

            cardSound();

            players[Mmove - 1]->getCards()[botCardsSize] = ColodCards[ColodCardsSize - 1];
            onAddWidgetBot(players[Mmove - 1], botCardsSize);
            players[Mmove - 1]->setCardsSize(botCardsSize + 1);
            ColodCards[ColodCardsSize - 1] = "";
            delete ColodButtons[ColodCardsSize - 1];
            ColodButtons[ColodCardsSize - 1] = nullptr;
            ColodCardsSize--;
            ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));
            if(tableCards[tableCardsSize - 1][0] != '6'){
                players[Mmove - 1]->setCheckForTake(1);
            }
            if(ColodCardsSize == 0){
                onAddWidgetColod(0);
                isShuffl = true;
                ui->label_4->setText(QString::fromStdString("Shuffling (" + std::to_string(PointsX + 1) + "x)"));
                ui->label_4->show();
            }
        }
    }
    AutoSave();
}

void MainWindow::shuffling()
{
    isShuffl = false;
    cardSound();

    delete ColodButtons[0];
    ColodButtons[0] = nullptr;

    for(int i = 0; i < tableCardsSize; i++){
        delete TableButtons[i];
        TableButtons[i] = nullptr;
    }

    for(int i = 0; i < tableCardsSize - 1; i++){
        ColodCards[i] = tableCards[i];
        onAddWidgetColod(i);
        tableCards[i] = "";
    }
    ColodCardsSize = tableCardsSize - 1;

    tableCards[0] = tableCards[tableCardsSize - 1];
    tableCards[tableCardsSize - 1] = "";
    tableCardsSize = 1;
    onAddWidgetTable(tableCards[0], 0);

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(std::begin(ColodCards), std::begin(ColodCards) + ColodCardsSize, g);

    PointsX++;
    ui->label_7->setText(QString::fromStdString("Points " + std::to_string(PointsX) + "x"));
    ui->label_4->hide();
}

void MainWindow::cleaner(bool sh)
{
    for(Player* playe : players){
        playe->deleteButtons();
        playe->setCheckForTake(0);
        playe->setJackKol(0);
        playe->setPass(0);
    }

    if(ColodCardsSize == 0 && sh){
        delete ColodButtons[0];
        ColodButtons[0] = nullptr;
    }
    for(int i = 0; i < ColodCardsSize; i++){
        delete ColodButtons[i];
        ColodButtons[i] = nullptr;
        ColodCards[i] = "";
    }
    for(int i = 0; i < tableCardsSize; i++){
        delete TableButtons[i];
        TableButtons[i] = nullptr;
        tableCards[i] = "";
    }
    ColodCardsSize = 0;
    tableCardsSize = 0;

    isShuffl = 0;
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
    ui->label_14->hide();
    ui->label_15->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_8->hide();
    ui->label_11->hide();
    ui->label_12->hide();
    ui->label_13->hide();
    ui->scrollArea->hide();
    ui->label_7->setText(QString::fromStdString("Points " + std::to_string(PointsX) + "x"));
}

void MainWindow::gameEnd()
{
    std::vector<Player*> losers;
    int losersCounter = 0;
    bool cheker = false;
    int index = 0;
    for(Player* playe : players){
        if(playe->getCardsSize() == 0 && playe->isInGame()){
            cheker = true;
            break;
        }
        index++;
    }
    if(cheker || Bridge){
        bool checkForPass = false;
        for(Player* playe : players){
            if(!playe->isInGame()){
                losers.push_back(playe);
            }
            else if(playe->getPass() > 0){
                checkForPass = true;
                endClicked(true);
            }
        }
        if(!checkForPass && (tableCards[tableCardsSize - 1][0] == '8' || tableCards[tableCardsSize - 1][0] == '7' || (tableCards[tableCardsSize - 1] == "Qp" && QSMode)))
            operation(Mmove);

        if(!Bridge){
            int pJackKol = players[index]->getJackKol();
            if(pJackKol != 0){
                players[index]->setPoints(players[index]->getPoints() - ((20 * pJackKol) * PointsX));
            }
        }
        else{
            index = 5;
        }

        int ind = 0;

        for(Player* playe : players){
            if(ind != index && std::count(losers.begin(), losers.end(), playe) == 0){
                std::string (&botCards)[36] = playe->getCards();

                int finalPoints = playe->getPoints();

                for(int i = 0; i < playe->getCardsSize(); i++){
                    if(botCards[i][0] == 'A'){
                        finalPoints += (15 * PointsX);
                    }
                    else if(botCards[i][0] == 'J'){
                        finalPoints += (20 * PointsX);
                    }
                    else if(QSMode && botCards[i] == "Qp"){
                        finalPoints += (50 * PointsX);
                    }
                    else if(botCards[i][0] == 'K' || botCards[i][0] == 'Q' || botCards[i][0] == '1'){
                        finalPoints += (10 * PointsX);
                    }
                }

                if(finalPoints == 225 && PointsMode){
                    playe->setPoints(0);
                    finalPoints = 0;
                }
                else if(finalPoints == 125){
                    playe->setPoints(0);
                    finalPoints = 0;
                }
                else{
                    playe->setPoints(finalPoints);
                }

                if(finalPoints > 225 && PointsMode){
                    playe->setInGame(false);
                    losers.push_back(playe);
                    losersCounter++;
                    playersCount--;
                }
                else if(finalPoints > 125 && !PointsMode){
                    playe->setInGame(false);
                    losers.push_back(playe);
                    losersCounter++;
                    playersCount--;
                }
            }
            ind++;
        }

        QMessageBox msgBox(this);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setStyleSheet(massBoxStyle);

        foreach (QPushButton *button, msgBox.findChildren<QPushButton*>()) {
            button->setCursor(Qt::PointingHandCursor);
        }

        std::string massage = "";

        if(Bridge){
            int i = 0;
            std::pair<int, int> temp = {INT_MAX, 0};
            for(Player* playe : players){
                if(playe->getPoints() < temp.first){
                    temp = {playe->getPoints(), i};
                }

                ++i;
            }
            index = temp.second;
        }

        if(losers.size() + 1 == players.size() || losers.size() == players.size()){
            isInGame = false;

            delete timer;
            msgBox.setWindowTitle("Game over");

            if(index != 0){
                massage = "Unfortunately. You lost!\nThe Winner is " + players[index]->getName() + "! Points: "
                                      + std::to_string(players[index]->getPoints()) + "\nYour points: " + std::to_string(players[0]->getPoints());
                for(int i = 1; i < players.size(); i++){
                    if(i != index){
                        massage = massage + "\n" + players[i]->getName() + " points: " + std::to_string(players[i]->getPoints());
                    }
                }
            }
            else{
                massage = "Congratulations! You won!!\nThe Winner is " + players[index]->getName() + "! Points: "
                                      + std::to_string(players[index]->getPoints());

                for(int i = 1; i < players.size(); i++){
                    massage = massage + "\n" + players[i]->getName() + " points: " + std::to_string(players[i]->getPoints());
                }
            }

            if(Bridge)
                massage = "Bridge!\n" + massage;

            Bridge = false;
            if(!ui->pushButton_6->isHidden())
                pushButtonBridge();

            msgBox.setText(QString::fromStdString(massage));
            msgBox.exec();
            clickedSound();

            if(server){
                delete server;
                server = nullptr;
            }

            cleaner();

            for (Player* playe : players) {
                delete playe;
            }
            players.clear();

            ui->pushButton_12->show();
            ui->pushButton_13->show();
            ui->pushButton_7->show();
            ui->pushButton_8->show();
            ui->pushButton_14->hide();
            speedUp = false;
            ui->pushButton_14->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 10px; background-color: rgb(230,230,230); color: black; font: bold;");
            ui->label_11->hide();
            ui->label_12->hide();
            ui->label_13->hide();
            ui->scrollArea->hide();
            ui->label_9->show();
            ui->label_10->show();
            ui->label_16->show();
            ui->line->show();
            ui->line_2->show();
            std::ofstream fout;
            fout.open(dirGame.toStdString());
            fout.close();

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

            bool check = false;
            for(int i = 0; i < losersCounter; i++){
                if(losers[losers.size() - (i + 1)] == players[0]){
                    massage = "Unfortunately. You lost!\nYour points: " + std::to_string(players[0]->getPoints());
                    check = true;
                    break;
                }
            }
            if(check){
                for(int i = 1; i < players.size(); i++){
                    massage = massage + "\n" + players[i]->getName() + " points: " + std::to_string(players[i]->getPoints());
                }
            }
            else{
                massage = "Your points: " + std::to_string(players[0]->getPoints());
                for(int i = 1; i < players.size(); i++){
                    massage = massage + "\n" + players[i]->getName() + " points: " + std::to_string(players[i]->getPoints());
                }
            }

            if(Bridge)
                massage = "Bridge!\n" + massage;

            Bridge = false;
            if(!ui->pushButton_6->isHidden())
                pushButtonBridge();

            msgBox.setText(QString::fromStdString(massage));
            msgBox.exec();

            clickedSound();

            cleaner();

            bool checkForHuman = false;

            for(Player* playe : players){
                if (playe->getDifficulty() == "Middle"){
                    if(players.size() == 2){
                        playe->getLabel()->setText(QString::fromStdString(playe->getName() + " | Points: " + std::to_string(playe->getPoints())));
                    }
                    else{
                        playe->getLabel()->setText("<html><span style='color: orange; font-size: 11pt; font-weight: 900; font-family: 'Segoe UI Black';'>M</span> "
                                                   + QString::fromStdString(playe->getName() + " | Points: " + std::to_string(playe->getPoints()) + "</html>"));
                    }
                }
                else if (playe->getDifficulty() == "Hard"){
                    if(players.size() == 2){
                        playe->getLabel()->setText(QString::fromStdString(playe->getName() + " | Points: " + std::to_string(playe->getPoints())));
                    }
                    else{
                        playe->getLabel()->setText("<html><span style='color: red; font-size: 11pt; font-weight: 900; font-family: 'Segoe UI Black';'>H</span> "
                                                   + QString::fromStdString(playe->getName() + " | Points: " + std::to_string(playe->getPoints()) + "</html>"));
                    }
                }
                else{
                    if(playe->isInGame()){
                        checkForHuman = true;
                    }
                    playe->getLabel()->setText(QString::fromStdString(playe->getName() + " | Points: " + std::to_string(playe->getPoints())));
                }

                playe->getLabel()->show();
            }

            players[0]->getLabel()->setText(QString::fromStdString("Your Points: " + std::to_string(players[0]->getPoints())));

            if(!checkForHuman)
                ui->pushButton_14->show();

            ui->label->setVisible(true);
            ui->label_2->setVisible(true);
            ui->label_7->setVisible(true);
            ui->label_8->setVisible(true);
            ui->pushButton_10->setVisible(true);
            ui->pushButton_11->setVisible(true);

            if(QSMode)
                ui->label_11->setVisible(true);

            ui->label_12->show();

            if(players.size() == 2)
                ui->label_13->show();

            ui->scrollArea->show();

            Set++;
            ui->label_8->setText(QString::fromStdString("Set: " + std::to_string(Set)));

            Start();
            int mv = Mmove;
            if(dynamic_cast<Bot*>(players[Mmove - 1]) && tableCards[0][0] == 'J'){
                std::string (&botCards)[36] = players[Mmove - 1]->getCards();

                int sign[4];
                sign[0] = 0;
                sign[1] = 0;
                sign[2] = 0;
                sign[3] = 0;
                for(int i = 0; i < players[Mmove - 1]->getCardsSize(); i++){
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

            if(isFullscreen){
                QString curText = ui->label_6->text();
                int newFontSize = 11 * scaleFactor;
                QRegularExpression regex("font-size:\\s*\\d+pt");
                curText.replace(regex, QString("font-size: %1pt").arg(newFontSize));
                ui->label_6->setText(curText);
                curText = ui->label_14->text();
                curText.replace(regex, QString("font-size: %1pt").arg(newFontSize));
                ui->label_14->setText(curText);
                curText = ui->label_15->text();
                curText.replace(regex, QString("font-size: %1pt").arg(newFontSize));
                ui->label_15->setText(curText);
            }

            timer = new QTimer();
            connect(timer, SIGNAL(timeout()), this, SLOT(botMove()));
            if(speedUp){
                timer->start(100);
            }
            else{
                timer->start(2000);
            }

            AutoSave();
        }
    }
    else{
        AutoSave();
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

int MainWindow::nextOne(int mv){
    int next;
    if(mv < players.size()){
        next = mv + 1;
    }
    else{
        next = 1;
    }

    while(!players[next - 1]->isInGame()){
        if(next < players.size()){
            next++;
        }
        else{
            next = 1;
        }
    }

    return next;
}

void MainWindow::operation(int mv)
{
    if(tableCards[tableCardsSize - 1] == "Qp" && QSMode == true){
        if(!secMove || playersCount == 2){
            secMove = 0;
            int next = nextOne(mv);

            std::string (&Cards)[36] = players[next - 1]->getCards();
            int CardsSize = players[next - 1]->getCardsSize();

            if(Bridge && Mmove == 1){
                if(isFullscreen){
                    ui->pushButton_6->resize(QSize(81 * scaleFactor,31 * scaleFactor));
                    ui->pushButton_6->move(QPoint(ui->pushButton_6->pos().x(), ui->pushButton_6->pos().y() + (5 * scaleFactor)));
                }
                else{
                    ui->pushButton_6->resize(QSize(81,31));
                    ui->pushButton_6->move(QPoint(ui->pushButton_6->pos().x(), ui->pushButton_6->pos().y() + 5));
                }
            }

            if(next != 1){
                for(int i = 0; i < 5; i++){
                    if(ColodCardsSize == 0){
                        if(i != 0){
                            onAddWidgetColod(0);
                        }
                        shuffling();
                    }
                    Cards[CardsSize] = ColodCards[ColodCardsSize - 1];
                    onAddWidgetBot(players[next - 1], CardsSize);
                    CardsSize++;
                    players[next - 1]->setCardsSize(CardsSize);
                    ColodCards[ColodCardsSize - 1] = "";
                    delete ColodButtons[ColodCardsSize - 1];
                    ColodButtons[ColodCardsSize - 1] = nullptr;
                    ColodCardsSize--;
                }
            }
            else{
                for(int i = 0; i < 5; i++){
                    if(ColodCardsSize == 0){
                        if(i != 0){
                            onAddWidgetColod(0);
                        }
                        shuffling();
                    }
                    Cards[CardsSize] = ColodCards[ColodCardsSize - 1];
                    onAddWidgetPlayer(players[0], Cards[CardsSize], CardsSize);
                    CardsSize++;
                    players[0]->setCardsSize(CardsSize);
                    ColodCards[ColodCardsSize - 1] = "";
                    delete ColodButtons[ColodCardsSize - 1];
                    ColodButtons[ColodCardsSize - 1] = nullptr;
                    ColodCardsSize--;
                }
            }

            next = nextOne(next);

            Mmove = next;
            if(Mmove == 1){
                ui->label_2->setText("Your turn");
                ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
            }
            else {
                ui->label_2->setText(QString::fromStdString(players[Mmove - 1]->getName()) + "\nturn");
                ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
            }
            if(isFullscreen && isInGame){
                QString cs = ui->label_2->styleSheet();
                ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
            }
            ui->pushButton->hide();

            if(ColodCardsSize == 0){
                onAddWidgetColod(0);
                isShuffl = true;
                ui->label_4->setText(QString::fromStdString("Shuffling (" + std::to_string(PointsX + 1) + "x)"));
                ui->label_4->show();
            }
            ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));
        }
        else {
            players[mv - 1]->setPass(1);
        }
    }
    else {
        char card = tableCards[tableCardsSize - 1][0];

        int next;

        switch (card) {
        case 'A':     
            if(playersCount == players[mv - 1]->getPass() + 2){
                secMove = 0;
                Mmove = mv;
                if(mv == 1){
                    ui->label_2->setText("Your turn");
                    ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
                }
                else {
                    ui->label_2->setText(QString::fromStdString(players[mv - 1]->getName()) + "\nturn");
                    ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
                }
                if(isFullscreen && isInGame){
                    QString cs = ui->label_2->styleSheet();
                    ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
                }
                ui->pushButton->hide();
                if(Bridge && Mmove == 1){
                    if(isFullscreen){
                        ui->pushButton_6->resize(QSize(81 * scaleFactor,31 * scaleFactor));
                        ui->pushButton_6->move(QPoint(ui->pushButton_6->pos().x(), ui->pushButton_6->pos().y() + (5 * scaleFactor)));
                    }
                    else{
                        ui->pushButton_6->resize(QSize(81,31));
                        ui->pushButton_6->move(QPoint(ui->pushButton_6->pos().x(), ui->pushButton_6->pos().y() + 5));
                    }
                }
                players[mv - 1]->setPass(0);
                break;
            }
            else if(secMove){
                players[mv - 1]->setPass(players[mv - 1]->getPass() + 1);
                if(isFullscreen && isInGame){
                    QString cs = ui->label_2->styleSheet();
                    ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
                }
                break;
            }
            else if(!Bridge){
                for(int i = 0; i < players[mv - 1]->getPass() + 1; i++){
                    next = nextOne(mv + i);
                    if(i == players[mv - 1]->getPass()){
                        next = nextOne(next);
                        Mmove = next;
                        if(Mmove == 1){
                            ui->label_2->setText("Your turn");
                            ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
                        }
                        else {
                            ui->label_2->setText(QString::fromStdString(players[Mmove - 1]->getName()) + "\nturn");
                            ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
                        }
                        if(isFullscreen && isInGame){
                            QString cs = ui->label_2->styleSheet();
                            ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
                        }
                    }
                }
                players[mv - 1]->setPass(0);
                ui->pushButton->hide();
                break;
            }
            break;

        case '8':
            if(playersCount == players[mv - 1]->getPass() + 2){
                secMove = 0;
                Mmove = mv;
                if(mv == 1){
                    ui->label_2->setText("Your turn");
                    ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
                }
                else {
                    ui->label_2->setText(QString::fromStdString(players[mv - 1]->getName()) + "\nturn");
                    ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
                }
                if(isFullscreen && isInGame){
                    QString cs = ui->label_2->styleSheet();
                    ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
                }
                ui->pushButton->hide();
                if(Bridge && Mmove == 1){
                    if(isFullscreen){
                        ui->pushButton_6->resize(QSize(81 * scaleFactor,31 * scaleFactor));
                        ui->pushButton_6->move(QPoint(ui->pushButton_6->pos().x(), ui->pushButton_6->pos().y() + (5 * scaleFactor)));
                    }
                    else{
                        ui->pushButton_6->resize(QSize(81,31));
                        ui->pushButton_6->move(QPoint(ui->pushButton_6->pos().x(), ui->pushButton_6->pos().y() + 5));
                    }
                }
                players[mv - 1]->setPass(0);

                for(int i = 0; i < playersCount - 1; i++){
                    next = nextOne(mv + i);

                    std::string (&Cards)[36] = players[next - 1]->getCards();
                    int CardsSize = players[next - 1]->getCardsSize();

                    if (ColodCardsSize == 0) {
                        shuffling();
                    }

                    Cards[CardsSize] = ColodCards[ColodCardsSize - 1];
                    if(next != 1){
                        onAddWidgetBot(players[next - 1], CardsSize);
                    }
                    else{
                        onAddWidgetPlayer(players[0], Cards[CardsSize], CardsSize);
                    }
                    CardsSize++;
                    players[next - 1]->setCardsSize(CardsSize);
                    ColodCards[ColodCardsSize - 1] = "";
                    delete ColodButtons[ColodCardsSize - 1];
                    ColodButtons[ColodCardsSize - 1] = nullptr;
                    ColodCardsSize--;

                    if (ColodCardsSize == 0) {
                        onAddWidgetColod(0);
                        isShuffl = true;
                        ui->label_4->setText(QString::fromStdString("Shuffling (" + std::to_string(PointsX + 1) + "x)"));
                        ui->label_4->show();
                    }

                    if (ColodCardsSize == 0) {
                        shuffling();
                    }

                    Cards[CardsSize] = ColodCards[ColodCardsSize - 1];
                    if(next != 1){
                        onAddWidgetBot(players[next - 1], CardsSize);
                    }
                    else{
                        onAddWidgetPlayer(players[0], Cards[CardsSize], CardsSize);
                    }
                    CardsSize++;
                    players[next - 1]->setCardsSize(CardsSize);
                    ColodCards[ColodCardsSize - 1] = "";
                    delete ColodButtons[ColodCardsSize - 1];
                    ColodButtons[ColodCardsSize - 1] = nullptr;
                    ColodCardsSize--;

                    ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));

                    if (ColodCardsSize == 0) {
                        onAddWidgetColod(0);
                        isShuffl = true;
                        ui->label_4->setText(QString::fromStdString("Shuffling (" + std::to_string(PointsX + 1) + "x)"));
                        ui->label_4->show();
                    }
                }

                break;
            }
            else if(secMove){
                players[mv - 1]->setPass(players[mv - 1]->getPass() + 1);
                if(isFullscreen && isInGame){
                    QString cs = ui->label_2->styleSheet();
                    ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
                }
            }
            else if(!Bridge){
                for(int i = 0; i < players[mv - 1]->getPass() + 1; i++){
                    next = nextOne(mv + i);
                    std::string (&Cards)[36] = players[next - 1]->getCards();
                    int CardsSize = players[next - 1]->getCardsSize();

                    if (ColodCardsSize == 0) {
                        shuffling();
                    }

                    Cards[CardsSize] = ColodCards[ColodCardsSize - 1];
                    if(next != 1){
                        onAddWidgetBot(players[next - 1], CardsSize);
                    }
                    else{
                        onAddWidgetPlayer(players[0], Cards[CardsSize], CardsSize);
                    }
                    CardsSize++;
                    players[next - 1]->setCardsSize(CardsSize);
                    ColodCards[ColodCardsSize - 1] = "";
                    delete ColodButtons[ColodCardsSize - 1];
                    ColodButtons[ColodCardsSize - 1] = nullptr;
                    ColodCardsSize--;

                    if (ColodCardsSize == 0) {
                        onAddWidgetColod(0);
                        isShuffl = true;
                        ui->label_4->setText(QString::fromStdString("Shuffling (" + std::to_string(PointsX + 1) + "x)"));
                        ui->label_4->show();
                    }

                    if (ColodCardsSize == 0) {
                        shuffling();
                    }

                    Cards[CardsSize] = ColodCards[ColodCardsSize - 1];
                    if(next != 1){
                        onAddWidgetBot(players[next - 1], CardsSize);
                    }
                    else{
                        onAddWidgetPlayer(players[0], Cards[CardsSize], CardsSize);
                    }
                    CardsSize++;
                    players[next - 1]->setCardsSize(CardsSize);
                    ColodCards[ColodCardsSize - 1] = "";
                    delete ColodButtons[ColodCardsSize - 1];
                    ColodButtons[ColodCardsSize - 1] = nullptr;
                    ColodCardsSize--;

                    ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));

                    if (ColodCardsSize == 0) {
                        onAddWidgetColod(0);
                        isShuffl = true;
                        ui->label_4->setText(QString::fromStdString("Shuffling (" + std::to_string(PointsX + 1) + "x)"));
                        ui->label_4->show();
                    }

                    if(i == players[mv - 1]->getPass()){
                        next = nextOne(next);
                        Mmove = next;
                        if(Mmove == 1){
                            ui->label_2->setText("Your turn");
                            ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
                        }
                        else {
                            ui->label_2->setText(QString::fromStdString(players[Mmove - 1]->getName()) + "\nturn");
                            ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
                        }
                        if(isFullscreen && isInGame){
                            QString cs = ui->label_2->styleSheet();
                            ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
                        }
                    }
                }
                players[mv - 1]->setPass(0);
                ui->pushButton->hide();
                break;
            }
            break;
        case '7': {
            next = nextOne(mv);

            std::string (&Cards)[36] = players[next - 1]->getCards();
            int CardsSize = players[next - 1]->getCardsSize();
            if (ColodCardsSize == 0) {
                shuffling();
            }

            Cards[CardsSize] = ColodCards[ColodCardsSize - 1];
            if(next != 1){
                onAddWidgetBot(players[next - 1], CardsSize);
            }
            else{
                onAddWidgetPlayer(players[0], Cards[CardsSize], CardsSize);
            }
            CardsSize++;
            players[next - 1]->setCardsSize(CardsSize);
            ColodCards[ColodCardsSize - 1] = "";
            delete ColodButtons[ColodCardsSize - 1];
            ColodButtons[ColodCardsSize - 1] = nullptr;
            ColodCardsSize--;

            ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));

            if (ColodCardsSize == 0) {
                onAddWidgetColod(0);
                isShuffl = true;
                ui->label_4->setText(QString::fromStdString("Shuffling (" + std::to_string(PointsX + 1) + "x)"));
                ui->label_4->show();
            }
            break;
        }
        default:
            players[mv - 1]->setPass(0);
            break;
        }
    }
}

void MainWindow::secondmove()
{
    if(Mmove == 1){
        secMove = 0;
        Mmove = nextOne(1);
        ui->label_2->setText(QString::fromStdString(players[Mmove - 1]->getName()) + "\nturn");
        ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
        if(isFullscreen && isInGame){
            QString cs = ui->label_2->styleSheet();
            ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
        }
        if(tableCardsSize > 3 && tableCards[tableCardsSize - 1][0] != '6' && tableCards[tableCardsSize - 1][0] == tableCards[tableCardsSize - 2][0]
            && tableCards[tableCardsSize - 1][0] == tableCards[tableCardsSize - 3][0]
            && tableCards[tableCardsSize - 1][0] == tableCards[tableCardsSize - 4][0])
        {
            Bridge = true;
            pushButtonBridge();
        }
        for(int i = 0; i < players[0]->getCardsSize(); i++){
            if(players[0]->getCards()[i][0] == tableCards[tableCardsSize - 1][0] || tableCards[tableCardsSize - 1][0] == '6' || tableCards[tableCardsSize - 1][0] == 'J' || Bridge){
                secMove = 1;
                Mmove = 1;
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
                    ui->pushButton->hide();
                    if(Bridge){
                        if(isFullscreen){
                            ui->pushButton_6->resize(QSize(81 * scaleFactor,31 * scaleFactor));
                            ui->pushButton_6->move(QPoint(ui->pushButton_6->pos().x(), ui->pushButton_6->pos().y() + (5 * scaleFactor)));
                        }
                        else{
                            ui->pushButton_6->resize(QSize(81,31));
                            ui->pushButton_6->move(QPoint(ui->pushButton_6->pos().x(), ui->pushButton_6->pos().y() + 5));
                        }
                    }
                    ui->pushButton_2->show();
                    ui->pushButton_3->show();
                    ui->pushButton_4->show();
                    ui->pushButton_5->show();
                }
                break;
            }
        }
        if(Mmove != 1)
            players[0]->setJackKol(0);
    }
    else{
        int mv = Mmove;
        Mmove = nextOne(Mmove);

        if(Mmove == 1){
            ui->label_2->setText("Your turn");
            ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
        }
        else{
            ui->label_2->setText(QString::fromStdString(players[Mmove - 1]->getName()) + "\nturn");
            ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
        }
        secMove = 0;

        if(isFullscreen && isInGame){
            QString cs = ui->label_2->styleSheet();
            ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
        }

        if(tableCardsSize > 3 && tableCards[tableCardsSize - 1][0] != '6' && tableCards[tableCardsSize - 1][0] == tableCards[tableCardsSize - 2][0]
            && tableCards[tableCardsSize - 1][0] == tableCards[tableCardsSize - 3][0]
            && tableCards[tableCardsSize - 1][0] == tableCards[tableCardsSize - 4][0])
        {
            Bridge = true;
        }

        if(tableCards[tableCardsSize - 1][0] == '6'){
            Mmove = mv;
            ui->label_2->setText(QString::fromStdString(players[Mmove - 1]->getName()) + "\nturn");
            ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
            if(isFullscreen && isInGame){
                QString cs = ui->label_2->styleSheet();
                ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
            }
            players[Mmove - 1]->setCheckForTake(0);
        }
        else{
            for(int i = 0; i < players[mv - 1]->getCardsSize(); i++){
                if(players[mv - 1]->getCards()[i][0] == tableCards[tableCardsSize - 1][0] || Bridge){
                    secMove = 1;
                    Mmove = mv;
                    ui->label_2->setText(QString::fromStdString(players[Mmove - 1]->getName()) + "\nturn");
                    ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
                    if(isFullscreen && isInGame){
                        QString cs = ui->label_2->styleSheet();
                        ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
                    }
                    break;
                }
            }
        }
        if(mv != Mmove)
            players[mv - 1]->setJackKol(0);
    }
}

void MainWindow::Start()
{
    srand(time(NULL));
    Mmove = rand() % playersCount + 1;

    if(!players[Mmove - 1]->isInGame()){
        Mmove = nextOne(Mmove);
    }

    for (int suit = 0; suit < 4; suit++) {
        for (int rank = 0; rank < 9; rank++) {
            ColodCards[ColodCardsSize++] = cardNames[suit][rank];
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(std::begin(ColodCards), std::end(ColodCards), g);

    for(int i = 0; i < 5; i++){
        if(i != 4){
            int index = 0;
            for(Player* playe : players){
                if(playe->isInGame()){
                    playe->addCard(ColodCards[ColodCardsSize - 1]);

                    if(index == 0){
                        onAddWidgetPlayer(playe, ColodCards[ColodCardsSize - 1], i);
                    }
                    else{
                        onAddWidgetBot(playe, i);
                    }

                    ColodCards[--ColodCardsSize] = "";
                }
                index++;
            }
        }
        else{
            int counter = 1;
            for(Player* playe : players){
                if(playe->isInGame()){
                    if(counter == Mmove){
                        tableCards[0] = ColodCards[ColodCardsSize - 1];
                        tableCardsSize++;

                        ColodCards[--ColodCardsSize] = "";

                        onAddWidgetTable(tableCards[0], 0);
                    }
                    else{
                        playe->addCard(ColodCards[ColodCardsSize - 1]);

                        if(counter == 1){
                            onAddWidgetPlayer(playe, ColodCards[ColodCardsSize - 1], i);
                        }
                        else{
                            onAddWidgetBot(playe, i);
                        }

                        ColodCards[--ColodCardsSize] = "";
                    }
                }
                counter++;
            }
        }
    }

    for(int i = 0; i < ColodCardsSize; i++){
        onAddWidgetColod(i);
    }
    ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));
}

void MainWindow::AutoSave()
{
    std::ofstream fout;
    fout.open(dirGame.toStdString());

    if(fout.is_open()){

        fout << "Can_Resume\n";

        fout << players.size() << "\n";

        for(Player* playe : players){
            if(playe->getDifficulty() == ""){
                fout << "empty" << "\n";
            }
            else{
                fout << playe->getDifficulty() << "\n";
            }
        }

        fout << playersCount << "\n";

        for(Player* playe : players){
            int cardsSize = playe->getCardsSize();

            fout << playe->getPass() << "\n";
            fout << playe->isInGame() << "\n";

            fout << playe->getName() << "\n";

            fout << cardsSize << "\n";
            for(int i = 0; i < cardsSize; i++){
                fout << playe->getCards()[i] << "\n";
            }

            fout << playe->getCheckForTake() << "\n";
            fout << playe->getPoints() << "\n";
            fout << playe->getJackKol() << "\n";
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
        fout << PointsX << "\n";
        fout << Set << "\n";
        fout << ui->pushButton->isVisible() << "\n";
        fout << ui->pushButton_2->isVisible() << "\n";
        fout << ui->label_3->isVisible() << "\n";
        fout << QSMode << "\n";
        fout << PointsMode << "\n";
        fout << Bridge << "\n";
        fout << !ui->pushButton_14->isHidden();
    }
    fout.close();
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
            if(windowSize.width() <= (ui->horizontalLayoutWidget_2->width() + (ui->horizontalLayoutWidget_2->height() * 2) + 70 + ui->pushButton_10->width()) * scaleFactor){
                ui->horizontalLayoutWidget_2->setGeometry(savedLayout2Geo);
            }
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
            scaleWidget(ui->label_14);
            scaleFontnRadiusWidget(ui->label_14, 8, 0);
            scaleWidget(ui->label_15);
            scaleFontnRadiusWidget(ui->label_15, 8, 0);
            scaleWidget(ui->label_7);
            scaleFontnRadiusWidget(ui->label_7, 8, 3);
            scaleWidget(ui->label_8);
            scaleFontnRadiusWidget(ui->label_8, 8, 3);
            scaleWidget(ui->label_11);
            scaleWidget(ui->label_12);
            ui->label_12->setStyleSheet(QString("font-weight: %1; font-family: 'Segoe UI Black'; font-size: %2pt; color: #c5c5c5").arg(900 * scaleFactor).arg(11 * scaleFactor));
            scaleWidget(ui->label_13);
            if(!players.empty()){
                std::string dif = players[1]->getDifficulty();
                if(dif == "Middle"){
                    ui->label_13->setStyleSheet(QString("font-weight: %1; font-size: %2pt; font-family: 'Segoe UI Black'; color: orange").arg(900 * scaleFactor).arg(11 * scaleFactor));
                }
                else if(dif == "Hard"){
                    ui->label_13->setStyleSheet(QString("font-weight: %1; font-size: %2pt; font-family: 'Segoe UI Black'; color: red").arg(900 * scaleFactor).arg(11 * scaleFactor));
                }
            }
            scaleWidget(ui->pushButton);
            scaleFontnRadiusWidget(ui->pushButton, 8, 3);
            scaleFontnRadiusWidget(ui->pushButton_6, 8, 3);
            scaleWidget(ui->pushButton_2);
            scaleWidget(ui->pushButton_3);
            scaleWidget(ui->pushButton_4);
            scaleWidget(ui->pushButton_5);
            scaleWidget(ui->pushButton_10);
            scaleFontnRadiusWidget(ui->pushButton_10, 0, 10);
            scaleWidget(ui->pushButton_11);
            scaleFontnRadiusWidget(ui->pushButton_11, 0, 10);
            scaleWidget(ui->pushButton_14);
            scaleFontnRadiusWidget(ui->pushButton_14, 0, 10);
            QSize picSize = ui->pushButton_10->iconSize();
            ui->pushButton_10->setIconSize(QSize(picSize.width()*scaleFactor, picSize.height()*scaleFactor));
            picSize = ui->pushButton_11->iconSize();
            ui->pushButton_11->setIconSize(QSize(picSize.width()*scaleFactor, picSize.height()*scaleFactor));
            picSize = ui->pushButton_14->iconSize();
            ui->pushButton_14->setIconSize(QSize(picSize.width()*scaleFactor, picSize.height()*scaleFactor));
            scaleWidget(ui->horizontalLayoutWidget_2);
            scaleWidget(ui->horizontalLayoutWidget_3);
            scaleWidget(ui->horizontalLayoutWidget_4);
            scaleWidget(ui->scrollArea);
            scaleWidget(ui->horizontalLayoutWidget);
            scaleWidget(ui->verticalLayoutWidget);
            scaleWidget(ui->verticalLayoutWidget_2);
            QSize s(70*scaleFactor,101*scaleFactor);

            for(Player* playe : players){
                QPushButton* (&buttons)[36] = playe->getButtons();
                if((playe == players[2] && players.size() == 3) || ((playe == players[1] || playe == players[3]) && players.size() == 4)){
                    s = QSize(101*scaleFactor,70*scaleFactor);
                }
                else{
                    s = QSize(70*scaleFactor,101*scaleFactor);
                }
                for(int i = 0; i < playe->getCardsSize(); i++){
                    if(buttons[i]){
                        buttons[i]->setIconSize(s);
                        buttons[i]->setFixedSize(s);
                    }
                }
            }
            s = QSize(70*scaleFactor,101*scaleFactor);

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
            ui->centralwidget->resize(windowSize);
            WidgetsLocation(windowSize);
        }
        else{
            setUiGeo();
            if(Mmove != 1)
                ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
            else
                ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");

            if(label2Str != "")
                ui->label_2->setText(label2Str);

            ui->scrollAreaWidgetContents->setMinimumSize(QSize(70*players[0]->getCardsSize(),0));
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

    if(players.size() != 4){
        ui->label_2->move(curWidgetPos.x() * scaleFactor, timePos);
        curWidgetPos = ui->pushButton->pos();
        ui->pushButton->move(curWidgetPos.x() * scaleFactor, timePos + ui->label_2->height() + (9 * scaleFactor));
        if(Bridge  && Mmove == 1){
            ui->pushButton_6->resize(QSize(81 * scaleFactor, 16 * scaleFactor));
            ui->pushButton_6->move(QPoint(ui->pushButton->pos().x(), ui->pushButton->pos().y() - (5 * scaleFactor)));
            ui->pushButton->move(QPoint(ui->pushButton->pos().x(), ui->pushButton->pos().y() + (14 * scaleFactor)));
        }

        curWidgetPos = ui->label_8->pos();
        timePos = timePos - 4 - ui->label_8->height();
        ui->label_8->move(curWidgetPos.x() * scaleFactor, timePos);

        if(players.size() == 2){
            curWidgetPos = ui->label_13->pos();
            timePos = timePos - 5 - ui->label_13->height();
            ui->label_13->move(curWidgetPos.x() * scaleFactor, timePos);

            curWidgetPos = ui->label_12->pos();
            ui->label_12->move(ui->label_13->pos().x() + ui->label_13->width() + (4 * scaleFactor), timePos);

            curWidgetPos = ui->label_11->pos();
            ui->label_11->move(ui->label_12->pos().x() + ui->label_12->width() + (3 * scaleFactor), timePos);
        }
        else{
            timePos = timePos - 5 - ui->label_12->height();
            curWidgetPos = ui->label_12->pos();
            ui->label_12->move(curWidgetPos.x() * scaleFactor, timePos);

            curWidgetPos = ui->label_11->pos();
            ui->label_11->move(ui->label_12->pos().x() + ui->label_12->width() + (5 * scaleFactor), timePos);
        }
    }

    ui->horizontalLayoutWidget_4->move((windowSize.width() / 3), (windowSize.height() / 2) - (ui->horizontalLayoutWidget_4->height() / 2));

    if(players.size() == 4){
        int timeInt = (10 * scaleFactor) + ui->verticalLayoutWidget->width();
        ui->label_2->move((((ui->horizontalLayoutWidget_4->pos().x() - timeInt) / 2) - (ui->label_2->width() / 2)) + timeInt, timePos);
        ui->pushButton->move(ui->label_2->pos().x(), timePos + ui->label_2->height() + (9 * scaleFactor));
        if(Bridge && Mmove == 1){
            ui->pushButton_6->resize(QSize(81 * scaleFactor, 16 * scaleFactor));
            ui->pushButton_6->move(QPoint(ui->pushButton->pos().x(), ui->pushButton->pos().y() - (5 * scaleFactor)));
            ui->pushButton->move(QPoint(ui->pushButton->pos().x(), ui->pushButton->pos().y() + (14 * scaleFactor)));
        }

        timePos = timePos - 4 - ui->label_8->height();
        ui->label_8->move(ui->label_2->pos().x(), timePos);

        timePos = timePos - 5 - ui->label_12->height();
        ui->label_12->move(ui->label_2->pos().x() + (5 * scaleFactor), timePos);

        ui->label_11->move(ui->label_12->pos().x() + ui->label_12->width() + (5 * scaleFactor), timePos);
    }

    ui->pushButton_10->move((windowSize.width() - 20) - ui->pushButton_10->width(), 20);
    ui->pushButton_11->move((windowSize.width() - 20) - ui->pushButton_11->width(), 20 + ui->pushButton_10->height() + (10 * scaleFactor));
    ui->pushButton_14->move((windowSize.width() - 20) - ui->pushButton_14->width(), ui->pushButton_11->pos().y() + ui->pushButton_11->height() + (10 * scaleFactor));

    ui->scrollArea->move((windowSize.width() / 2) - (ui->scrollArea->width() / 2), windowSize.height() - 20 - ui->scrollArea->height());
    ui->scrollAreaWidgetContents->setMinimumSize(QSize((70*scaleFactor)*players[0]->getCardsSize(),0));

    ui->horizontalLayoutWidget_2->move((windowSize.width() / 2) - (ui->horizontalLayoutWidget_2->width() / 2), 20);
    ui->verticalLayoutWidget->move(ui->verticalLayoutWidget->pos().x() * scaleFactor, (windowSize.height() / 2) - (ui->verticalLayoutWidget->height() / 2));
    ui->verticalLayoutWidget_2->move(ui->pushButton_10->pos().x() - (ui->verticalLayoutWidget_2->width() + (10 * scaleFactor)), (windowSize.height() / 2) - (ui->verticalLayoutWidget_2->height() / 2));

    ui->label_5->move((windowSize.width() / 2) - (ui->label_5->width() / 2), ui->scrollArea->pos().y() - 3 - ui->label_5->height());
    ui->label_6->move((windowSize.width() / 2) - (ui->label_6->width() / 2), ui->horizontalLayoutWidget_2->pos().y() + 3 + ui->horizontalLayoutWidget_2->height());
    ui->label_14->move(ui->verticalLayoutWidget_2->pos().x(), ui->verticalLayoutWidget_2->pos().y() + ui->verticalLayoutWidget_2->height() + 3);
    ui->label_15->move(ui->verticalLayoutWidget->pos().x(), ui->verticalLayoutWidget->pos().y() + ui->verticalLayoutWidget->height() + 3);

    ui->label_3->move(ui->horizontalLayoutWidget_4->pos().x() + ui->horizontalLayoutWidget_4->width() + (9 * scaleFactor), ui->horizontalLayoutWidget_4->pos().y() + (ui->horizontalLayoutWidget_4->height() / 2) - (ui->label_3->height() / 2));

    if(players.size() != 2){
        int timeInt = ui->label_3->pos().x() + ui->label_3->width();
        ui->horizontalLayoutWidget_3->move((((ui->verticalLayoutWidget_2->pos().x() - timeInt) / 2) - (ui->horizontalLayoutWidget_4->width() + 10 + ui->label->width()) / 2) + timeInt, (windowSize.height() / 2) - (ui->horizontalLayoutWidget_3->height() / 2));
    }
    else{
        ui->horizontalLayoutWidget_3->move((windowSize.width() / 3) * 2, (windowSize.height() / 2) - (ui->horizontalLayoutWidget_3->height() / 2));
    }

    ui->label_7->move(ui->horizontalLayoutWidget_3->pos().x(), ui->horizontalLayoutWidget_3->pos().y() - ui->label_7->height() - (9 * scaleFactor));
    ui->label->move(ui->horizontalLayoutWidget_3->pos().x() + ui->horizontalLayoutWidget_3->width() + (9 * scaleFactor), (windowSize.height() / 2) - (ui->label->height() / 2));
    ui->label_4->move(ui->label_7->pos().x(), ui->horizontalLayoutWidget_3->pos().y() + ui->horizontalLayoutWidget_3->height() + (2 * scaleFactor));

    ui->pushButton_2->move(ui->horizontalLayoutWidget_4->pos().x() + (ui->horizontalLayoutWidget_4->width() / 2) - (ui->pushButton_2->width() * 2), ui->horizontalLayoutWidget_4->pos().y() + ui->horizontalLayoutWidget_4->height() + (9 * scaleFactor));
    ui->pushButton_3->move(ui->pushButton_2->pos().x() + ui->pushButton_2->width(), ui->horizontalLayoutWidget_4->pos().y() + ui->horizontalLayoutWidget_4->height() + (9 * scaleFactor));
    ui->pushButton_4->move(ui->pushButton_3->pos().x() + ui->pushButton_3->width(), ui->horizontalLayoutWidget_4->pos().y() + ui->horizontalLayoutWidget_4->height() + (9 * scaleFactor));
    ui->pushButton_5->move(ui->pushButton_4->pos().x() + ui->pushButton_4->width(), ui->horizontalLayoutWidget_4->pos().y() + ui->horizontalLayoutWidget_4->height() + (9 * scaleFactor));
}

void MainWindow::scaleWidget(QWidget* widget) {
    if (!widget) return;

    QSize currentSize = widget->size();

    int newWidth = static_cast<int>(currentSize.width() * scaleFactor);
    int newHeight = static_cast<int>(currentSize.height() * scaleFactor);

    if(widget == ui->verticalLayoutWidget || widget == ui->verticalLayoutWidget_2){
        QSize windowSize = this->size();
        if(windowSize.width() > ui->horizontalLayoutWidget_2->width() + (ui->horizontalLayoutWidget_2->height() * 2) + (70 * scaleFactor) + ui->pushButton_10->width()){
            newHeight = static_cast<int>(windowSize.height() - windowSize.height()/2);
        }
        if(windowSize.height() - windowSize.height()/2 > ui->horizontalLayoutWidget_2->width()){
            newHeight = static_cast<int>(ui->horizontalLayoutWidget_2->width());
        }
    }

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

    if(widget == ui->label_6){
        QString curText = ui->label_6->text();
        int newFontSize = 11 * scaleFactor;
        QRegularExpression regex("font-size:\\s*\\d+pt");
        curText.replace(regex, QString("font-size: %1pt").arg(newFontSize));
        ui->label_6->setText(curText);
    }
    else if(widget == ui->label_14){
        QString curText = ui->label_14->text();
        int newFontSize = 11 * scaleFactor;
        QRegularExpression regex("font-size:\\s*\\d+pt");
        curText.replace(regex, QString("font-size: %1pt").arg(newFontSize));
        ui->label_14->setText(curText);
    }else if(widget == ui->label_15){
        QString curText = ui->label_15->text();
        int newFontSize = 11 * scaleFactor;
        QRegularExpression regex("font-size:\\s*\\d+pt");
        curText.replace(regex, QString("font-size: %1pt").arg(newFontSize));
        ui->label_15->setText(curText);
    }

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
    ui->label_14->setGeometry(savedlabel14Geo);
    ui->label_15->setGeometry(savedlabel15Geo);
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
    ui->pushButton_6->setGeometry(savedButton6Geo);
    ui->pushButton_10->setGeometry(savedButton10Geo);
    ui->pushButton_11->setGeometry(savedButton11Geo);
    ui->pushButton_14->setGeometry(savedButton14Geo);
    ui->label->setStyleSheet(savedLabelStyle);
    ui->label_2->setStyleSheet(savedLabel2Style);
    ui->label_4->setStyleSheet(savedLabel4Style);
    ui->label_5->setStyleSheet(savedLabel5Style);
    ui->label_6->setStyleSheet(savedLabel6Style);
    ui->label_14->setStyleSheet(savedLabel14Style);
    ui->label_15->setStyleSheet(savedLabel15Style);
    ui->label_7->setStyleSheet(savedLabel7Style);
    ui->label_8->setStyleSheet(savedLabel8Style);
    ui->label_12->setStyleSheet(savedLabel12Style);
    ui->pushButton->setStyleSheet(savedButtonStyle);
    ui->pushButton_6->setStyleSheet(savedButton6Style);
    ui->pushButton_10->setStyleSheet(savedButton10Style);
    ui->pushButton_11->setStyleSheet(savedButton11Style);
    ui->pushButton_14->setStyleSheet(savedButton14Style);

    if(speedUp){
        ui->pushButton_14->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(113,178,235); border-radius: 10px; background-color: rgb(147,195,237); color: black; font: bold;");
    }
    else{
        ui->pushButton_14->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 10px; background-color: rgb(230,230,230); color: black; font: bold;");
    }

    if(Bridge && Mmove == 1){
        ui->pushButton->resize(QSize(ui->pushButton->size().width(), ui->pushButton->size().height() - 15));
        ui->pushButton->move(QPoint(ui->pushButton->pos().x(), ui->pushButton->pos().y() + 14));
    }

    QString curText = ui->label_6->text();
    int newFontSize = 11;
    QRegularExpression regex("font-size:\\s*\\d+pt");
    curText.replace(regex, QString("font-size: %1pt").arg(newFontSize));
    ui->label_6->setText(curText);
    curText = ui->label_14->text();
    curText.replace(regex, QString("font-size: %1pt").arg(newFontSize));
    ui->label_14->setText(curText);
    curText = ui->label_15->text();
    curText.replace(regex, QString("font-size: %1pt").arg(newFontSize));
    ui->label_15->setText(curText);

    if(!players.empty()){
        std::string dif = players[1]->getDifficulty();
        if(dif == "Middle"){
            ui->label_13->setStyleSheet(QString("font-weight: %1; font-size: %2pt; font-family: 'Segoe UI Black'; color: orange").arg(900).arg(11));
        }
        else if(dif == "Hard"){
            ui->label_13->setStyleSheet(QString("font-weight: %1; font-size: %2pt; font-family: 'Segoe UI Black'; color: red").arg(900).arg(11));
        }
    }

    ui->centralwidget->resize(savedMainWindowSize);

    ui->verticalLayoutWidget->setGeometry(savedVLayoutGeo);
    ui->verticalLayoutWidget_2->setGeometry(savedVLayout2Geo);
    ui->horizontalLayoutWidget_2->setGeometry(savedLayout2Geo);
    ui->horizontalLayoutWidget_3->setGeometry(savedLayout3Geo);
    ui->horizontalLayoutWidget_4->setGeometry(savedLayout4Geo);
    ui->horizontalLayoutWidget->setGeometry(savedLayoutGeo);
    ui->scrollArea->setGeometry(savedScroll);
    QSize s(70,101);

    for(Player* playe : players){
        QPushButton* (&buttons)[36] = playe->getButtons();
        if((playe == players[2] && players.size() == 3) || ((playe == players[1] || playe == players[3]) && players.size() == 4)){
            s = QSize(101,70);
        }
        else{
            s = QSize(70,101);
        }
        for(int i = 0; i < playe->getCardsSize(); i++){
            if(buttons[i]){
                buttons[i]->setIconSize(s);
                buttons[i]->setFixedSize(s);
            }
        }
    }
    s = QSize(70,101);
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

    ui->pushButton_10->setIconSize(savedIcon10);
    ui->pushButton_11->setIconSize(savedIcon11);
    ui->pushButton_14->setIconSize(savedIcon14);
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
    savedlabel14Geo = ui->label_14->geometry();
    savedlabel15Geo = ui->label_15->geometry();
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
    savedButton6Geo = ui->pushButton_6->geometry();
    savedButton10Geo = ui->pushButton_10->geometry();
    savedButton11Geo = ui->pushButton_11->geometry();
    savedButton14Geo = ui->pushButton_14->geometry();
    savedLabelStyle = ui->label->styleSheet();
    savedLabel2Style = ui->label_2->styleSheet();
    savedLabel4Style = ui->label_4->styleSheet();
    savedLabel5Style = ui->label_5->styleSheet();
    savedLabel6Style = ui->label_6->styleSheet();
    savedLabel14Style = ui->label_14->styleSheet();
    savedLabel15Style = ui->label_15->styleSheet();
    savedLabel7Style = ui->label_7->styleSheet();
    savedLabel8Style = ui->label_8->styleSheet();
    savedLabel12Style = ui->label_12->styleSheet();
    savedButtonStyle = ui->pushButton->styleSheet();
    savedButton6Style = ui->pushButton_6->styleSheet();
    savedButton10Style = ui->pushButton_10->styleSheet();
    savedButton11Style = ui->pushButton_11->styleSheet();
    savedButton14Style = ui->pushButton_14->styleSheet();
    savedLayout2Geo = ui->horizontalLayoutWidget_2->geometry();
    savedVLayoutGeo = ui->verticalLayoutWidget->geometry();
    savedVLayout2Geo = ui->verticalLayoutWidget_2->geometry();
    savedLayout3Geo = ui->horizontalLayoutWidget_3->geometry();
    savedLayout4Geo = ui->horizontalLayoutWidget_4->geometry();
    savedLayoutGeo = ui->horizontalLayoutWidget->geometry();
    savedScroll = ui->scrollArea->geometry();
    savedIcon10 = ui->pushButton_10->iconSize();
    savedIcon11 = ui->pushButton_11->iconSize();
    savedIcon14 = ui->pushButton_14->iconSize();
}

void MainWindow::scaleForMany()
{
    if(players.size() == 2){
        if((ui->comboBox_2->currentIndex() == 1 && !ui->comboBox_2->isHidden()) ||
            (ui->comboBox_5->currentIndex() == 1 && !ui->comboBox_5->isHidden()) ||
            (ui->comboBox_7->currentIndex() == 1 && !ui->comboBox_7->isHidden())){
            ui->label_12->move(12, 150);
            ui->label_11->move(48, 150);
            ui->label_13->hide();
        }
        else {
            ui->label_11->move(68, 150);
            ui->label_12->move(34, 150);
            ui->label_13->move(12, 150);
        }

        ui->label_2->move(10, 200);
        ui->label_8->move(10, 175);
        ui->pushButton->move(10, 240);
        ui->pushButton_6->move(10, 235);

        ui->label->move(590, 200);
        ui->label_4->move(500, 260);
        ui->label_7->move(500, 130);
        ui->horizontalLayoutWidget_3->move(500, 160);

        ui->label_5->move(10, 280);
        ui->label_6->move(10, 125);

        ui->horizontalLayoutWidget_2->setGeometry(QRect(10,20,611,101));
    }
    else if(players.size() == 3){
        ui->label_12->move(12, 150);
        ui->label_11->move(48, 150);
        ui->label_13->hide();

        ui->label_2->move(10, 200);
        ui->label_8->move(10, 175);
        ui->pushButton->move(10, 240);
        ui->pushButton_6->move(10, 235);

        ui->label->move(450, 200);
        ui->label_4->move(360, 260);
        ui->label_7->move(360, 130);
        ui->horizontalLayoutWidget_3->move(360, 160);

        ui->label_14->move(520, 272);
        ui->label_14->show();

        ui->label_5->move(10, 280);
        ui->label_6->move(10, 125);

        ui->horizontalLayoutWidget_2->setGeometry(QRect(10,20,490,101));
    }
    else{
        ui->label_2->move(125, 200);
        ui->label_8->move(125, 175);
        ui->pushButton->move(125, 240);
        ui->pushButton_6->move(125, 235);

        ui->label_12->move(127, 150);
        ui->label_11->move(163, 150);
        ui->label_13->hide();

        ui->label->move(450, 200);
        ui->label_4->move(360, 260);
        ui->label_7->move(360, 130);
        ui->horizontalLayoutWidget_3->move(360, 160);

        ui->label_14->move(520, 272);
        ui->label_14->show();
        ui->label_15->move(10, 272);
        ui->label_15->show();
        ui->label_5->move(320, 280);
        ui->label_6->move(190, 125);

        ui->horizontalLayoutWidget_2->setGeometry(QRect(130,20,370,101));
    }
}

void MainWindow::playerCreator()
{
    clickedSound();

    std::vector<std::string> botNames = {"Adam", "David", "Alex", "Niko", "Kevin", "Andrew", "Luka", "Steven", "Ivan", "Leo","Robert",
                                         "Thomas", "John", "Colin", "Edward", "Frank", "Daniel", "Donald", "Bruce", "Tom", "Max", "Mark", "Oscar"};

    players.push_back(new Human(ui->lineEdit->text().toStdString(), this));
    players[0]->getLayout() = ui->horizontalLayoutWidget;
    players[0]->getLabel() = ui->label_5;

    if(playersCount == 2){
        if((ui->comboBox_2->currentIndex() == 1 && !ui->comboBox_2->isHidden()) ||
            (ui->comboBox_5->currentIndex() == 1 && !ui->comboBox_5->isHidden()) ||
            (ui->comboBox_7->currentIndex() == 1 && !ui->comboBox_7->isHidden())){
            players.push_back(new Human("Name", this));
        }
        else{
            int randomNumber = std::rand() % (botNames.size() - 1);
            players.push_back(new Bot("Bot " + botNames[randomNumber], this));
            botNames.erase(botNames.begin() + randomNumber);

            if(!ui->comboBox_3->isHidden()){
                players[1]->setDifficulty(ui->comboBox_3->currentText().toStdString());
            }
            else if(!ui->comboBox_4->isHidden()){
                players[1]->setDifficulty(ui->comboBox_4->currentText().toStdString());
            }
            else{
                players[1]->setDifficulty(ui->comboBox_6->currentText().toStdString());
            }
        }

        players[1]->getLayout() = ui->horizontalLayoutWidget_2;
        players[1]->getLabel() = ui->label_6;
    }
    else if(playersCount == 3){
        if(ui->comboBox_5->isHidden()){
            if(ui->comboBox_2->currentIndex() == 0){
                int randomNumber = std::rand() % (botNames.size() - 1);
                players.push_back(new Bot("Bot " + botNames[randomNumber], this));
                botNames.erase(botNames.begin() + randomNumber);
                players[1]->setDifficulty(ui->comboBox_3->currentText().toStdString());
            }
            else{
                players.push_back(new Human("Name", this));
            }
            players[1]->getLayout() = ui->horizontalLayoutWidget_2;
            players[1]->getLabel() = ui->label_6;

            if(ui->comboBox_7->currentIndex() == 0){
                int randomNumber = std::rand() % (botNames.size() - 1);
                players.push_back(new Bot("Bot " + botNames[randomNumber], this));
                botNames.erase(botNames.begin() + randomNumber);
                players[2]->setDifficulty(ui->comboBox_6->currentText().toStdString());
            }
            else{
                players.push_back(new Human("Name", this));
            }
            players[2]->getLayout() = ui->verticalLayoutWidget_2;
            players[2]->getLabel() = ui->label_14;
        }
        else if(ui->comboBox_2->isHidden()){
            if(ui->comboBox_5->currentIndex() == 0){
                int randomNumber = std::rand() % (botNames.size() - 1);
                players.push_back(new Bot("Bot " + botNames[randomNumber], this));
                botNames.erase(botNames.begin() + randomNumber);
                players[1]->setDifficulty(ui->comboBox_4->currentText().toStdString());
            }
            else{
                players.push_back(new Human("Name", this));
            }
            players[1]->getLayout() = ui->horizontalLayoutWidget_2;
            players[1]->getLabel() = ui->label_6;

            if(ui->comboBox_7->currentIndex() == 0){
                int randomNumber = std::rand() % (botNames.size() - 1);
                players.push_back(new Bot("Bot " + botNames[randomNumber], this));
                botNames.erase(botNames.begin() + randomNumber);
                players[2]->setDifficulty(ui->comboBox_6->currentText().toStdString());
            }
            else{
                players.push_back(new Human("Name", this));
            }
            players[2]->getLayout() = ui->verticalLayoutWidget_2;
            players[2]->getLabel() = ui->label_14;
        }
        else{
            if(ui->comboBox_5->currentIndex() == 0){
                int randomNumber = std::rand() % (botNames.size() - 1);
                players.push_back(new Bot("Bot " + botNames[randomNumber], this));
                botNames.erase(botNames.begin() + randomNumber);
                players[1]->setDifficulty(ui->comboBox_4->currentText().toStdString());
            }
            else{
                players.push_back(new Human("Name", this));
            }
            players[1]->getLayout() = ui->horizontalLayoutWidget_2;
            players[1]->getLabel() = ui->label_6;

            if(ui->comboBox_2->currentIndex() == 0){
                int randomNumber = std::rand() % (botNames.size() - 1);
                players.push_back(new Bot("Bot " + botNames[randomNumber], this));
                botNames.erase(botNames.begin() + randomNumber);
                players[2]->setDifficulty(ui->comboBox_3->currentText().toStdString());
            }
            else{
                players.push_back(new Human("Name", this));
            }
            players[2]->getLayout() = ui->verticalLayoutWidget_2;
            players[2]->getLabel() = ui->label_14;
        }
    }
    else{
        if(ui->comboBox_5->currentIndex() == 0){
            int randomNumber = std::rand() % (botNames.size() - 1);
            players.push_back(new Bot("Bot " + botNames[randomNumber], this));
            botNames.erase(botNames.begin() + randomNumber);
            players[1]->setDifficulty(ui->comboBox_4->currentText().toStdString());
        }
        else{
            players.push_back(new Human("Name", this));
        }
        players[1]->getLayout() = ui->verticalLayoutWidget;
        players[1]->getLabel() = ui->label_15;

        if(ui->comboBox_2->currentIndex() == 0){
            int randomNumber = std::rand() % (botNames.size() - 1);
            players.push_back(new Bot("Bot " + botNames[randomNumber], this));
            botNames.erase(botNames.begin() + randomNumber);
            players[2]->setDifficulty(ui->comboBox_3->currentText().toStdString());
        }
        else{
            players.push_back(new Human("Name", this));
        }
        players[2]->getLayout() = ui->horizontalLayoutWidget_2;
        players[2]->getLabel() = ui->label_6;

        if(ui->comboBox_7->currentIndex() == 0){
            int randomNumber = std::rand() % (botNames.size() - 1);
            players.push_back(new Bot("Bot " + botNames[randomNumber], this));
            botNames.erase(botNames.begin() + randomNumber);
            players[3]->setDifficulty(ui->comboBox_6->currentText().toStdString());
        }
        else{
            players.push_back(new Human("Name", this));
        }
        players[3]->getLayout() = ui->verticalLayoutWidget_2;
        players[3]->getLabel() = ui->label_14;
    }

    ui->pushButton_11->move(630, 60);
    ui->pushButton_15->hide();
    ui->pushButton_16->hide();
    ui->pushButton_17->hide();
    ui->pushButton_18->hide();
    ui->pushButton_19->hide();
    ui->label_20->hide();
    ui->pushButton_20->hide();
    ui->pushButton_27->hide();
    ui->lineEdit->hide();
    ui->comboBox_2->hide();
    ui->comboBox_3->hide();
    ui->comboBox_4->hide();
    ui->comboBox_5->hide();
    ui->comboBox_6->hide();
    ui->comboBox_7->hide();
    ui->pushButton_21->hide();
    ui->pushButton_22->hide();
    ui->pushButton_23->hide();
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
    ui->pushButton_24->hide();
    ui->pushButton_25->hide();
    ui->pushButton_26->hide();

    isInGame = true;

    player->stop();
    player->setSource(QUrl::fromLocalFile(tempFilePathTheme));
    audioOutput->setVolume((musicvol / 100.0) * 0.5);
    player->setLoops(-1);
    player->play();

    window->hide();
    ui->pushButton_12->hide();
    ui->pushButton_13->hide();
    ui->pushButton_7->hide();
    ui->pushButton_8->hide();
    ui->label_9->hide();
    ui->label_10->hide();
    ui->label_16->hide();
    ui->line->hide();
    ui->line_2->hide();

    for(Player* playe : players){
        if (playe->getDifficulty() == "Middle"){
            if(players.size() == 2){
                playe->getLabel()->setText(QString::fromStdString(playe->getName() + " | Points: " + std::to_string(playe->getPoints())));
            }
            else{
                playe->getLabel()->setText("<html><span style='color: orange; font-size: 11pt; font-weight: 900; font-family: 'Segoe UI Black';'>M</span> "
                                           + QString::fromStdString(playe->getName() + " | Points: " + std::to_string(playe->getPoints()) + "</html>"));
            }
        }
        else if (playe->getDifficulty() == "Hard"){
            if(players.size() == 2){
                playe->getLabel()->setText(QString::fromStdString(playe->getName() + " | Points: " + std::to_string(playe->getPoints())));
            }
            else{
                playe->getLabel()->setText("<html><span style='color: red; font-size: 11pt; font-weight: 900; font-family: 'Segoe UI Black';'>H</span> "
                                           + QString::fromStdString(playe->getName() + " | Points: " + std::to_string(playe->getPoints()) + "</html>"));
            }
        }
        else{
            playe->getLabel()->setText(QString::fromStdString(playe->getName() + " | Points: " + std::to_string(playe->getPoints())));
        }

        playe->getLabel()->show();
    }

    players[0]->getLabel()->setText(QString::fromStdString("Your Points: " + std::to_string(players[0]->getPoints())));

    ui->label->show();
    ui->label_2->show();
    ui->label_7->show();
    Set = 1;
    ui->label_8->setText(QString::fromStdString("Set: " + std::to_string(Set)));
    ui->label_8->show();
    ui->pushButton_10->show();
    ui->pushButton_11->show();
}

void MainWindow::endClicked(bool end) {
    int tempEndValue = 0;
    if(end)
        tempEndValue = 1;
    if(tableCards[tableCardsSize - 1] == "Qp" && QSMode && players[Mmove - 1]->getPass() > 0){
        players[Mmove - 1]->setPass(0);
        int next;

        next = nextOne(Mmove);

        std::string (&Cards)[36] = players[next - 1]->getCards();
        int CardsSize = players[next - 1]->getCardsSize();

        if(next != 1){
            for(int i = 0; i < 5; i++){
                if(ColodCardsSize == 0){
                    if(i != 0){
                        onAddWidgetColod(0);
                    }
                    shuffling();
                }
                Cards[CardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetBot(players[next - 1], CardsSize);
                CardsSize++;
                players[next - 1]->setCardsSize(CardsSize);
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodButtons[ColodCardsSize - 1] = nullptr;
                ColodCardsSize--;
            }
        }
        else {
            for(int i = 0; i < 5; i++){
                if(ColodCardsSize == 0){
                    if(i != 0){
                        onAddWidgetColod(0);
                    }
                    shuffling();
                }
                Cards[CardsSize] = ColodCards[ColodCardsSize - 1];
                onAddWidgetPlayer(players[0], Cards[CardsSize], CardsSize);
                CardsSize++;
                players[0]->setCardsSize(CardsSize);
                ColodCards[ColodCardsSize - 1] = "";
                delete ColodButtons[ColodCardsSize - 1];
                ColodButtons[ColodCardsSize - 1] = nullptr;
                ColodCardsSize--;
            }
        }

        next = nextOne(next);

        Mmove = next;

        if(ColodCardsSize == 0){
            onAddWidgetColod(0);
            isShuffl = true;
            ui->label_4->setText(QString::fromStdString("Shuffling (" + std::to_string(PointsX + 1) + "x)"));
            ui->label_4->show();
        }
        ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));
    }
    else if(tableCards[tableCardsSize - 1][0] == 'A' && players[Mmove - 1]->getPass() > 0){
        int next;
        next = nextOne(Mmove);
        for(int i = 0; i < players[Mmove - 1]->getPass() + tempEndValue; i++){
            next = nextOne(next);
        }
        players[Mmove - 1]->setPass(0);
        Mmove = next;
    }
    else if(tableCards[tableCardsSize - 1][0] == '8' && players[Mmove - 1]->getPass() > 0){
        int next;

        for(int i = 0; i < players[Mmove - 1]->getPass() + tempEndValue; i++){
            if(i == 0){
                next = nextOne(Mmove);
            }
            else{
                next = nextOne(next);
            }
            std::string (&Cards)[36] = players[next - 1]->getCards();
            int CardsSize = players[next - 1]->getCardsSize();

            if (ColodCardsSize == 0) {
                shuffling();
            }

            Cards[CardsSize] = ColodCards[ColodCardsSize - 1];
            if(next != 1){
                onAddWidgetBot(players[next - 1], CardsSize);
            }
            else{
                onAddWidgetPlayer(players[0], Cards[CardsSize], CardsSize);
            }
            CardsSize++;
            players[next - 1]->setCardsSize(CardsSize);
            ColodCards[ColodCardsSize - 1] = "";
            delete ColodButtons[ColodCardsSize - 1];
            ColodButtons[ColodCardsSize - 1] = nullptr;
            ColodCardsSize--;

            if (ColodCardsSize == 0) {
                onAddWidgetColod(0);
                isShuffl = true;
                ui->label_4->setText(QString::fromStdString("Shuffling (" + std::to_string(PointsX + 1) + "x)"));
                ui->label_4->show();
            }

            if (ColodCardsSize == 0) {
                shuffling();
            }

            Cards[CardsSize] = ColodCards[ColodCardsSize - 1];
            if(next != 1){
                onAddWidgetBot(players[next - 1], CardsSize);
            }
            else{
                onAddWidgetPlayer(players[0], Cards[CardsSize], CardsSize);
            }
            CardsSize++;
            players[next - 1]->setCardsSize(CardsSize);
            ColodCards[ColodCardsSize - 1] = "";
            delete ColodButtons[ColodCardsSize - 1];
            ColodButtons[ColodCardsSize - 1] = nullptr;
            ColodCardsSize--;

            ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));

            if (ColodCardsSize == 0) {
                onAddWidgetColod(0);
                isShuffl = true;
                ui->label_4->setText(QString::fromStdString("Shuffling (" + std::to_string(PointsX + 1) + "x)"));
                ui->label_4->show();
            }
        }
        next = nextOne(next);
        players[Mmove - 1]->setPass(0);
        Mmove = next;
    }
    else{
        players[Mmove - 1]->setPass(0);
        players[Mmove - 1]->setJackKol(0);
        Mmove = nextOne(Mmove);
    }

    if(!end){
        for(Player* playe : players){
            playe->setCheckForTake(0);
        }

        if(Mmove == 1){
            ui->label_2->setText("Your turn");
            ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
        }
        else {
            ui->label_2->setText(QString::fromStdString(players[Mmove - 1]->getName()) + "\nturn");
            ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
        }
        if(isFullscreen && isInGame){
            QString cs = ui->label_2->styleSheet();
            ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
        }

        secMove = 0;
        ui->pushButton->hide();
    }
}

void MainWindow::on_pushButton_clicked()
{
    clickedSound();

    endClicked();

    if(Bridge){
        Bridge = false;
        pushButtonBridge();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    clickedSound();
    if(Bridge){
        Bridge = false;
        pushButtonBridge();
    }
    Jackchoose = "c";
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    Mmove = nextOne(1);
    ui->label_2->setText(QString::fromStdString(players[Mmove - 1]->getName()) + "\nturn");
    ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
    if(isFullscreen && isInGame){
        QString cs = ui->label_2->styleSheet();
        ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
    }
    secMove = 0;
    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/chirva.png);");
    ui->label_3->show();
    players[0]->setJackKol(0);
}

void MainWindow::on_pushButton_3_clicked()
{
    clickedSound();
    if(Bridge){
        Bridge = false;
        pushButtonBridge();
    }
    Jackchoose = "k";
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    Mmove = nextOne(1);
    ui->label_2->setText(QString::fromStdString(players[Mmove - 1]->getName()) + "\nturn");
    ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
    if(isFullscreen && isInGame){
        QString cs = ui->label_2->styleSheet();
        ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
    }
    secMove = 0;
    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/kresti.png);");
    ui->label_3->show();
    players[0]->setJackKol(0);
}

void MainWindow::on_pushButton_4_clicked()
{
    clickedSound();
    if(Bridge){
        Bridge = false;
        pushButtonBridge();
    }
    Jackchoose = "b";
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    Mmove = nextOne(1);
    ui->label_2->setText(QString::fromStdString(players[Mmove - 1]->getName()) + "\nturn");
    ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
    if(isFullscreen && isInGame){
        QString cs = ui->label_2->styleSheet();
        ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
    }
    secMove = 0;
    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/bybna.png);");
    ui->label_3->show();
    players[0]->setJackKol(0);
}

void MainWindow::on_pushButton_5_clicked()
{
    clickedSound();
    if(Bridge){
        Bridge = false;
        pushButtonBridge();
    }
    Jackchoose = "p";
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    Mmove = nextOne(1);
    ui->label_2->setText(QString::fromStdString(players[Mmove - 1]->getName()) + "\nturn");
    ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px;  color: white; font: bold;");
    if(isFullscreen && isInGame){
        QString cs = ui->label_2->styleSheet();
        ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
    }
    secMove = 0;
    ui->label_3->setStyleSheet("border-image: url(:/img/PNG-cards-1.3/piki.png);");
    ui->label_3->show();
    players[0]->setJackKol(0);
}

void MainWindow::on_pushButton_8_clicked()
{
    clickedSound();
    QMessageBox YesNomsgBox(this);
    YesNomsgBox.setWindowTitle("Quit");
    YesNomsgBox.setText("Are you sure?");
    YesNomsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    YesNomsgBox.setDefaultButton(QMessageBox::No);
    YesNomsgBox.setStyleSheet(massBoxStyle);

    foreach (QPushButton *button, YesNomsgBox.findChildren<QPushButton*>()) {
        button->setCursor(Qt::PointingHandCursor);
    }

    int reply = YesNomsgBox.exec();
    clickedSound();

    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    clickedSound();

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

    if(server)
        server->HubChanged();
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

    if(server)
        server->HubChanged();
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
    clickedSound();
    if(server){
        delete server;
        server = nullptr;
    }

    QMessageBox YesNomsgBox(this);
    YesNomsgBox.setWindowTitle("Quit");
    YesNomsgBox.setText("Are you sure?");
    YesNomsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    YesNomsgBox.setDefaultButton(QMessageBox::No);
    YesNomsgBox.setStyleSheet(massBoxStyle);

    foreach (QPushButton *button, YesNomsgBox.findChildren<QPushButton*>()) {
        button->setCursor(Qt::PointingHandCursor);
    }

    int reply = YesNomsgBox.exec();
    clickedSound();

    if (reply == QMessageBox::Yes) {
        isInGame = false;
        if(Bridge){
            Bridge = false;
            if(Mmove == 1)
                pushButtonBridge();
        }

        for (Player* playe : players) {
            delete playe;
        }
        players.clear();

        if(ColodCardsSize == 0){
            delete ColodButtons[0];
            ColodButtons[0] = nullptr;
        }
        for(int i = 0; i < ColodCardsSize; i++){
            delete ColodButtons[i];
            ColodButtons[i] = nullptr;
            ColodCards[i] = "";
        }
        for(int i = 0; i < tableCardsSize; i++){
            delete TableButtons[i];
            TableButtons[i] = nullptr;
            tableCards[i] = "";
        }

        ColodCardsSize = 0;
        tableCardsSize = 0;

        isShuffl = false;
        PointsX = 1;
        ui->label->setVisible(false);
        ui->label_2->setVisible(false);

        ui->pushButton->hide();
        ui->pushButton_6->hide();
        ui->pushButton_2->hide();
        ui->pushButton_3->hide();
        ui->pushButton_4->hide();
        ui->pushButton_5->hide();
        ui->pushButton_10->hide();
        ui->pushButton_11->hide();
        ui->pushButton_14->hide();
        speedUp = false;
        ui->pushButton_14->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 10px; background-color: rgb(230,230,230); color: black; font: bold;");
        ui->label_3->hide();
        ui->label_4->hide();
        ui->label_5->hide();
        ui->label_14->hide();
        ui->label_15->hide();
        ui->label_6->hide();
        ui->label_7->hide();
        ui->label_8->hide();
        ui->label_11->hide();
        ui->label_12->hide();
        ui->label_13->hide();
        ui->label_14->hide();
        ui->label_15->hide();
        ui->scrollArea->hide();
        ui->label_7->setText(QString::fromStdString("Points " + std::to_string(PointsX) + "x"));

        delete timer;
        ui->pushButton_12->show();
        ui->pushButton_13->show();
        ui->pushButton_7->show();
        ui->pushButton_8->show();
        ui->label_11->hide();
        ui->label_12->hide();
        ui->label_13->hide();
        ui->scrollArea->hide();
        ui->label_9->show();
        ui->label_10->show();
        ui->label_16->show();
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
    clickedSound();

    window->hide();
    window->setWindowTitle("Options");
    window->setWindowIcon(QIcon(":/img/PNG-cards-1.3/kindpng_4637727.png"));
    window->show();
}

void MainWindow::on_pushButton_12_clicked()
{
    clickedSound();

    ui->lineEdit->setDisabled(false);

    ui->comboBox_2->setDisabled(false);
    ui->comboBox_3->setDisabled(false);
    ui->comboBox_5->setDisabled(false);
    ui->comboBox_4->setDisabled(false);
    ui->comboBox_7->setDisabled(false);
    ui->comboBox_6->setDisabled(false);
    ui->label_17->setText("");
    ui->label_18->setText("");
    ui->label_19->setText("");

    ui->pushButton_15->setDisabled(false);
    ui->pushButton_16->setDisabled(false);
    ui->pushButton_17->setDisabled(false);

    ui->label_17->move(340, 61);
    ui->label_18->move(120, 221);
    ui->label_19->move(560, 221);

    ui->pushButton_15->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 10px; background-color: rgb(230,230,230); color: black; font: bold;");
    ui->pushButton_16->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 10px; background-color: rgb(230,230,230); color: black; font: bold;");
    ui->pushButton_17->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 10px; background-color: rgb(230,230,230); color: black; font: bold;");

    ui->pushButton_20->hide();
    ui->pushButton_20->setText("Load");

    std::ifstream fin;
    fin.open(dirGame.toStdString());
    if(fin.is_open()){
        std::string getstr;
        fin >> getstr;
        if(getstr == "Can_Resume"){
            ui->pushButton_20->show();
        }
    }
    fin.close();

    fin.open(dirName.toStdString());
    if(fin.is_open()){
        std::string name;
        std::getline(fin, name);
        ui->lineEdit->setText(QString::fromStdString(name));
    }
    fin.close();

    playersCount = 1;

    QIcon ic3(":/img/PNG-cards-1.3/plus.png");
    ui->pushButton_15->setIcon(ic3);
    ui->pushButton_16->setIcon(ic3);
    ui->pushButton_17->setIcon(ic3);

    ui->pushButton_12->hide();
    ui->pushButton_13->hide();
    ui->pushButton_7->hide();
    ui->pushButton_8->hide();
    ui->label_9->hide();
    ui->label_10->hide();
    ui->label_16->hide();
    ui->line->hide();
    ui->line_2->hide();
    ui->pushButton_11->show();
    ui->pushButton_11->move(630, 20);
    ui->pushButton_15->show();
    ui->pushButton_15->move(310, 30);
    ui->pushButton_16->show();
    ui->pushButton_16->move(90, 190);
    ui->pushButton_17->show();
    ui->pushButton_17->move(530, 190);
    ui->pushButton_18->show();
    ui->pushButton_19->show();
    ui->pushButton_27->show();
    ui->label_20->show();
    ui->lineEdit->show();
    ui->pushButton_18->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 20px; color: white; font: bold;");
    ui->pushButton_18->setDisabled(true);

    server = new GameServer();
    QString ip = server->startServer();
    server->setMain(this);
    if(ip == ""){
        ui->label_20->hide();
        ui->pushButton_27->hide();
    }
    else{
        ui->label_20->setText("IP: " + ip);
    }
}

void MainWindow::on_pushButton_19_clicked()
{
    clickedSound();

    if(server){
        delete server;
        server = nullptr;
    }

    if(client){
        delete client;
        client = nullptr;
    }

    ui->pushButton_12->show();
    ui->pushButton_13->show();
    ui->pushButton_7->show();
    ui->pushButton_8->show();
    ui->label_9->show();
    ui->label_10->show();
    ui->label_16->show();
    ui->line->show();
    ui->line_2->show();
    ui->pushButton_11->hide();
    ui->pushButton_11->move(630, 60);
    ui->pushButton_15->hide();
    ui->pushButton_16->hide();
    ui->pushButton_17->hide();
    ui->pushButton_18->hide();
    ui->pushButton_19->hide();
    ui->label_20->hide();
    ui->pushButton_20->hide();
    ui->pushButton_27->hide();
    ui->lineEdit->hide();
    ui->comboBox_2->hide();
    ui->comboBox_3->hide();
    ui->comboBox_4->hide();
    ui->comboBox_5->hide();
    ui->comboBox_6->hide();
    ui->comboBox_7->hide();
    ui->pushButton_21->hide();
    ui->pushButton_22->hide();
    ui->pushButton_23->hide();
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
    ui->pushButton_24->hide();
    ui->pushButton_25->hide();
    ui->pushButton_26->hide();
    ui->label_21->hide();
    ui->label_22->hide();
    ui->label_25->hide();
    ui->label_26->hide();
    ui->lineEdit_2->hide();
    ui->lineEdit_3->hide();
    ui->pushButton_28->hide();
}

void MainWindow::on_pushButton_15_clicked()
{
    clickedSound();

    ui->comboBox_2->blockSignals(true);
    ui->comboBox_3->blockSignals(true);

    if(ui->pushButton_15->pos().x() == 310){
        QIcon ic4(":/img/PNG-cards-1.3/cross.png");
        ui->pushButton_15->setIcon(ic4);
        ui->pushButton_15->move(257, 30);
        ui->comboBox_2->setCurrentIndex(0);
        ui->comboBox_2->show();
        ui->pushButton_24->show();

        if(difficulty == "Middle"){
            ui->comboBox_3->setCurrentIndex(0);
        }
        else{
            ui->comboBox_3->setCurrentIndex(1);
        }
        ui->comboBox_3->show();

        if(playersCount == 1 && ui->lineEdit->text() != ""){
            ui->pushButton_18->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 2px solid; border-color: black; border-radius: 20px; background-color: white; color: black; font: bold;");
            ui->pushButton_18->setDisabled(false);
        }

        playersCount++;
    }
    else{
        if(ui->comboBox_2->currentIndex() == 1){
            server->setSlotsCounter(server->getSlotsCounter() - 1);
            server->kickClient(ui->label_17);
        }
        QIcon ic4(":/img/PNG-cards-1.3/plus.png");
        ui->pushButton_15->setIcon(ic4);
        ui->pushButton_15->move(310, 30);
        ui->comboBox_2->hide();
        ui->comboBox_3->hide();
        ui->pushButton_21->hide();
        ui->label_17->hide();
        ui->pushButton_24->hide();
        playersCount--;

        if(playersCount == 1){
            ui->pushButton_18->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 20px; color: white; font: bold;");
            ui->pushButton_18->setDisabled(true);
        }
        else{
            startCheck(0);
        }
    }

    ui->comboBox_2->blockSignals(false);
    ui->comboBox_3->blockSignals(false);

    if(server){
        server->HubChanged();
    }
}

void MainWindow::on_pushButton_16_clicked()
{
    clickedSound();

    ui->comboBox_4->blockSignals(true);
    ui->comboBox_5->blockSignals(true);

    if(ui->pushButton_16->pos().x() == 90){
        QIcon ic4(":/img/PNG-cards-1.3/cross.png");
        ui->pushButton_16->setIcon(ic4);
        ui->pushButton_16->move(37, 190);
        ui->comboBox_5->setCurrentIndex(0);
        ui->comboBox_5->show();
        ui->pushButton_25->show();

        if(difficulty == "Middle"){
            ui->comboBox_4->setCurrentIndex(0);
        }
        else{
            ui->comboBox_4->setCurrentIndex(1);
        }
        ui->comboBox_4->show();

        if(playersCount == 1){
            ui->pushButton_18->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 2px solid; border-color: black; border-radius: 20px; background-color: white; color: black; font: bold;");
            ui->pushButton_18->setDisabled(false);
        }

        playersCount++;
    }
    else{
        if(ui->comboBox_5->currentIndex() == 1){
            server->setSlotsCounter(server->getSlotsCounter() - 1);
            server->kickClient(ui->label_18);
        }
        QIcon ic4(":/img/PNG-cards-1.3/plus.png");
        ui->pushButton_16->setIcon(ic4);
        ui->pushButton_16->move(90, 190);
        ui->comboBox_5->hide();
        ui->comboBox_4->hide();
        ui->pushButton_22->hide();
        ui->label_18->hide();
        ui->pushButton_25->hide();
        playersCount--;

        if(playersCount == 1){
            ui->pushButton_18->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 20px; color: white; font: bold;");
            ui->pushButton_18->setDisabled(true);
        }
        else{
            startCheck(0);
        }
    }

    ui->comboBox_4->blockSignals(false);
    ui->comboBox_5->blockSignals(false);

    if(server){
        server->HubChanged();
    }
}

void MainWindow::on_pushButton_17_clicked()
{
    clickedSound();

    ui->comboBox_6->blockSignals(true);
    ui->comboBox_7->blockSignals(true);

    if(ui->pushButton_17->pos().x() == 530){
        QIcon ic4(":/img/PNG-cards-1.3/cross.png");
        ui->pushButton_17->setIcon(ic4);
        ui->pushButton_17->move(477, 190);
        ui->comboBox_7->setCurrentIndex(0);
        ui->comboBox_7->show();
        ui->pushButton_26->show();

        if(difficulty == "Middle"){
            ui->comboBox_6->setCurrentIndex(0);
        }
        else{
            ui->comboBox_6->setCurrentIndex(1);
        }
        ui->comboBox_6->show();

        if(playersCount == 1){
            ui->pushButton_18->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 2px solid; border-color: black; border-radius: 20px; background-color: white; color: black; font: bold;");
            ui->pushButton_18->setDisabled(false);
        }

        playersCount++;
    }
    else{
        if(ui->comboBox_7->currentIndex() == 1){
            server->setSlotsCounter(server->getSlotsCounter() - 1);
            server->kickClient(ui->label_19);
        }
        QIcon ic4(":/img/PNG-cards-1.3/plus.png");
        ui->pushButton_17->setIcon(ic4);
        ui->pushButton_17->move(530, 190);
        ui->comboBox_7->hide();
        ui->comboBox_6->hide();
        ui->pushButton_23->hide();
        ui->label_19->hide();
        ui->pushButton_26->hide();
        playersCount--;

        if(playersCount == 1){
            ui->pushButton_18->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 20px; color: white; font: bold;");
            ui->pushButton_18->setDisabled(true);
        }
        else{
            startCheck(0);
        }
    }

    ui->comboBox_6->blockSignals(false);
    ui->comboBox_7->blockSignals(false);

    if(server){
        server->HubChanged();
    }
}

void MainWindow::on_pushButton_18_clicked()
{
    std::ofstream fout;
    fout.open(dirName.toStdString());

    if(fout.is_open()){
        fout << ui->lineEdit->text().toStdString();
    }
    fout.close();

    ui->label_13->show();
    playerCreator();
    scaleForMany();

    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    ui->label_3->hide();
    ui->label_4->hide();

    if(ui->pushButton_20->text() != "Unload"){
        ColodCardsSize = 0;
        tableCardsSize = 0;

        PointsX = 1;
        ui->label_7->setText(QString::fromStdString("Points " + std::to_string(PointsX) + "x"));

        Start();
        int mv = Mmove;
        if(dynamic_cast<Bot*>(players[Mmove - 1]) && tableCards[0][0] == 'J'){
            std::string (&botCards)[36] = players[Mmove - 1]->getCards();

            int sign[4];
            sign[0] = 0;
            sign[1] = 0;
            sign[2] = 0;
            sign[3] = 0;
            for(int i = 0; i < players[Mmove - 1]->getCardsSize(); i++){
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
        AutoSave();
    }
    else{
        std::ifstream fin;
        fin.open(dirGame.toStdString());

        if(fin.is_open()){

            int bucketInt;
            bool bucketBool;
            std::string bucket;

            fin >> bucket;

            fin >> playersCount;

            for(Player* playe : players){
                fin >> bucket;

                if(bucket != "empty"){
                    playe->setDifficulty(bucket);
                }
            }

            fin >> playersCount;

            int index = 0;

            for(Player* playe : players){
                fin >> bucketInt;
                playe->setPass(bucketInt);

                fin >> bucketBool;
                playe->setInGame(bucketBool);

                fin.ignore();

                std::getline(fin, bucket);
                playe->setName(bucket);

                fin >> bucketInt;
                for(int i = 0; i < bucketInt; i++){
                    fin >> bucket;
                    playe->addCard(bucket);
                    if(index == 0){
                        onAddWidgetPlayer(playe, bucket, i);
                    }
                    else{
                        onAddWidgetBot(playe, i);
                    }
                }

                fin >> bucketBool;
                playe->setCheckForTake(bucketBool);

                fin >> bucketInt;
                playe->setPoints(bucketInt);

                if(index == 0){
                    playe->getLabel()->setText(QString::fromStdString("Your Points: " + std::to_string(bucketInt)));
                }
                else{
                    if (playe->getDifficulty() == "Middle"){
                        if(players.size() == 2){
                            playe->getLabel()->setText(QString::fromStdString(playe->getName() + " | Points: " + std::to_string(bucketInt)));
                        }
                        else{
                            playe->getLabel()->setText("<html><span style='color: orange; font-size: 11pt; font-weight: 900; font-family: 'Segoe UI Black';'>M</span> "
                                                       + QString::fromStdString(playe->getName() + " | Points: " + std::to_string(bucketInt) + "</html>"));
                        }
                    }
                    else if (playe->getDifficulty() == "Hard"){
                        if(players.size() == 2){
                            playe->getLabel()->setText(QString::fromStdString(playe->getName() + " | Points: " + std::to_string(bucketInt)));
                        }
                        else{
                            playe->getLabel()->setText("<html><span style='color: red; font-size: 11pt; font-weight: 900; font-family: 'Segoe UI Black';'>H</span> "
                                                       + QString::fromStdString(playe->getName() + " | Points: " + std::to_string(bucketInt) + "</html>"));
                        }
                    }
                    else{
                        playe->getLabel()->setText(QString::fromStdString(playe->getName() + " | Points: " + std::to_string(bucketInt)));
                    }
                }

                fin >> bucketInt;
                playe->setJackKol(bucketInt);

                index++;
            }

            fin >> ColodCardsSize;
            ui->label->setText(QString::fromStdString(std::to_string(ColodCardsSize)));
            for(int i = 0; i < ColodCardsSize; i++){
                fin >> ColodCards[i];
                onAddWidgetColod(i);
            }
            if (ColodCardsSize == 0) {
                onAddWidgetColod(0);
                isShuffl = true;
                ui->label_4->setText(QString::fromStdString("Shuffling (" + std::to_string(PointsX + 1) + "x)"));
                ui->label_4->show();
            }

            fin >> tableCardsSize;
            for(int i = 0; i < tableCardsSize; i++){
                fin >> tableCards[i];
                onAddWidgetTable(tableCards[i], i);
            }

            fin >> Mmove;
            if(Mmove == 1){
                ui->label_2->setText("Your turn");
                ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(46, 80, 200);  border-radius: 3px; background-color: rgb(0,81,80); color: white; font: bold;");
            }
            else{
                ui->label_2->setText(QString::fromStdString(players[Mmove - 1]->getName()) + "\nturn");
                ui->label_2->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192);  border-radius: 3px; color: white; font: bold;");
            }
            if(isFullscreen && isInGame){
                QString cs = ui->label_2->styleSheet();
                ui->label_2->setStyleSheet(cs + QString(" font-size: %1pt; border-radius: %2px;").arg(8 * scaleFactor).arg(3 * scaleFactor));
            }

            fin >> secMove;
            fin >> Jackchoose;
            if(Jackchoose == "empty")
                Jackchoose = "";

            fin >> PointsX;
            ui->label_7->setText(QString::fromStdString("Points " + std::to_string(PointsX) + "x"));
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
            fin >> Bridge;
            fin >> bucketBool;
            if(bucketBool)
                ui->pushButton_14->show();
        }
        fin.close();
    }

    ui->label_12->show();
    ui->scrollArea->show();
    if(!ui->label_13->isHidden()){
        std::string dif = players[1]->getDifficulty();
        ui->label_13->setText(QString::fromStdString(dif));
        if(dif == "Middle"){
            ui->label_13->setText("M");
            ui->label_13->setStyleSheet("font-weight: 900; font-size: 11pt; font-family: 'Segoe UI Black'; color: orange");
        }
        else if(dif == "Hard"){
            ui->label_13->setText("H");
            ui->label_13->setStyleSheet("font-weight: 900; font-size: 11pt; font-family: 'Segoe UI Black'; color: red");
        }
    }
    if(QSMode)
        ui->label_11->show();
    if(PointsMode)
        ui->label_12->setText("225");
    else
        ui->label_12->setText("125");

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(botMove()));
    timer->start(2000);

    SaveWindowGeometry();

    Resizing();

    qDebug() << tableCards[tableCardsSize - 1];

    if(Bridge && ui->pushButton->isHidden() && Mmove == 1){
        pushButtonBridge();
        ui->pushButton->hide();
        if(isFullscreen){
            ui->pushButton_6->resize(QSize(81 * scaleFactor,31 * scaleFactor));
            ui->pushButton_6->move(QPoint(ui->pushButton_6->pos().x(), ui->pushButton_6->pos().y() + (5 * scaleFactor)));
        }
        else{
            ui->pushButton_6->resize(QSize(81,31));
            ui->pushButton_6->move(QPoint(ui->pushButton_6->pos().x(), ui->pushButton_6->pos().y() + 5));
        }
    }
    else if(Bridge && Mmove == 1){
        pushButtonBridge();
    }
}

void MainWindow::on_pushButton_20_clicked()
{
    clickedSound();

    ui->label_17->setText("");
    ui->label_18->setText("");
    ui->label_19->setText("");

    QString curText = ui->pushButton_20->text();

    if(curText == "Load"){
        ui->comboBox_2->blockSignals(true);
        ui->comboBox_3->blockSignals(true);
        ui->comboBox_4->blockSignals(true);
        ui->comboBox_5->blockSignals(true);
        ui->comboBox_6->blockSignals(true);
        ui->comboBox_7->blockSignals(true);

        std::ifstream fin;
        fin.open(dirGame.toStdString());
        if(fin.is_open()){
            std::string getstr;
            fin >> getstr;
            fin >> getstr;

            playersCount = std::stoi(getstr);

            QIcon ic3(":/img/PNG-cards-1.3/plus.png");
            QIcon ic4(":/img/PNG-cards-1.3/cross.png");
            ui->pushButton_15->setIcon(ic4);
            ui->pushButton_15->move(257, 30);
            ui->pushButton_24->hide();
            ui->comboBox_2->setCurrentIndex(0);
            ui->comboBox_2->show();
            ui->comboBox_3->show();
            ui->comboBox_2->setDisabled(true);
            ui->comboBox_3->setDisabled(true);
            ui->pushButton_15->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 10px; color: white; font: bold;");
            ui->pushButton_15->setDisabled(true);

            ui->pushButton_16->setIcon(ic4);
            ui->pushButton_16->move(37, 190);
            ui->pushButton_25->hide();
            ui->comboBox_5->setCurrentIndex(0);
            ui->comboBox_5->show();
            ui->comboBox_4->show();
            ui->comboBox_5->setDisabled(true);
            ui->comboBox_4->setDisabled(true);
            ui->pushButton_16->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 10px; color: white; font: bold;");
            ui->pushButton_16->setDisabled(true);

            ui->pushButton_17->setIcon(ic4);
            ui->pushButton_17->move(477, 190);
            ui->pushButton_26->hide();
            ui->comboBox_7->setCurrentIndex(0);
            ui->comboBox_7->show();
            ui->comboBox_6->show();
            ui->comboBox_7->setDisabled(true);
            ui->comboBox_6->setDisabled(true);
            ui->pushButton_17->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 10px; color: white; font: bold;");
            ui->pushButton_17->setDisabled(true);

            ui->lineEdit->setDisabled(true);

            if(getstr == "4"){
                fin >> getstr;

                fin >> getstr;
                if(getstr == "Middle"){
                    ui->comboBox_4->setCurrentIndex(0);
                }
                else if(getstr == "Hard"){
                    ui->comboBox_4->setCurrentIndex(1);
                }
                else{
                    ui->comboBox_5->setCurrentIndex(1);
                    ui->comboBox_4->hide();
                }

                fin >> getstr;
                if(getstr == "Middle"){
                    ui->comboBox_3->setCurrentIndex(0);
                }
                else if(getstr == "Hard"){
                    ui->comboBox_3->setCurrentIndex(1);
                }
                else{
                    ui->comboBox_2->setCurrentIndex(1);
                    ui->comboBox_3->hide();
                }

                fin >> getstr;
                if(getstr == "Middle"){
                    ui->comboBox_6->setCurrentIndex(0);
                }
                else if(getstr == "Hard"){
                    ui->comboBox_6->setCurrentIndex(1);
                }
                else{
                    ui->comboBox_7->setCurrentIndex(1);
                    ui->comboBox_6->hide();
                }
            }
            else if(getstr == "3"){
                ui->pushButton_16->move(90, 190);
                ui->pushButton_16->setIcon(ic3);
                ui->comboBox_5->hide();
                ui->comboBox_4->hide();

                fin >> getstr;

                fin >> getstr;
                if(getstr == "Middle"){
                    ui->comboBox_3->setCurrentIndex(0);
                }
                else if(getstr == "Hard"){
                    ui->comboBox_3->setCurrentIndex(1);
                }
                else{
                    ui->comboBox_2->setCurrentIndex(1);
                    ui->comboBox_3->hide();
                }

                fin >> getstr;
                if(getstr == "Middle"){
                    ui->comboBox_6->setCurrentIndex(0);
                }
                else if(getstr == "Hard"){
                    ui->comboBox_6->setCurrentIndex(1);
                }
                else{
                    ui->comboBox_7->setCurrentIndex(1);
                    ui->comboBox_6->hide();
                }
            }
            else {
                ui->pushButton_16->setIcon(ic3);
                ui->pushButton_16->move(90, 190);
                ui->pushButton_17->setIcon(ic3);
                ui->pushButton_17->move(530, 190);
                ui->comboBox_5->hide();
                ui->comboBox_4->hide();
                ui->comboBox_7->hide();
                ui->comboBox_6->hide();

                fin >> getstr;

                fin >> getstr;
                if(getstr == "Middle"){
                    ui->comboBox_3->setCurrentIndex(0);
                }
                else if(getstr == "Hard"){
                    ui->comboBox_3->setCurrentIndex(1);
                }
                else{
                    ui->comboBox_2->setCurrentIndex(1);
                    ui->comboBox_3->hide();
                }
            }

            fin >> getstr;
            fin >> getstr;
            fin >> getstr;
            fin.ignore();
            std::getline(fin, getstr);
            ui->lineEdit->setText(QString::fromStdString(getstr));
        }
        fin.close();

        ui->pushButton_18->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 2px solid; border-color: black; border-radius: 20px; background-color: white; color: black; font: bold;");
        ui->pushButton_18->setDisabled(false);

        curText = "Unload";

        ui->comboBox_2->blockSignals(false);
        ui->comboBox_3->blockSignals(false);
        ui->comboBox_4->blockSignals(false);
        ui->comboBox_5->blockSignals(false);
        ui->comboBox_6->blockSignals(false);
        ui->comboBox_7->blockSignals(false);
    }
    else{
        curText = "Load";

        playersCount = 1;

        ui->lineEdit->setDisabled(false);

        ui->comboBox_2->setDisabled(false);
        ui->comboBox_3->setDisabled(false);
        ui->comboBox_5->setDisabled(false);
        ui->comboBox_4->setDisabled(false);
        ui->comboBox_7->setDisabled(false);
        ui->comboBox_6->setDisabled(false);

        ui->comboBox_2->hide();
        ui->comboBox_3->hide();
        ui->comboBox_5->hide();
        ui->comboBox_4->hide();
        ui->comboBox_7->hide();
        ui->comboBox_6->hide();
        ui->pushButton_21->hide();
        ui->pushButton_22->hide();
        ui->pushButton_23->hide();
        ui->label_17->hide();
        ui->label_18->hide();
        ui->label_19->hide();

        QIcon ic3(":/img/PNG-cards-1.3/plus.png");
        ui->pushButton_15->setIcon(ic3);
        ui->pushButton_16->setIcon(ic3);
        ui->pushButton_17->setIcon(ic3);

        ui->pushButton_15->setDisabled(false);
        ui->pushButton_16->setDisabled(false);
        ui->pushButton_17->setDisabled(false);

        ui->pushButton_15->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 10px; background-color: rgb(230,230,230); color: black; font: bold;");
        ui->pushButton_16->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 10px; background-color: rgb(230,230,230); color: black; font: bold;");
        ui->pushButton_17->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 10px; background-color: rgb(230,230,230); color: black; font: bold;");

        ui->pushButton_15->show();
        ui->pushButton_15->move(310, 30);
        ui->pushButton_16->show();
        ui->pushButton_16->move(90, 190);
        ui->pushButton_17->show();
        ui->pushButton_17->move(530, 190);
        ui->pushButton_18->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 20px; color: white; font: bold;");
        ui->pushButton_18->setDisabled(true);

        std::ifstream fin;
        fin.open(dirName.toStdString());
        if(fin.is_open()){
            std::string name;
            std::getline(fin, name);
            ui->lineEdit->setText(QString::fromStdString(name));
        }
        fin.close();
    }

    ui->pushButton_20->setText(curText);
}

void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    if(server){
        server->HubChanged();
    }
    if(arg1 != "" && playersCount > 1){
        startCheck(0);
    }
    else{
        ui->pushButton_18->setStyleSheet("font-family: 'Segoe UI'; font-size: 13pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 20px; color: white; font: bold;");
        ui->pushButton_18->setDisabled(true);
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    clickedSound();

    players[0]->setJackKol(0);

    gameEnd();
}

void MainWindow::on_pushButton_14_clicked()
{
    clickedSound();

    if(speedUp){
        speedUp = false;
        ui->pushButton_14->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 10px; background-color: rgb(230,230,230); color: black; font: bold;");
    }
    else{
        speedUp = true;
        ui->pushButton_14->setStyleSheet("font-family: 'Segoe UI'; font-size: 8pt; border: 1px solid; border-color: rgb(113,178,235); border-radius: 10px; background-color: rgb(147,195,237); color: black; font: bold;");
    }
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    if(server){
        int counter = server->getSlotsCounter();

        startCheck(index);

        if(index == 0){
            server->setSlotsCounter(--counter);
            server->kickClient(ui->label_17);
            ui->comboBox_3->show();
        }
        else{
            server->setSlotsCounter(++counter);
            ui->comboBox_3->hide();
        }

        server->HubChanged();
    }
}

void MainWindow::on_comboBox_5_currentIndexChanged(int index)
{
    if(server){
        int counter = server->getSlotsCounter();

        startCheck(index);

        if(index == 0){
            server->setSlotsCounter(--counter);
            server->kickClient(ui->label_18);
            ui->comboBox_4->show();
        }
        else{
            server->setSlotsCounter(++counter);
            ui->comboBox_4->hide();
        }

        server->HubChanged();
    }
}

void MainWindow::on_comboBox_7_currentIndexChanged(int index)
{
    if(server){
        int counter = server->getSlotsCounter();

        startCheck(index);

        if(index == 0){
            server->setSlotsCounter(--counter);
            server->kickClient(ui->label_19);
            ui->comboBox_6->show();
        }
        else{
            server->setSlotsCounter(++counter);
            ui->comboBox_6->hide();
        }

        server->HubChanged();
    }
}

void MainWindow::on_pushButton_21_clicked()
{
    clickedSound();

    server->kickClient(ui->label_17);
}

void MainWindow::on_pushButton_22_clicked()
{
    clickedSound();

    server->kickClient(ui->label_18);
}

void MainWindow::on_pushButton_23_clicked()
{
    clickedSound();

    server->kickClient(ui->label_19);
}

void MainWindow::swapCombobox(QComboBox* firstUpper, QComboBox* secondUpper, QComboBox* firstLower, QComboBox* secondLower,
                              QPushButton* firstBigButton, QPushButton* secondBigButton, QLabel* firstLabel, QLabel* secondLabel,
                              QPushButton* firstKickButton,QPushButton* secondKickButton, QPushButton* firstSwapButton,
                              QPushButton* secondSwapButton, int first_x1, int first_y1, int first_x2,
                              int first_y2, int second_x1, int second_y1, int second_x2, int second_y2)
{
    if(!firstUpper->isHidden() && !secondUpper->isHidden()){
        int tempIndex = firstUpper->currentIndex();
        if(secondLower->isHidden()){
            if(!firstLower->isHidden()){
                firstLower->hide();
                secondLower->show();
                secondLower->setCurrentIndex(firstLower->currentIndex());
            }
        }
        else{
            int tempIdex2 = secondLower->currentIndex();
            if(firstLower->isHidden()){
                secondLower->hide();
                firstLower->show();
                firstLower->setCurrentIndex(tempIdex2);
            }
            else{
                secondLower->setCurrentIndex(firstLower->currentIndex());
                firstLower->setCurrentIndex(tempIdex2);
            }
        }

        if(!firstKickButton->isHidden()){
            if(secondKickButton->isHidden()){
                firstKickButton->hide();
                secondKickButton->show();
                secondLabel->setText(firstLabel->text());
                secondLabel->show();
                firstLabel->setText("");
                firstLabel->hide();
            }
            else{
                QString name = firstLabel->text();
                firstLabel->setText(secondLabel->text());
                secondLabel->setText(name);
            }
            server->swapClients(firstLabel, secondLabel);
        }
        else if(!secondKickButton->isHidden()){
            secondKickButton->hide();
            firstKickButton->show();
            firstLabel->setText(secondLabel->text());
            firstLabel->show();
            secondLabel->setText("");
            secondLabel->hide();
            server->swapClients(secondLabel, firstLabel);
        }

        firstUpper->setCurrentIndex(secondUpper->currentIndex());
        secondUpper->setCurrentIndex(tempIndex);
    }
    else if(firstUpper->isHidden() && !secondUpper->isHidden()){
        QIcon ic4(":/img/PNG-cards-1.3/plus.png");
        secondBigButton->setIcon(ic4);
        secondBigButton->move(second_x1, second_y1);
        secondUpper->hide();
        secondSwapButton->hide();

        QIcon ic5(":/img/PNG-cards-1.3/cross.png");
        firstBigButton->setIcon(ic5);
        firstBigButton->move(first_x2, first_y2);
        firstUpper->show();
        firstSwapButton->show();

        if(!secondLower->isHidden()){
            firstLower->show();
            firstLower->setCurrentIndex(secondLower->currentIndex());
        }
        secondLower->hide();

        if(!secondKickButton->isHidden()){
            secondKickButton->hide();
            firstKickButton->show();
            firstLabel->setText(secondLabel->text());
            firstLabel->show();
            secondLabel->setText("");
            secondLabel->hide();
            server->swapClients(secondLabel, firstLabel);
        }

        firstUpper->setCurrentIndex(secondUpper->currentIndex());
    }
    else if(!firstUpper->isHidden() && secondUpper->isHidden()){
        QIcon ic4(":/img/PNG-cards-1.3/plus.png");
        firstBigButton->setIcon(ic4);
        firstBigButton->move(first_x1, first_y1);
        firstUpper->hide();
        firstSwapButton->hide();

        QIcon ic5(":/img/PNG-cards-1.3/cross.png");
        secondBigButton->setIcon(ic5);
        secondBigButton->move(second_x2, second_y2);
        secondUpper->show();
        secondSwapButton->show();

        if(!firstLower->isHidden()){
            secondLower->show();
            secondLower->setCurrentIndex(firstLower->currentIndex());
        }
        firstLower->hide();

        if(!firstKickButton->isHidden()){
            firstKickButton->hide();
            secondKickButton->show();
            secondLabel->setText(firstLabel->text());
            secondLabel->show();
            firstLabel->setText("");
            firstLabel->hide();
            server->swapClients(firstLabel, secondLabel);
        }

        secondUpper->setCurrentIndex(firstUpper->currentIndex());
    }
}

void MainWindow::on_pushButton_24_clicked()
{
    clickedSound();

    if(server){
        ui->comboBox_4->blockSignals(true);
        ui->comboBox_5->blockSignals(true);
        ui->comboBox_6->blockSignals(true);
        ui->comboBox_7->blockSignals(true);

        swapCombobox(ui->comboBox_5, ui->comboBox_7, ui->comboBox_4, ui->comboBox_6, ui->pushButton_16, ui->pushButton_17, ui->label_18,
                     ui->label_19, ui->pushButton_22, ui->pushButton_23, ui->pushButton_25, ui->pushButton_26, 90, 190, 37, 190, 530, 190, 477, 190);

        ui->comboBox_4->blockSignals(false);
        ui->comboBox_5->blockSignals(false);
        ui->comboBox_6->blockSignals(false);
        ui->comboBox_7->blockSignals(false);

        server->HubChanged();
    }
    else if(client){
        client->swapPressed("1");
    }
}

void MainWindow::on_pushButton_25_clicked()
{
    clickedSound();

    if(server){
        ui->comboBox_2->blockSignals(true);
        ui->comboBox_3->blockSignals(true);
        ui->comboBox_4->blockSignals(true);
        ui->comboBox_5->blockSignals(true);
        ui->comboBox_6->blockSignals(true);
        ui->comboBox_7->blockSignals(true);

        swapCombobox(ui->comboBox_5, ui->comboBox_7, ui->comboBox_4, ui->comboBox_6, ui->pushButton_16, ui->pushButton_17, ui->label_18,
                     ui->label_19, ui->pushButton_22, ui->pushButton_23, ui->pushButton_25, ui->pushButton_26, 90, 190, 37, 190, 530, 190, 477, 190);
        swapCombobox(ui->comboBox_5, ui->comboBox_2, ui->comboBox_4, ui->comboBox_3, ui->pushButton_16, ui->pushButton_15, ui->label_18,
                     ui->label_17, ui->pushButton_22, ui->pushButton_21, ui->pushButton_25, ui->pushButton_24, 90, 190, 37, 190, 310, 30, 257, 30);

        ui->comboBox_2->blockSignals(false);
        ui->comboBox_3->blockSignals(false);
        ui->comboBox_4->blockSignals(false);
        ui->comboBox_5->blockSignals(false);
        ui->comboBox_6->blockSignals(false);
        ui->comboBox_7->blockSignals(false);

        server->HubChanged();
    }
    else if(client){
        client->swapPressed("2");
    }
}

void MainWindow::on_pushButton_26_clicked()
{
    clickedSound();

    if(server){
        ui->comboBox_2->blockSignals(true);
        ui->comboBox_3->blockSignals(true);
        ui->comboBox_4->blockSignals(true);
        ui->comboBox_5->blockSignals(true);
        ui->comboBox_6->blockSignals(true);
        ui->comboBox_7->blockSignals(true);

        swapCombobox(ui->comboBox_5, ui->comboBox_7, ui->comboBox_4, ui->comboBox_6, ui->pushButton_16, ui->pushButton_17, ui->label_18,
                     ui->label_19, ui->pushButton_22, ui->pushButton_23, ui->pushButton_25, ui->pushButton_26, 90, 190, 37, 190, 530, 190, 477, 190);
        swapCombobox(ui->comboBox_7, ui->comboBox_2, ui->comboBox_6, ui->comboBox_3, ui->pushButton_17, ui->pushButton_15, ui->label_19,
                     ui->label_17, ui->pushButton_23, ui->pushButton_21, ui->pushButton_26, ui->pushButton_24, 530, 190, 477, 190, 310, 30, 257, 30);

        ui->comboBox_2->blockSignals(false);
        ui->comboBox_3->blockSignals(false);
        ui->comboBox_4->blockSignals(false);
        ui->comboBox_5->blockSignals(false);
        ui->comboBox_6->blockSignals(false);
        ui->comboBox_7->blockSignals(false);

        server->HubChanged();
    }
    else if(client){
        client->swapPressed("3");
    }
}

void MainWindow::on_pushButton_27_clicked()
{
    clickedSound();

    QString text = ui->label_20->text();
    QString afterSpace = text.section(' ', 1);

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(afterSpace);
}

void MainWindow::on_pushButton_13_clicked()
{
    clickedSound();

    ui->pushButton_11->show();
    ui->pushButton_11->move(630, 20);
    ui->label_21->show();
    ui->label_22->show();
    ui->lineEdit_2->show();
    ui->lineEdit_3->show();
    ui->pushButton_28->show();
    ui->pushButton_19->show();

    ui->pushButton_12->hide();
    ui->pushButton_13->hide();
    ui->pushButton_7->hide();
    ui->pushButton_8->hide();
    ui->label_9->hide();
    ui->label_10->hide();
    ui->label_16->hide();
    ui->line->hide();
    ui->line_2->hide();

    ui->pushButton_28->setDisabled(true);
    ui->pushButton_28->setStyleSheet("font-family: 'Segoe UI'; font-size: 10pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 20px; color: white; font: bold;");

    std::ifstream fin;
    fin.open(dirName.toStdString());
    if(fin.is_open()){
        std::string name;
        std::getline(fin, name);
        ui->lineEdit_3->setText(QString::fromStdString(name));
    }
    fin.close();

    QHostAddress addr;
    if(addr.setAddress(ui->lineEdit_2->text()) && ui->lineEdit_3->text() != ""){
        ui->pushButton_28->setStyleSheet("font-family: 'Segoe UI'; font-size: 10pt; border: 2px solid; border-color: black; border-radius: 20px; background-color: white; color: black; font: bold;");
        ui->pushButton_28->setDisabled(false);
    }
}

void MainWindow::on_lineEdit_2_textEdited(const QString &arg1)
{
    QHostAddress addr;
    if(addr.setAddress(arg1) && ui->lineEdit_3->text() != ""){
        ui->pushButton_28->setStyleSheet("font-family: 'Segoe UI'; font-size: 10pt; border: 2px solid; border-color: black; border-radius: 20px; background-color: white; color: black; font: bold;");
        ui->pushButton_28->setDisabled(false);
    }
    else{
        ui->pushButton_28->setStyleSheet("font-family: 'Segoe UI'; font-size: 10pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 20px; color: white; font: bold;");
        ui->pushButton_28->setDisabled(true);
    }
}

void MainWindow::on_lineEdit_3_textEdited(const QString &arg1)
{
    QHostAddress addr;
    if(addr.setAddress(ui->lineEdit_2->text()) && arg1 != ""){
        ui->pushButton_28->setStyleSheet("font-family: 'Segoe UI'; font-size: 10pt; border: 2px solid; border-color: black; border-radius: 20px; background-color: white; color: black; font: bold;");
        ui->pushButton_28->setDisabled(false);
    }
    else{
        ui->pushButton_28->setStyleSheet("font-family: 'Segoe UI'; font-size: 10pt; border: 1px solid; border-color: rgb(192,192,192); border-radius: 20px; color: white; font: bold;");
        ui->pushButton_28->setDisabled(true);
    }
}

void MainWindow::on_pushButton_28_clicked()
{
    clickedSound();

    client = new GameClient();

    ui->pushButton_11->hide();
    ui->label_21->hide();
    ui->label_22->hide();
    ui->lineEdit_2->hide();
    ui->lineEdit_3->hide();
    ui->pushButton_28->hide();
    ui->pushButton_19->hide();

    movie = new QMovie(":/img/PNG-cards-1.3/loading.gif");
    ui->label_23->setMovie(movie);
    ui->label_23->setScaledContents(true);
    movie->start();
    ui->label_23->show();
    ui->label_24->show();

    QString str = ui->lineEdit_3->text();

    std::ofstream fout;
    fout.open(dirName.toStdString());

    if(fout.is_open()){
        fout << str.toStdString();
    }
    fout.close();

    client->setMain(this);
    client->setName(str);

    client->connectToServer(ui->lineEdit_2->text(), 55555);
}

void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    if(server){
        server->HubChanged();
    }
}

void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
    if(server){
        server->HubChanged();
    }
}

void MainWindow::on_comboBox_6_currentIndexChanged(int index)
{
    if(server){
        server->HubChanged();
    }
}
