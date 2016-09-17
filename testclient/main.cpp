#include <QApplication>
#include <QDebug>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("KDE");
    QCoreApplication::setOrganizationDomain("kde.org");
    QCoreApplication::setApplicationName("KPS - KDE Print Service");

    MainWindow window;
    window.show();

    return app.exec();
}
