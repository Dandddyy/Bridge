#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "optionwindow.h"
#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QSignalMapper>
#include <QTimer>
#include <QMediaPlayer>
#include <QtMultimedia>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void onAddWidgetPlayer(std::string card, int iter);

    void onAddWidgetBot(int iter);

    void onAddWidgetColod(int iter);

    void onAddWidgetTable(std::string card, int iter);

    void onRemoveWidgetPlayer();

    void onRemoveWidgetColod();

    void onRemoveWidgetColodBot();

    void shuffling();

    void gameEnd();

    bool possibleMove(std::string p, std::string t, bool ndmove);

    void operation(bool mv);

    void secondmove();

    void Start();

    void AutoSave();

    void OptionsSave();

private slots:

    void chooseBestMove();

    void botMove();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_7_clicked();

    void QSMslot(QString text);

    void Pointsslot(QString text);

    void Difficultyslot(QString text);

    void Musicslot(int val);

    void Soundslot(int val);

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

    std::string playerCards[36];
    int playerCardsSize;
    std::string botCards[36];
    int botCardsSize;
    std::string ColodCards[36];
    int ColodCardsSize;
    std::string tableCards[36];
    int tableCardsSize;
    bool move;
    QPushButton* playerButtons[36];
    QPushButton* botButtons[36];
    QPushButton* ColodButtons[36];
    QPushButton* TableButtons[36];
    bool secMove;
    std::string Jackchoose;
    bool checkForTake;
    bool BcheckForTake;
    int playerPoints;
    int botPoints;
    int PointsX;
    int pJackKol;
    int bJackKol;
    QTimer *timer;
    int Set;
    optionwindow *window;
    bool QSMode;
    bool PointsMode;
    std::string difficulty;
    int musicvol;
    int soundvol;
    QString tempFilePathMenu;
    QString tempFilePathTheme;
    QString tempFilePathClick;
    QString tempFilePathCard;
    QString dirOption;
    QString dirGame;

    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QSoundEffect *soundplayer;

signals:
    void QSMsignal2(bool, bool, std::string, int, int);
};
#endif // MAINWINDOW_H
