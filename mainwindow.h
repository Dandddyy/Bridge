#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "optionwindow.h"
#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QSignalMapper>
#include <QTimer>
#include <QMediaPlayer>
#include <QtMultimedia>
#include <QScrollArea>
#include <QEvent>
#include <QWheelEvent>

class Player;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::vector<Player*> &getPlayers() {
        return players;
    }

    void onAddWidgetPlayer(Player* pl, std::string card, int iter);

    void onAddWidgetBot(Player* pl, int iter);

    void onAddWidgetColod(int iter);

    void onAddWidgetTable(std::string card, int iter);

    void onRemoveWidgetPlayer();

    void onRemoveWidgetColod();

    void onRemoveWidgetColodBot();

    void shuffling();

    void gameEnd();

    void cleaner();

    bool possibleMove(std::string p, std::string t, bool ndmove);

    void operation(int mv);

    void secondmove();

    void Start();

    void AutoSave();

    void OptionsSave();

    void Resizing(QString label2Str = "");

    void WidgetsLocation(QSize windowSize);

    void scaleWidget(QWidget* widget);

    void scaleFontnRadiusWidget(QWidget* widget, int fontWidget, int borderRadius);

    void setUiGeo();

    void SaveWindowGeometry();

    void scaleForMany();

    void closeEvent(QCloseEvent *event) override {
        if (window->isVisible()) {
            window->close();
        }
        QMainWindow::closeEvent(event);
    }

    int isMove() const { return Mmove; }

    const std::string getJackchoose() const { return Jackchoose; }

    void setJackchoose(std::string param) { Jackchoose =  param; }

    void setTable(std::string param) {
        tableCards[tableCardsSize] = param;
        onAddWidgetTable(tableCards[tableCardsSize], tableCardsSize);
        tableCardsSize++;
    }

    const std::string* getTable() const { return tableCards; }

    int getTableSize() const { return tableCardsSize; }

    bool isSecMove() const { return secMove; }

    void hideLable3();

    bool isQSM() const { return QSMode; }

    bool isPoints() const { return PointsMode; }

    bool isBridge() const { return Bridge; }

    void setBridge(bool param) { Bridge = param; }

    int getPointsX() const { return PointsX; }

    void lable3Style(QString param);

    void RemoveWidgetBot(int id) {
        onRemoveWidgetColodBot();
    }

    void botNoChoice();

    void cardSound()
    {
        soundplayer->stop();
        soundplayer->setSource(QUrl::fromLocalFile(tempFilePathCard));
        soundplayer->setVolume(soundvol / 100.0);
        soundplayer->play();
    }

    int getPlayercardsSize() const;

    void playerCreator();

    void endClicked(bool end = false);

    int nextOne(int mv);

    int getPlayersCount() const { return playersCount; }

    void pushButtonBridge();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void QSMslot(QString text);

    void Pointsslot(QString text);

    void Difficultyslot(QString text);

    void Musicslot(int val);

    void Soundslot(int val);

    void Displayslot(bool val);

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void botMove();

    void on_pushButton_12_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_20_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_pushButton_6_clicked();

    void on_pushButton_14_clicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::MainWindow *ui;

    const std::string links[4][9] = {
        {":/img/PNG-cards-1.3/6_of_hearts.png", ":/img/PNG-cards-1.3/7_of_hearts.png", ":/img/PNG-cards-1.3/8_of_hearts.png", ":/img/PNG-cards-1.3/9_of_hearts.png", ":/img/PNG-cards-1.3/10_of_hearts.png", ":/img/PNG-cards-1.3/jack_of_hearts2.png", ":/img/PNG-cards-1.3/queen_of_hearts2.png", ":/img/PNG-cards-1.3/king_of_hearts2.png", ":/img/PNG-cards-1.3/ace_of_hearts.png"},
        {":/img/PNG-cards-1.3/6_of_clubs.png", ":/img/PNG-cards-1.3/7_of_clubs.png", ":/img/PNG-cards-1.3/8_of_clubs.png", ":/img/PNG-cards-1.3/9_of_clubs.png", ":/img/PNG-cards-1.3/10_of_clubs.png", ":/img/PNG-cards-1.3/jack_of_clubs2.png", ":/img/PNG-cards-1.3/queen_of_clubs2.png", ":/img/PNG-cards-1.3/king_of_clubs2.png", ":/img/PNG-cards-1.3/ace_of_clubs.png"},
        {":/img/PNG-cards-1.3/6_of_diamonds.png", ":/img/PNG-cards-1.3/7_of_diamonds.png", ":/img/PNG-cards-1.3/8_of_diamonds.png", ":/img/PNG-cards-1.3/9_of_diamonds.png", ":/img/PNG-cards-1.3/10_of_diamonds.png", ":/img/PNG-cards-1.3/jack_of_diamonds2.png", ":/img/PNG-cards-1.3/queen_of_diamonds2.png", ":/img/PNG-cards-1.3/king_of_diamonds2.png", ":/img/PNG-cards-1.3/ace_of_diamonds.png"},
        {":/img/PNG-cards-1.3/6_of_spades.png", ":/img/PNG-cards-1.3/7_of_spades.png", ":/img/PNG-cards-1.3/8_of_spades.png", ":/img/PNG-cards-1.3/9_of_spades.png", ":/img/PNG-cards-1.3/10_of_spades.png", ":/img/PNG-cards-1.3/jack_of_spades2.png", ":/img/PNG-cards-1.3/queen_of_spades2.png", ":/img/PNG-cards-1.3/king_of_spades2.png", ":/img/PNG-cards-1.3/ace_of_spades2.png"}
    };
    const std::string cardNames[4][9] = {
        {"6c", "7c", "8c", "9c", "1c", "Jc", "Qc", "Kc", "Ac"},
        {"6k", "7k", "8k", "9k", "1k", "Jk", "Qk", "Kk", "Ak"},
        {"6b", "7b", "8b", "9b", "1b", "Jb", "Qb", "Kb", "Ab"},
        {"6p", "7p", "8p", "9p", "1p", "Jp", "Qp", "Kp", "Ap"}
    };

    std::vector<Player*> players;
    int playersCount;
    std::string ColodCards[36];
    int ColodCardsSize;
    std::string tableCards[36];
    int tableCardsSize;
    int Mmove;
    QPushButton* ColodButtons[36] = {nullptr};
    QPushButton* TableButtons[36] = {nullptr};
    bool secMove;
    std::string Jackchoose;
    int PointsX;
    QTimer* timer;
    int Set;
    optionwindow *window;
    bool QSMode;
    bool PointsMode;
    std::string difficulty;
    int musicvol;
    int soundvol;
    bool isFullscreen;
    bool isInGame = false;
    bool isFromSet = false;
    bool isShuffl = false;
    bool Bridge = false;
    bool speedUp = false;
    QString tempFilePathMenu;
    QString tempFilePathTheme;
    QString tempFilePathClick;
    QString tempFilePathCard;
    QString dirOption;
    QString dirGame;
    QString dirName;
    QString massBoxStyle = R"(
        QMessageBox {
            background-color: rgb(0,81,44);
        }
        QMessageBox QLabel {
            color: white;
            font-family: 'Segoe UI';
            font-size: 11pt;
        }
        QPushButton {
            width: 100px;
            height: 25px;
            font-family: 'Segoe UI';
            font-size: 10pt;
            border: 2px solid;
            border-color: black;
            border-radius: 20px;
            padding: 6px;
            margin: 4px;
            background-color: white;
            color: black;
            font: bold;
        })";

    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QSoundEffect *soundplayer;
    double scaleFactor;
    QPoint savedMainWindowPoint;
    QSize savedMainWindowSize;
    QRect savedlabel1Geo;
    QRect savedlabel2Geo;
    QRect savedlabel3Geo;
    QRect savedlabel4Geo;
    QRect savedlabel5Geo;
    QRect savedlabel6Geo;
    QRect savedlabel14Geo;
    QRect savedlabel15Geo;
    QRect savedlabel7Geo;
    QRect savedlabel8Geo;
    QRect savedlabel11Geo;
    QRect savedlabel12Geo;
    QRect savedlabel13Geo;
    QRect savedButton1Geo;
    QRect savedButton2Geo;
    QRect savedButton3Geo;
    QRect savedButton4Geo;
    QRect savedButton5Geo;
    QRect savedButton6Geo;
    QRect savedButton10Geo;
    QRect savedButton11Geo;
    QRect savedButton14Geo;
    QString savedLabelStyle;
    QString savedLabel2Style;
    QString savedLabel4Style;
    QString savedLabel5Style;
    QString savedLabel6Style;
    QString savedLabel14Style;
    QString savedLabel15Style;
    QString savedLabel7Style;
    QString savedLabel8Style;
    QString savedLabel12Style;
    QString savedButtonStyle;
    QString savedButton6Style;
    QString savedButton10Style;
    QString savedButton11Style;
    QString savedButton14Style;
    QRect savedLayoutGeo;
    QRect savedLayout2Geo;
    QRect savedLayout3Geo;
    QRect savedLayout4Geo;
    QRect savedVLayoutGeo;
    QRect savedVLayout2Geo;
    QRect savedScroll;
    QSize savedIcon10;
    QSize savedIcon11;
    QSize savedIcon14;

    void clickedSound()
    {
        soundplayer->stop();
        soundplayer->setSource(QUrl::fromLocalFile(tempFilePathClick));
        soundplayer->setVolume(soundvol / 100.0);
        soundplayer->play();
    }

signals:
    void QSMsignal2(bool, bool, std::string, int, int, bool);
};
#endif // MAINWINDOW_H
