#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QUdpSocket>
#include "ClientConnection.h"

class Server: public QObject{
    Q_OBJECT

    public:
        explicit Server(QObject *parent = nullptr);
        void start(quint16 tcpPort, quint16 udpPort);
        
    private slots:
        void onNewTcpConnection();
        void onClientDisconnected(ClientConnection *client);
        void processPendingDatagrams();

    private:
        QTcpServer *tcpServer;
        QUdpSocket *udpSocket;
        QList<ClientConnection*> clients;
};

#endif