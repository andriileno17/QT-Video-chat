#include <QCoreApplication>
#include <QDebug>
#include "Server.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    qDebug() << "Запуск Qt-Video-chat Сервера...";

    Server server;
    server.start(12345, 12346);

    return a.exec();
}