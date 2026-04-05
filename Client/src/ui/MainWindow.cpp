#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent){

    setupUi();

    cameraCapture = new CameraCapture(this);
    networkManager = new NetworkManager(this);

    connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);
    
    connect(networkManager, &NetworkManager::connected, this, &MainWindow::onNetworkConnected);
    connect(networkManager, &NetworkManager::disconnected, this, &MainWindow::onNetworkDisconnected);

    connect(cameraCapture, &CameraCapture::frameCaptured, this, &MainWindow::updateLocalVideo);

    connect(cameraCapture, &CameraCapture::frameCaptured, networkManager, &NetworkManager::sendVideoFrame);

    connect(networkManager, &NetworkManager::remoteFrameReceived, this, &MainWindow::updateRemoteVideo);

    cameraCapture->start();
}

MainWindow::~MainWindow(){
    cameraCapture->stop();
}

void MainWindow::onConnectClicked() {
    networkManager->connectToServer("127.0.0.1", 12345, 12346);
    connectButton->setEnabled(false);
}

void MainWindow::onDisconnectClicked() {
    networkManager->disconnectFromServer();
}

void MainWindow::onNetworkConnected() {
    disconnectButton->setEnabled(true);
    remoteVideoLabel->setText("З'єднання встановлено! Очікування відео...");
}

void MainWindow::onNetworkDisconnected() {
    connectButton->setEnabled(true);
    disconnectButton->setEnabled(false);
    remoteVideoLabel->clear();
    remoteVideoLabel->setText("Відключено");
}

void MainWindow::updateLocalVideo(const QImage &image) {
    localVideoLabel->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::updateRemoteVideo(const QImage &image) {
    QImage scaledImage = image.scaled(remoteVideoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    remoteVideoLabel->setPixmap(QPixmap::fromImage(scaledImage));
}

void MainWindow::setupUi(){
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    remoteVideoLabel = new QLabel("Очікування відео ...", this);
    remoteVideoLabel->setMinimumSize(640, 480);
    remoteVideoLabel->setStyleSheet("background-color: black; color: white;");
    remoteVideoLabel->setAlignment(Qt::AlignCenter);

    localVideoLabel = new QLabel("Моя камера", this);
    localVideoLabel->setFixedSize(320, 240);
    localVideoLabel->setStyleSheet("background-color: #222222; color: white;");
    localVideoLabel->setAlignment(Qt::AlignCenter);

    connectButton = new QPushButton("Підключитися", this);
    disconnectButton = new QPushButton("Відключитися", this);
    disconnectButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(connectButton);
    buttonLayout->addWidget(disconnectButton);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(localVideoLabel);
    bottomLayout->addStretch();
    bottomLayout->addLayout(buttonLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(remoteVideoLabel);
    mainLayout->addLayout(bottomLayout);

    setWindowTitle("QT-Video-chat");
    resize(800, 600);
}