#include <QApplication>
#include <QDebug>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        qDebug() << "Please use: kps port baudrate. (./kpg /dev/ttyUSB0 115200)";
        return -1;
    }

    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("KDE");
    QCoreApplication::setOrganizationDomain("kde.org");
    QCoreApplication::setApplicationName("KPS - KDE Print Service");

    MainWindow *window = new MainWindow(argv[1], atoi(argv[2]));
    return app.exec();
}
