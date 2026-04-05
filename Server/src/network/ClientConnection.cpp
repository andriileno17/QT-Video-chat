#include "ClientConnection.h"

ClientConnection::ClientConnection(QTcpSocket *socket, QObject *parent)
    :QObject(parent), tcpSocket(socket), udpPort(0){
    
    connect(tcpSocket, &QTcpSocket::disconnected, this, &ClientConnection::onDisconnected);
}

ClientConnection::~ClientConnection() {
    if (tcpSocket->isOpen()) {
        tcpSocket->close();
    }
    tcpSocket->deleteLater();
}

QTcpSocket* ClientConnection::getSocket() const {
    return tcpSocket;
}

quint16 ClientConnection::getUdpPort() const {
    return udpPort;
}

void ClientConnection::setUdpPort(quint16 port) {
    udpPort = port;
}

void ClientConnection::onDisconnected() {
    emit disconnected(this);
}