/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2017-2018 Chris Rizzitello <rizzitello@kde.org>
*/

#include <QCoreApplication>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QString>
#include <QStyle>

#include "about.h"

About::About(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QStringLiteral("About Atcore"));
    setWindowIcon(QIcon::fromTheme(QStringLiteral("help-about"), style()->standardIcon(QStyle::SP_MessageBoxInformation)));

    auto lbl_version = new QLabel(tr("Version: %1").arg(QCoreApplication::applicationVersion()), this);
    auto lbl_qt_version = new QLabel(tr("Using Qt: %1").arg(QString::fromLatin1(qVersion())), this);
    auto lbl_authors = new QLabel(tr("Authors:\n"
                                     "  Chris Rizzitello <rizzitello@kde.org>\n"
                                     "  Patrick José Pereira <patrickjp@kde.org>\n"
                                     "  Lays Rodrigues <lays.rodrigues@kde.org>\n"
                                     "  Tomaz Canabrava <tcanabrava@kde.org>"),
                                  this);

    auto lbl_icon = new QLabel(this);
    lbl_icon->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lbl_icon->setScaledContents(true);
    lbl_icon->setPixmap(QPixmap(QStringLiteral(":/icon/atcore")));

    auto btn_close = new QPushButton(tr("Close"), this);
    connect(btn_close, &QPushButton::clicked, this, &QDialog::close);

    auto versionInfo = new QVBoxLayout;
    versionInfo->addWidget(lbl_version);
    versionInfo->addWidget(lbl_qt_version);

    auto topLayout = new QVBoxLayout;
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addWidget(lbl_icon);
    topLayout->addItem(versionInfo);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addItem(topLayout);
    mainLayout->addWidget(lbl_authors);
    mainLayout->addWidget(btn_close);

    setLayout(mainLayout);
}
