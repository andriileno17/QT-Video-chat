#include "NetworkManager.h"
#include <QBuffer>

NetworkManager::NetworkManager(QObject *parent): QObject(parent){
    tcpSocket = new QTcpSocket(this);
    udpSocket = new QUdpSocket(this);

    connect(tcpSocket, &QTcpSocket::connected, this, &NetworkManager::onTcpConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &NetworkManager::onTcpDisconnected);
    connect(udpSocket, &QUdpSocket::readyRead, this, &NetworkManager::readUdpDatagrams);
}

void NetworkManager::connectToServer(const QString &host, quint16 tcpPort, quint16 udpPort){
    serverHost = host;
    serverUdpPort = udpPort;
    tcpPort->connectToHost(host, tcpPort);
}

void NetworkManager::sendVideoFrame(const QImage &image){
    if(tcpSocket->state() != QAbstractSocket::ConnectedState) return;

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPEG", 60);

    udpSocket->writeDatagrams(ba, QHostAdress(serverHost), serverUdpPort);
}

void NetworkManager::onTcpConnected(){
    udpSocket->bind(QHostAdress::AnyIPv4, 0);
    emit connected();
}

void NetworkManager::onTcpDisconnected(){
    udpSocket->close();
    emit disconnected();
}

void NetworkManager::readUdpDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());

        QImage image;
        if (image.loadFromData(datagram, "JPEG")) {
            emit remoteFrameReceived(image);
        }
    }
}