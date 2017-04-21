/* AtCore Test Client
    Copyright (C) <2016>

    Authors:
        Lays Rodrigues <laysrodrigues@gmail.com>
        Patrick José Pereira <patrickelectric@gmail.com>
        Chris Rizzitello <sithlord48@gmail.com>
        Tomaz Canabrava <tcanabrava@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QApplication>
#include <QDebug>
#include <KAboutData>
#include <KLocalizedString>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QCoreApplication::setApplicationName(QStringLiteral("AtCore - KDE Print Service"));

    KAboutData aboutData(
        // The program name used internally. (componentName)
        QStringLiteral("atcore"),
        // A displayable program name string. (displayName)
        i18n("Atcore"),
        // The program version string. (version)
        QStringLiteral("1.0"),
        // Short description of what the app does. (shortDescription)
        i18n("Test Client of AtCore"),
        // The license this code is released under
        KAboutLicense::GPL,
        // Copyright Statement (copyrightStatement = QString())
        i18n("(c) 2017"),
        // Optional text shown in the About box.
        // Can contain any information desired. (otherText)
        i18n(""),
        // The program homepage string. (homePageAddress = QString())
        QStringLiteral("http://atelier.kde.org"),
        // The bug report email address
        // (bugsEmailAddress = QLatin1String("submit@bugs.kde.org")
        QStringLiteral("atelier@bugs.kde.org"));
    aboutData.addAuthor(i18n("Lays Rodrigues"), i18n("Developer"), QStringLiteral("laysrodriguessilva@gmail.com"),
                        QStringLiteral("http://laysrodriguesdev.wordpress.com"));
    aboutData.addAuthor(i18n("Chris Rizzitello"), i18n("Developer"), QStringLiteral("sithlord48@gmail.com"),
                        QStringLiteral("http://rizzitello.wordpress.com"));
    aboutData.addAuthor(i18n("Patrick Pereira"), i18n("Developer"), QStringLiteral("patrickelectric@gmail.com"),
                        QStringLiteral("http://patrickjp.com"));
    aboutData.addAuthor(i18n("Tomaz Canabrava"), i18n("Contributor"), QStringLiteral("tomaz@kde.com"),
                        QStringLiteral("http://angrycane.com.br"));
    KAboutData::setApplicationData(aboutData);

    MainWindow *m = new MainWindow();
    m->show();
    return app.exec();
}
