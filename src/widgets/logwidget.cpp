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
#include <QTime>
#include <QVBoxLayout>

LogWidget::LogWidget(QTemporaryFile *tempFile, QWidget *parent) :
    QWidget(parent),
    logFile(tempFile)
{
    textLog = new QPlainTextEdit;
    textLog->setReadOnly(true);
    textLog->setMaximumBlockCount(1000);

    auto newButton = new QPushButton(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("Save Session Log"));
    connect(newButton, &QPushButton::clicked, this, &LogWidget::savePressed);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textLog);
    mainLayout->addWidget(newButton);

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
    /*
    A QTemporaryFile will always be opened in QIODevice::ReadWrite mode,
    this allows easy access to the data in the file. This function will
    return true upon success and will set the fileName() to the unique
    filename used.
    */
    logFile->open();
    logFile->seek(logFile->size());
    logFile->write(text.toLatin1());
    logFile->putChar('\n');
    logFile->close();
}

void LogWidget::savePressed()
{
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
