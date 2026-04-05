#include "Server.h"
#include <QDebug>

Server::Server(QObject *parent) 
    :QObject(parent){
    
    tcpServer = new QTcpServer(this);
    udpSocket = new QUdpSocket(this);

    connect(tcpServer, &QTcpServer::newConnection, this, &Server::onNewTcpConnection);
    connect(udpSocket, &QUdpSocket::readyRead, this, &Server::processPendingDatagrams);
}

void Server::start(quint16 tcpPort, quint16 udpPort){
    if(tcpServer->listen(QHostAddress::Any, tcpPort)){
        qDebug() << "Tcp Сервер запущено на порту: " << tcpPort;
    } else {
        qDebug() << "Помилка запуску ТСР сервера: " << tcpServer->errorString();
    }

    if(udpSocket->bind(QHostAddress::Any, udpPort)){
        qDebug() << "UDP Сервер запущено на порту: " << udpPort;
    } else {
        qDebug() << "Помилка запуску UDP сервера: " << udpSocket->errorString();
    }
}

void Server::onNewTcpConnection() {
    while (tcpServer->hasPendingConnections()) {
        QTcpSocket *socket = tcpServer->nextPendingConnection();
        ClientConnection *client = new ClientConnection(socket, this);
        
        clients.append(client);
        connect(client, &ClientConnection::disconnected, this, &Server::onClientDisconnected);
        
        qDebug() << "Новий клієнт підключився:" << socket->peerAddress().toString();
    }
}

void Server::onClientDisconnected(ClientConnection *client) {
    qDebug() << "Клієнт відключився:" << client->getSocket()->peerAddress().toString();
    clients.removeOne(client);
    client->deleteLater();
}

void Server::processPendingDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        
        QHostAddress senderAddress;
        quint16 senderPort;
        
        udpSocket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);
        
        for (ClientConnection *client : clients) {
            if (client->getSocket()->peerAddress() == senderAddress) {
                if (client->getUdpPort() == 0) {
                    client->setUdpPort(senderPort);
                    qDebug() << "Знайдено UDP порт для клієнта:" << senderAddress.toString() << ":" << senderPort;
                }
            }
        }
        
        for (ClientConnection *client : clients) {
            quint16 clientUdpPort = client->getUdpPort();
            QHostAddress clientAddress = client->getSocket()->peerAddress();
            
            if (clientUdpPort != 0 && (clientAddress != senderAddress || clientUdpPort != senderPort)) {
                udpSocket->writeDatagram(datagram, clientAddress, clientUdpPort);
            }
        }
    }
}