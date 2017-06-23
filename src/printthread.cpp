/* AtCore
   Copyright (C) <2017>

   Authors:
       Chris Rizzitello <sithlord48@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "printthread.h"
#include "gcodecommands.h"
#include <QLoggingCategory>
#include <QTime>

Q_LOGGING_CATEGORY(PRINT_THREAD, "org.kde.atelier.core.printThread");

class PrintThreadPrivate
{
public:
    AtCore *core = nullptr;
    QTextStream *gcodestream = nullptr;
    float printProgress = 0;
    qint64 totalSize = 0;
    qint64 stillSize = 0;
    QString cline;
    AtCore::STATES state = AtCore::IDLE;
    QFile *file = nullptr;
};

PrintThread::PrintThread(AtCore *parent, QString fileName) : d(new PrintThreadPrivate)
{
    d->core = parent;
    d->state = d->core->state();
    d->file = new QFile(fileName);
    d->file->open(QFile::ReadOnly);
    d->totalSize = d->file->bytesAvailable();
    d->stillSize = d->totalSize;
    d->gcodestream = new QTextStream(d->file);
}

void PrintThread::start()
{
    // we only want to do this when printing
    connect(d->core->plugin(), &IFirmware::readyForCommand, this, &PrintThread::commandReady, Qt::QueuedConnection);
    connect(this, &PrintThread::nextCommand, d->core, &AtCore::pushCommand, Qt::QueuedConnection);
    connect(this, &PrintThread::stateChanged, d->core, &AtCore::setState, Qt::QueuedConnection);
    connect(d->core, &AtCore::stateChanged, this, &PrintThread::setState, Qt::QueuedConnection);
    connect(this, &PrintThread::finished, this, &PrintThread::deleteLater);
    // force a command if the printer doesn't send "wait" when idle
    commandReady();
}

void PrintThread::commandReady()
{
    if (d->gcodestream->atEnd()) {
        endPrint();
    }

    switch (d->state) {
    case AtCore::STARTPRINT:
    case AtCore::IDLE:
    case AtCore::BUSY:
        setState(AtCore::BUSY);
        nextLine();
        while (d->cline.isEmpty() && !d->gcodestream->atEnd()) {
            nextLine();
        }
        if (!d->cline.isEmpty()) {
            qCDebug(PRINT_THREAD) << "cline:" << d->cline;
            emit nextCommand(d->cline);
        }
        break;

    case AtCore::ERRORSTATE:
        qCDebug(PRINT_THREAD) << "Error State";
        break;

    case AtCore::STOP: {
        endPrint();
        break;
    }

    case AtCore::PAUSE:
        break;

    default:
        qCDebug(PRINT_THREAD) << "Unknown State";
        break;
    }
}

void PrintThread::endPrint()
{
    emit(printProgressChanged(100));
    qCDebug(PRINT_THREAD) << "atEnd";
    disconnect(d->core->plugin(), &IFirmware::readyForCommand, this, &PrintThread::commandReady);
    disconnect(this, &PrintThread::nextCommand, d->core, &AtCore::pushCommand);
    disconnect(d->core, &AtCore::stateChanged, this, &PrintThread::setState);
    emit(stateChanged(AtCore::FINISHEDPRINT));
    emit(stateChanged(AtCore::IDLE));
    disconnect(this, &PrintThread::stateChanged, d->core, &AtCore::setState);
    emit finished();

}
void PrintThread::nextLine()
{
    d->cline = d->gcodestream->readLine();
    qCDebug(PRINT_THREAD) << "Nextline:" << d->cline;
    d->stillSize -= d->cline.size() + 1; //remove read chars
    d->printProgress = float(d->totalSize - d->stillSize) * 100.0 / float(d->totalSize);
    qCDebug(PRINT_THREAD) << "progress:" << QString::number(d->printProgress);
    emit(printProgressChanged(d->printProgress));
    if (d->cline.contains(QChar::fromLatin1(';'))) {
        d->cline.resize(d->cline.indexOf(QChar::fromLatin1(';')));
    }
    d->cline = d->cline.simplified();
}

void PrintThread::setState(const AtCore::STATES &newState)
{
    if (newState != d->state) {
        qCDebug(PRINT_THREAD) << "State Changed from [" << d->state << "] to [" << newState << ']';
        disconnect(d->core, &AtCore::stateChanged, this, &PrintThread::setState);
        d->state = newState;
        emit(stateChanged(d->state));
        connect(d->core, &AtCore::stateChanged, this, &PrintThread::setState, Qt::QueuedConnection);
    }
}
