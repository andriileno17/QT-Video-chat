#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QImage>

class NetworkManager: public QObject{
    Q_OBJECT

    public:
        explicit NetworkManager(QObject *parent = nullptr);
        void connectToServer(const QString &host, quint16 tcpPort, quint16 udpPort);
        void disconnectFromServer();

    public slots:
        void sendVideoFrame(const QImage &image);

    signals:
        void connected();
        void disconnected();
        void remoteFrameRecieved(const QImage &image);

    private slots:
        void onTcpConnected();
        void onTcpDisconnected();
        void readUdpDatagrams();

    private:
        QTcpSocket *tcpSocket;
        QUdpSocket *udpSocket;
        QString serverHost;
        quint16 serverUdpPort;
};

#endif