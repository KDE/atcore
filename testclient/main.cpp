/* AtCore Test Client
 * SPDX-License-Identifier: GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 * SPDX-FileCopyrightText: 2016-2017 Lays Rodrigues <lays.rodrigues@kde.org>
 * SPDX-FileCopyrightText: 2016 Patrick José Pereira <patrickjp@kde.org>
 * SPDX-FileCopyrightText: 2016-2018 Chris Rizzitello <rizzitello@kde.org>
 * SPDX-FileCopyrightText: 2016 Tomaz Canabrava <tcanabrava@kde.org>
 */

#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QCoreApplication::setApplicationName(QStringLiteral("AtCore - Test Client"));

    MainWindow window;
    window.show();

    return app.exec();
}
