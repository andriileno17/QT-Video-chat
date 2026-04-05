#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QTcpSocket>

class ClientConnection: public QObject{
    Q_OBJECT

    public:
        explicit ClientConnection(QTcpSocket *socket, QObject *parent = nullptr);
        ~ClientConnection();

        QTcpSocket* getSocket() const;

        quint16 getUdpPort() const;
        void setUdpPort(quint16 port);
        
    signals:
        void disconnected(ClientConnection *client);

    private slots:
        void onDisconnected();

    private:
        QTcpSocket *tcpSocket;
        quint16 udpPort;
};

#endif
