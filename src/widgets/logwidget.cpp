/* AtCore Test Client
    Copyright (C) <2018>
    Author: Chris Rizzitello - rizzitello@kde.org
            Patrick José Pereira <patrickjp@kde.org>

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
#include "logwidget.h"
#include <QFileDialog>
#include <QPushButton>
#include <QStyle>
#include <QStackedWidget>
#include <QTime>
#include <QToolButton>
#include <QVBoxLayout>

LogWidget::LogWidget(QTemporaryFile *tempFile, QWidget *parent) :
    QWidget(parent),
    logFile(tempFile)
{
    QSize iconSize = QSize(fontMetrics().height(), fontMetrics().height());
    auto *page = new QWidget;
    textLog = new QPlainTextEdit;
    textLog->setReadOnly(true);
    textLog->setMaximumBlockCount(1000);
    auto pageLayout = new QVBoxLayout;
    pageLayout->addWidget(textLog);
    page->setLayout(pageLayout);

    QStackedWidget *mainStack = new QStackedWidget;
    mainStack->insertWidget(0, page);

    page = new QWidget;
    auto textbox = new QTextEdit;
    textbox->setReadOnly(true);
    textbox->setHtml(tr("\
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

    auto saveButton = new QPushButton(QIcon::fromTheme(QStringLiteral("document-save"), style()->standardIcon(QStyle::SP_DialogSaveButton)), tr("Save Session Log"));
    saveButton->setIconSize(iconSize);
    connect(saveButton, &QPushButton::clicked, this, &LogWidget::savePressed);

    auto helpButton = new QToolButton();
    helpButton->setCheckable(true);
    helpButton->setChecked(false);
    helpButton->setIconSize(iconSize);
    helpButton->setIcon(QIcon::fromTheme(QStringLiteral("help-about"), style()->standardIcon(QStyle::SP_DialogHelpButton)));
    connect(helpButton, &QToolButton::clicked, this, [this, mainStack](bool checked) {
        mainStack->setCurrentIndex(checked);
    });

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
    QRegExp _newLine(QChar::fromLatin1('\n'));
    QRegExp _return(QChar::fromLatin1('\r'));
    msg.replace(_newLine, QStringLiteral("\\n"));
    msg.replace(_return, QStringLiteral("\\r"));
    QString message(QStringLiteral("[%1]> %2").arg(getTime(), msg));
    textLog->appendPlainText(message);
    writeTempFile(message);
}

void LogWidget::writeTempFile(QString text)
{
    //Add text to our unsynced string list when that hits 100 sync to the temp file.
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
    logFile->open();
    logFile->seek(logFile->size());
    for (const auto &string : qAsConst(unsyncedStrings)) {
        logFile->write(string.toLatin1());
        logFile->putChar('\n');
    }
    logFile->close();
    unsyncedStrings.clear();
}

void LogWidget::savePressed()
{
    //If saving be sure we have flushed to temp.
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
