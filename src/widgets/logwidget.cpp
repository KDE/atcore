/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2016-2020 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2018 Patrick José Pereira <patrickjp@kde.org>
*/

#include "logwidget.h"

#include <QFileDialog>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QStyle>
#include <QTemporaryFile>
#include <QTime>
#include <QToolButton>
#include <QVBoxLayout>

namespace
{
static constexpr QChar _newLine = QChar::fromLatin1('\n');
static constexpr QChar _return = QChar::fromLatin1('\r');
}

LogWidget::LogWidget(QWidget *parent)
: QWidget(parent)
{
    logFile = new QTemporaryFile();

    initialize();
}

LogWidget::LogWidget(QTemporaryFile *tempFile, QWidget *parent)
    : QWidget(parent)
    , logFile(tempFile)
{
    if (!logFile) {
        logFile = new QTemporaryFile();
    }

    initialize();
}

void LogWidget::initialize()
{
    QSize iconSize = QSize(fontMetrics().height(), fontMetrics().height());
    auto page = new QWidget(this);
    textLog = new QPlainTextEdit(this);
    textLog->setReadOnly(true);
    textLog->setMaximumBlockCount(1000);
    auto pageLayout = new QVBoxLayout;
    pageLayout->addWidget(textLog);
    page->setLayout(pageLayout);

    auto mainStack = new QStackedWidget(this);
    mainStack->insertWidget(0, page);

    page = new QWidget(this);
    auto textbox = new QTextEdit(this);
    textbox->setReadOnly(true);
    textbox->setHtml(
        tr("\
                        <h4>Special Log Entries</h4> \
                        <p><strong>Failed to open device in read/write mode.</strong></p> \
                        <p>&nbsp;&nbsp;&nbsp; The Device was not able to be opened.</p> \
                        <p>&nbsp; &nbsp; &nbsp;&nbsp; Check the device is not opened by another program.</p> \
                        <p>&nbsp; &nbsp; &nbsp;&nbsp; Check you have the correct permissions to open the device.</p> \
                        <p><strong>No plugin found for &lt;Detected Firmware&gt;</strong></p> \
                        <p>&nbsp;&nbsp;&nbsp; Firmware plugins are missing or your firmware is not currently supported.</p> \
                        <p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Manually select the Marlin or Repetier plugin.</p> \
                        <p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; If your firmware does not have a plugin please let us know.</p> \
                        <p><strong>Lots of &ldquo;Waiting for firmware detect&rdquo;</strong></p> \
                        <p>&nbsp;&nbsp;&nbsp; Unable to send the firmware detect waiting for printer to restart</p> \
                        <p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Push the restart button on your printer or turn it on and off. </p> \
                        <p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Select a firmware plugin in place of auto detect.</p>"));
    pageLayout = new QVBoxLayout;
    pageLayout->addWidget(textbox);
    page->setLayout(pageLayout);
    mainStack->insertWidget(1, page);

    auto saveButton = new QPushButton(QIcon::fromTheme(QStringLiteral("document-save"), style()->standardIcon(QStyle::SP_DialogSaveButton)), tr("Save Session Log"), this);
    saveButton->setIconSize(iconSize);
    connect(saveButton, &QPushButton::clicked, this, &LogWidget::savePressed);

    auto helpButton = new QToolButton(this);
    helpButton->setCheckable(true);
    helpButton->setChecked(false);
    helpButton->setIconSize(iconSize);
    helpButton->setIcon(QIcon::fromTheme(QStringLiteral("help-about"), style()->standardIcon(QStyle::SP_DialogHelpButton)));
    connect(helpButton, &QToolButton::clicked, this, [mainStack](bool checked) { mainStack->setCurrentIndex(checked); });

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(helpButton);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mainStack);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
}

QString LogWidget::getTime()
{
    return QTime::currentTime().toString(QStringLiteral("hh:mm:ss:zzz"));
}

void LogWidget::appendLog(const QString &msg)
{
    QString message(QStringLiteral("[%1]  %2").arg(getTime(), msg));
    textLog->appendPlainText(message);
    writeTempFile(message);
}

void LogWidget::appendRLog(const QByteArray &bmsg)
{
    QString message(QStringLiteral("[%1]< %2").arg(getTime(), QString::fromUtf8(bmsg)));
    textLog->appendPlainText(message);
    writeTempFile(message);
}

void LogWidget::appendSLog(const QByteArray &bmsg)
{
    QString msg = QString::fromUtf8(bmsg);
    msg.replace(_newLine, QStringLiteral("\\n"));
    msg.replace(_return, QStringLiteral("\\r"));
    QString message(QStringLiteral("[%1]> %2").arg(getTime(), msg));
    textLog->appendPlainText(message);
    writeTempFile(message);
}

void LogWidget::writeTempFile(const QString &text)
{
    // Add text to our unsynced string list when that hits 100 sync to the temp file.
    unsyncedStrings.append(text);
    if (unsyncedStrings.count() > 100) {
        flushTemp();
    }
}

void LogWidget::flushTemp()
{
    /*
    A QTemporaryFile will always be opened in QIODevice::ReadWrite mode,
    this allows easy access to the data in the file. This function will
    return true upon success and will set the fileName() to the unique
    filename used.
    */
    if (!logFile->open()) {
        qWarning("failed to open temporary log file");
        return;
    }
    logFile->seek(logFile->size());
    for (const auto &string : std::as_const(unsyncedStrings)) {
        logFile->write(string.toLatin1());
        logFile->putChar('\n');
    }
    logFile->close();
    unsyncedStrings.clear();
}

void LogWidget::savePressed()
{
    // If saving be sure we have flushed to temp.
    flushTemp();
    // Note that if a file with the name newName already exists, copy() returns false (i.e. QFile will not overwrite it).
    QString fileName = QDir::homePath() + QChar::fromLatin1('/') + QFileInfo(logFile->fileName()).fileName() + QStringLiteral(".txt");
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save Log to file"), fileName);
    QFile::copy(logFile->fileName(), saveFileName);
    logFile->close();
}

bool LogWidget::endsWith(const QString &string)
{
    return textLog->toPlainText().endsWith(string);
}
