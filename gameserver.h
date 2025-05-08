#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "qobject.h"
#include "qtcpserver.h"
#include "qtcpsocket.h"
#include "mainwindow.h"

class GameServer : public QObject
{
    Q_OBJECT

public:
    explicit GameServer(QObject *parent = nullptr);
    ~GameServer();
    QString startServer();
    int clientsCount() { return clients.size(); }
    void setSlotsCounter(int param) { slotsCounter = param; }
    int getSlotsCounter() { return slotsCounter; }
    void setMain(MainWindow* param) { mainWindow = param; }
    void kickClient(QLabel*);
    void swapClients(QLabel*, QLabel*);
    void HubChanged();
    void GameChanged();
    void setId(QLabel*, int);
    void sendEndMessage(int index);
    void sendSetMessage(int index, const QString &massage);
    void setProcessingPaused(bool param) { processingPaused = param; }

private slots:
    void onNewConnection();
    void onClientDisconnected();
    void onReadyRead();

private:
    MainWindow* mainWindow;
    QTcpServer *server;
    QVector<std::pair<QTcpSocket*, std::pair<QLabel*, int>>> clients;
    int slotsCounter = 0;
    bool gameStarted = false;
    bool processingPaused = false;
};

#endif // GAMESERVER_H
