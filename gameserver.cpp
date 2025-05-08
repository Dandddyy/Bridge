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
            socket->disconnect();
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
        if (pair.second.first == label) {
            pair.first->write("You were kicked out.\n");
            pair.first->flush();
            break;
        }
    }
}

void GameServer::swapClients(QLabel* first, QLabel* second)
{
    auto itFirst = std::find_if(clients.begin(), clients.end(),
                                [first](const std::pair<QTcpSocket*, std::pair<QLabel*, int>>& pair) {
                                    return pair.second.first == first;
                                });

    if (itFirst == clients.end())
        return;

    auto itSecond = std::find_if(clients.begin(), clients.end(),
                                 [second](const std::pair<QTcpSocket*, std::pair<QLabel*, int>>& pair) {
                                     return pair.second.first == second;
                                 });

    if (itSecond != clients.end()) {
        std::swap(itFirst->second, itSecond->second);
    } else {
        itFirst->second.first = second;
    }
}

void GameServer::HubChanged()
{
    for (auto& pair : clients) {
        QJsonDocument doc(mainWindow->hubJson(pair.second.first));
        QString message = "HUB:" + QString::fromUtf8(doc.toJson(QJsonDocument::Compact)) + "\n";

        pair.first->write(message.toUtf8());
    }
}

void GameServer::GameChanged()
{
    for (auto& pair : clients) {
        QJsonDocument doc(mainWindow->gameJson(pair.second.second));
        QString message = "GAME:" + QString::fromUtf8(doc.toJson(QJsonDocument::Compact)) + "\n";

        pair.first->write(message.toUtf8());
    }
}

void GameServer::setId(QLabel *label, int id)
{
    for (auto& client : clients) {
        if (client.second.first == label) {
            client.second.second = id;
            return;
        }
    }
}

void GameServer::sendEndMessage(int index)
{
    for (const auto& pair : clients) {
        QString msg = "GAMEOVER:" + mainWindow->endMessage(index, pair.second.second) + "\n";
        pair.first->write(msg.toUtf8());
    }
}

void GameServer::sendSetMessage(int index, const QString &massage)
{
    processingPaused = true;
    for (const auto& pair : clients) {
        if(pair.second.second == index){
            QString msg = "SET:" + massage + "\n";
            pair.first->write(msg.toUtf8());
            break;
        }
    }
}

void GameServer::onNewConnection()
{
    QTcpSocket *clientSocket = server->nextPendingConnection();

    if (slotsCounter == clients.size()) {
        clientSocket->write("No free slots.\n");
        clientSocket->flush();
        return;
    }

    connect(clientSocket, &QTcpSocket::readyRead, this, &GameServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &GameServer::onClientDisconnected);

    clients.push_back(std::pair(clientSocket, std::pair(mainWindow->getLabel(), 0)));
    qDebug() << "New player joined. Total: " << clients.size();
}

void GameServer::onReadyRead() {    
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    if (processingPaused) {
        clientSocket->readAll();
        return;
    }

    QByteArray data = clientSocket->readAll();
    QString message = QString::fromUtf8(data).trimmed();

    QStringList parts = message.split(":", Qt::KeepEmptyParts);
    QString command = parts[0];

    if (command == "NAME") {
        QString name = parts.value(1, "Unknown");

        for (auto& pair : clients) {
            if (pair.first == clientSocket) {
                pair.second.first->setText(name);
                break;
            }
        }

        HubChanged();
    }
    else if(command == "SWAP"){
        QString button = parts.value(1, "0");

        for (auto& pair : clients) {
            if (pair.first == clientSocket) {
                mainWindow->parseSwap(pair.second.first, button);
                break;
            }
        }
    }
    else if(command == "CARD"){
        QString card = parts.value(1, "0");

        for (auto& pair : clients) {
            if (pair.first == clientSocket) {
                mainWindow->parseCard(pair.second.second, card.toStdString());
                break;
            }
        }
    }
    else if(command == "COLOD"){
        for (auto& pair : clients) {
            if (pair.first == clientSocket) {
                if(mainWindow->isMove() - 1 == pair.second.second){
                    mainWindow->onRemoveWidgetColodBot();
                    GameChanged();
                }
                break;
            }
        }
    }
    else if(command == "END"){
        for (auto& pair : clients) {
            if (pair.first == clientSocket) {
                if(mainWindow->isMove() - 1 == pair.second.second){
                    mainWindow->endClicked();
                    mainWindow->setBridge(false);
                    GameChanged();
                }
                break;
            }
        }
    }
    else if(command == "BRIDGE"){
        for (auto& pair : clients) {
            if (pair.first == clientSocket) {
                mainWindow->parseBridge(pair.second.second);
                break;
            }
        }
    }
    else if(command == "SUIT"){
        QString suit = parts.value(1, "0");

        for (auto& pair : clients) {
            if (pair.first == clientSocket) {
                mainWindow->parseSuit(pair.second.second, suit);
                GameChanged();
                break;
            }
        }
    }
}

void GameServer::onClientDisconnected() {
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    QLabel* disconnectedLabel = nullptr;
    int index = 0;
    for (const auto& pair : clients) {
        if (pair.first == clientSocket) {
            disconnectedLabel = pair.second.first;
            index = pair.second.second;
            break;
        }
    }

    qDebug() << "Client disconnected: " << clientSocket->peerAddress().toString();
    if(disconnectedLabel){
        clients.erase(
            std::remove_if(clients.begin(), clients.end(),
                           [clientSocket](const std::pair<QTcpSocket*, std::pair<QLabel*, int>>& pair) {
                               return pair.first == clientSocket;
                           }),
            clients.end()
            );
        clientSocket->deleteLater();

        mainWindow->ClientDisconnected(disconnectedLabel, index);
    }
}
