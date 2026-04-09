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
    tcpSocket->connectToHost(host, tcpPort);
}

void NetworkManager::disconnectFromServer() {
    tcpSocket->disconnectFromHost();
}

void NetworkManager::sendVideoFrame(const QImage &image){
    if(tcpSocket->state() != QAbstractSocket::ConnectedState) return;

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPEG", 60);

    ba.append('V');
    udpSocket->writeDatagram(ba, QHostAddress(serverHost), serverUdpPort);
}

void NetworkManager::sendAudioData(const QByteArray &data){
    if (tcpSocket->state() != QAbstractSocket::ConnectedState) return;

    QByteArray packet;
    packet.append('A');
    packet.append(data);
    
    udpSocket->writeDatagram(packet, QHostAddress(serverHost), serverUdpPort);
}

void NetworkManager::onTcpConnected() {
    udpSocket->bind(QHostAddress::AnyIPv4, 0); 
    
    QByteArray dummy;
    dummy.append('P'); 
    dummy.append("ING");
    udpSocket->writeDatagram(dummy, QHostAddress(serverHost), serverUdpPort);

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

        if(datagram.isEmpty()) continue;

        char packetType = datagram.at(0);

        datagram.remove(0, 1);

        if(packetType == 'V'){
            QImage image;
            if(image.loadFromData(datagram, "JPEG")){
                emit remoteFrameReceived(image);
            }
        } else if (packetType == 'A'){
            emit remoteAudioReceived(datagram);
        }
    }
}