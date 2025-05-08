#include "gameclient.h"

GameClient::GameClient(QObject *parent)
    : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &GameClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &GameClient::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &GameClient::onDisconnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &GameClient::onErrorOccurred);
}

GameClient::~GameClient()
{
    if (socket) {
        if (socket->state() == QTcpSocket::ConnectedState) {
            socket->disconnectFromHost();
        }
        socket->close();
    }
}

void GameClient::connectToServer(const QString &host, quint16 port)
{
    socket->connectToHost(host, port);
}

void GameClient::sendMessage(const QString &message)
{
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write(message.toUtf8());
    }
}

void GameClient::swapPressed(const QString &button)
{
    sendMessage("SWAP:" + button);
}

void GameClient::cardPressed(const QString &card)
{
    sendMessage("CARD:" + card);
}

void GameClient::colodPressed()
{
    sendMessage("COLOD: ");
}

void GameClient::endPressed()
{
    sendMessage("END: ");
}

void GameClient::bridgePressed()
{
    sendMessage("BRIDGE: ");
}

void GameClient::suitPressed(QString suit)
{
    sendMessage("SUIT:" + suit);
}

void GameClient::onConnected()
{
    qDebug() << "Successful connection to the server!";
    sendMessage("NAME:" + name);
}

void GameClient::onReadyRead()
{
    static QByteArray buffer;
    buffer.append(socket->readAll());

    int index;
    while ((index = buffer.indexOf('\n')) != -1) {
        QByteArray line = buffer.left(index).trimmed();
        buffer.remove(0, index + 1);

        QString msg = QString::fromUtf8(line);
        qDebug() << "Received line:" << msg;

        if (msg == "No free slots." || msg == "You were kicked out.") {
            mainWindow->fromHostMessage(msg, "Message");
            socket->disconnectFromHost();
        }
        else if (msg.startsWith("HUB:")) {
            QString jsonString = msg.mid(QString("HUB:").length());

            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);

            if (error.error != QJsonParseError::NoError) {
                qDebug() << "Error parsing JSON:" << error.errorString();
                return;
            }

            mainWindow->parseHub(doc.object());
        }
        else if(msg.startsWith("GAME:")){
            QString jsonString = msg.mid(QString("GAME:").length());

            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);

            if (error.error != QJsonParseError::NoError) {
                qDebug() << "Error parsing JSON:" << error.errorString();
                return;
            }

            mainWindow->parseGame(doc.object());
        }
        else if(msg.startsWith("GAMEOVER:")){
            QString message = msg.mid(QString("GAMEOVER:").length()).replace("|n|", "\n");
            mainWindow->fromHostMessage(message, "Game over");
        }
        else if(msg.startsWith("SET:")){
            QString message = msg.mid(QString("SET:").length()).replace("|n|", "\n");
            mainWindow->fromHostMessage(message, "Set");
        }
    }
}

void GameClient::onDisconnected()
{
    qDebug() << "Disconnected";
}

void GameClient::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Error:" << socket->errorString();
    mainWindow->connectionError(socket->errorString());
}
