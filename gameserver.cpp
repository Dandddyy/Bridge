#include "gameserver.h"

GameServer::GameServer(QObject *parent)
    : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &GameServer::onNewConnection);
}

GameServer::~GameServer()
{
    for (auto& pair : clients) {
        QTcpSocket* socket = pair.first;
        if (socket) {
            socket->disconnectFromHost();
            socket->close();
            socket->deleteLater();
        }
    }

    clients.clear();

    if (server) {
        server->close();
    }
}


QString GameServer::startServer()
{
    if (!server->listen(QHostAddress::Any, 55555)) {
        qDebug() << "The server did not start: " << server->errorString();
    } else {
        qDebug() << "The server is running on port 55555";

        const QList<QHostAddress> &ipList = QNetworkInterface::allAddresses();
        for (const QHostAddress &address : ipList) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress::LocalHost)
                return address.toString();
        }
    }
    return "";
}

void GameServer::kickClient(QLabel *label)
{
    for (const auto& pair : clients) {
        if (pair.second == label) {
            pair.first->write("You were kicked out.\n");
            pair.first->flush();
            pair.first->disconnectFromHost();
            break;
        }
    }
}

void GameServer::swapClients(QLabel* first, QLabel* second)
{
    auto itFirst = std::find_if(clients.begin(), clients.end(),
                                [first](const std::pair<QTcpSocket*, QLabel*>& pair) {
                                    return pair.second == first;
                                });

    if (itFirst == clients.end())
        return;

    auto itSecond = std::find_if(clients.begin(), clients.end(),
                                 [second](const std::pair<QTcpSocket*, QLabel*>& pair) {
                                     return pair.second == second;
                                 });

    if (itSecond != clients.end()) {
        std::swap(itFirst->second, itSecond->second);
    } else {
        itFirst->second = second;
    }
}

void GameServer::HubChanged()
{
    for (auto& pair : clients) {
        QJsonDocument doc(mainWindow->hubJson(pair.second));
        QString message = "HUB:" + QString::fromUtf8(doc.toJson(QJsonDocument::Compact)) + "\n";

        pair.first->write(message.toUtf8());
    }
}

void GameServer::onNewConnection()
{
    QTcpSocket *clientSocket = server->nextPendingConnection();

    if (slotsCounter == clients.size()) {
        clientSocket->write("No free slots.\n");
        clientSocket->flush();
        clientSocket->disconnectFromHost();
        return;
    }

    connect(clientSocket, &QTcpSocket::readyRead, this, &GameServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &GameServer::onClientDisconnected);

    clients.push_back(std::pair(clientSocket, mainWindow->getLabel()));
    qDebug() << "New player joined. Total: " << clients.size();
}

void GameServer::onReadyRead() {
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    QByteArray data = clientSocket->readAll();
    QString message = QString::fromUtf8(data).trimmed();

    QStringList parts = message.split(":", Qt::KeepEmptyParts);
    QString command = parts[0];

    if (command == "NAME") {
        QString name = parts.value(1, "Unknown");

        for (auto& pair : clients) {
            if (pair.first == clientSocket) {
                pair.second->setText(name);
                break;
            }
        }

        HubChanged();
    }
    else if(command == "SWAP"){
        QString button = parts.value(1, "0");

        for (auto& pair : clients) {
            if (pair.first == clientSocket) {
                qDebug() << "SWAP";
                mainWindow->parseSwap(pair.second, button);
                break;
            }
        }
    }
}

void GameServer::onClientDisconnected() {
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    QLabel* disconnectedLabel = nullptr;
    for (const auto& pair : clients) {
        if (pair.first == clientSocket) {
            disconnectedLabel = pair.second;
            break;
        }
    }

    qDebug() << "Client disconnected: " << clientSocket->peerAddress().toString();
    if(disconnectedLabel){
        clients.erase(
            std::remove_if(clients.begin(), clients.end(),
                           [clientSocket](const std::pair<QTcpSocket*, QLabel*>& pair) {
                               return pair.first == clientSocket;
                           }),
            clients.end()
            );
        clientSocket->deleteLater();

        mainWindow->ClientDisconnected(disconnectedLabel);
        HubChanged();
    }
}
