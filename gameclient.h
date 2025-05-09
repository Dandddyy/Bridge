#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include "mainwindow.h"
#include "qobject.h"
#include "qtcpsocket.h"


class GameClient : public QObject
{
    Q_OBJECT

public:
    explicit GameClient(QObject *parent = nullptr);
    ~GameClient();
    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QString &message);
    void setName(QString &param) { name = param; }
    void setMain(MainWindow* param) { mainWindow = param; }
    void swapPressed(const QString &button);
    void cardPressed(const QString &card);
    void colodPressed();
    void endPressed();
    void bridgePressed();
    void suitPressed(QString suit);
    bool isConnected()
    {
        if (socket->state() == QAbstractSocket::ConnectedState) {
            return true;
        }
        return false;
    }

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    MainWindow* mainWindow;
    QString name;
    QTcpSocket *socket;
};

#endif // GAMECLIENT_H
