#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include "CameraCapture.h"
#include "NetworkManager.h"

class MainWindow: public QMainWindow{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void onConnectClicked();
        void onDisconnectClicked();

        void onNetworkConnected();
        void onNetworkDisconnected();

        void updateLocalVideo(const QImage &image);
        void updateRemoteVideo(const QImage &image);

    private:
        void setupUi();

        QLabel *remoteVideoLabel;
        QLabel *localVideoLabel;
        QPushButton *connectButton;
        QPushButton *disconnectButton;

        CameraCapture *cameraCapture;
        NetworkManager *networkManager;
};

#endif