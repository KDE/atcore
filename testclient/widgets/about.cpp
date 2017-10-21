/* AtCore Test Client
    Copyright (C) <2017>

    Authors:
        Chris Rizzitello <rizzitello@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QCoreApplication>
#include <QStyle>
#include <QLayout>
#include <QPushButton>
#include <QString>
#include <QLabel>

#include "about.h"

About::About(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(QStringLiteral("About Atcore"));
    setWindowIcon(QIcon::fromTheme(QStringLiteral("help-about"), style()->standardIcon(QStyle::SP_MessageBoxInformation)));

    QLabel *lbl_version = new QLabel(QString::fromLatin1("Version: %1").arg(QCoreApplication::applicationVersion()));
    QLabel *lbl_git_version = new QLabel(QString::fromLatin1("GitID: %1").arg(QStringLiteral("GIDIDHERE")));
    QLabel *lbl_qt_version = new QLabel(QString::fromLatin1("Using Qt: %1").arg(QString::fromLatin1(qVersion())));
    QLabel *lbl_authors = new QLabel(QStringLiteral("Authors:\n  Chris Rizzitello <rizzitello@kde.org>\n  Patrick José Pereira <patrickelectric@gmail.com>\n  Lays Rodrigues <laysrodrigues@gmail.com>\n  Tomaz Canabrava <tcanabrava@kde.org>"));

    QLabel *lbl_icon = new QLabel();
    lbl_icon->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lbl_icon->setScaledContents(true);
    lbl_icon->setPixmap(QPixmap(QStringLiteral(":/icon/atcore")));

    QPushButton *btn_close = new QPushButton(QStringLiteral("&Close"));
    connect(btn_close, &QPushButton::clicked, this, &QDialog::close);

    QVBoxLayout *versionInfo = new QVBoxLayout;
    versionInfo->addWidget(lbl_version);
    versionInfo->addWidget(lbl_git_version);
    versionInfo->addWidget(lbl_qt_version);

    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addWidget(lbl_icon);
    topLayout->addItem(versionInfo);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addItem(topLayout);
    mainLayout->addWidget(lbl_authors);
    mainLayout->addWidget(btn_close);

    setLayout(mainLayout);
}

About::~About()
{

}
