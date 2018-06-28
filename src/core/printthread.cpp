/* AtCore
   Copyright (C) <2017>

   Authors:
       Chris Rizzitello <rizzitello@kde.org>

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
#include <QTime>
#include <QLoggingCategory>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QRegularExpression>

#include "printthread.h"

Q_LOGGING_CATEGORY(PRINT_THREAD, "org.kde.atelier.core.printThread")
/**
 * @brief The PrintThreadPrivate class
 */
class PrintThreadPrivate
{
public:
    AtCore *core = nullptr;             //!<@param core: Pointer to AtCore
    QTextStream *gcodestream = nullptr; //!<@param gcodestream: Steam the job is read from
    float printProgress = 0;            //!<@param printProgress: Progress of the print job
    qint64 totalSize = 0;               //!<@param totalSize: total file size
    qint64 stillSize = 0;               //!<@param stillSize: remaining file
    QString cline;                      //!<@param cline: current line
    AtCore::STATES state = AtCore::IDLE;//!<@param state: printer state
    QFile *file = nullptr;              //!<@param file: gcode File to stream from
    QList<QCommandLineOption> options = {
        {QCommandLineOption(QStringLiteral("pause"))},
        {QCommandLineOption(QStringLiteral("extruder temperature"))},
        {QCommandLineOption(QStringLiteral("bed temperature"))},
        {QCommandLineOption(QStringLiteral("print speed"))},
        {QCommandLineOption(QStringLiteral("fan speed"))},
        {QCommandLineOption(QStringLiteral("flow rate"))},
        {QCommandLineOption(QStringLiteral("message"))},
        {QCommandLineOption(QStringLiteral("command"))}
    };  //!<@param options: injectable commands.
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
    connect(d->core->firmwarePlugin(), &IFirmware::readyForCommand, this, &PrintThread::processJob, Qt::QueuedConnection);
    connect(this, &PrintThread::nextCommand, d->core, &AtCore::pushCommand, Qt::QueuedConnection);
    connect(this, &PrintThread::stateChanged, d->core, &AtCore::setState, Qt::QueuedConnection);
    connect(d->core, &AtCore::stateChanged, this, &PrintThread::setState, Qt::QueuedConnection);
    connect(this, &PrintThread::finished, this, &PrintThread::deleteLater);
    // force a command if the printer doesn't send "wait" when idle
    processJob();
}

void PrintThread::processJob()
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
        if (!d->cline.isEmpty() && d->core->state() != AtCore::PAUSE) {
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
        if (d->cline.startsWith(QStringLiteral(";-"))) {
            nextLine();
        }
        break;

    default:
        qCDebug(PRINT_THREAD) << "Unknown State";
        break;
    }
}

void PrintThread::endPrint()
{
    emit printProgressChanged(100);
    qCDebug(PRINT_THREAD) << "atEnd";
    disconnect(d->core->firmwarePlugin(), &IFirmware::readyForCommand, this, &PrintThread::processJob);
    disconnect(this, &PrintThread::nextCommand, d->core, &AtCore::pushCommand);
    disconnect(d->core, &AtCore::stateChanged, this, &PrintThread::setState);
    emit stateChanged(AtCore::FINISHEDPRINT);
    emit stateChanged(AtCore::IDLE);
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
    emit printProgressChanged(d->printProgress);

    if (d->cline.startsWith(QStringLiteral(";-"))) {
        injectCommand(d->cline);
        d->cline = QStringLiteral("");
        return;
    }
    //Remove Comments from the gcode.
    //Type 1: Anything after ; is comment.
    //Example G28 Z; Home Axis Z
    if (d->cline.contains(QChar::fromLatin1(';'))) {
        d->cline.resize(d->cline.indexOf(QChar::fromLatin1(';')));
    }
    //Type 2: Block Type anything between ( and ) is a comment
    // Example G28 (Home)Z
    if (d->cline.contains(QChar::fromLatin1('('))) {
        //Remove (.....) from the line
        d->cline.remove(QRegularExpression(QStringLiteral(".(?<=[(])(.*)(?=[)]).")));
    }

    d->cline = d->cline.simplified();
}

void PrintThread::setState(const AtCore::STATES &newState)
{
    if (d->state == AtCore::STATES::DISCONNECTED &&
            (
                newState == AtCore::STATES::PAUSE ||
                newState == AtCore::STATES::STOP
            )
       ) {
        qCDebug(PRINT_THREAD) << "Serial not connected !";
        return;
    }
    if (newState != d->state) {
        qCDebug(PRINT_THREAD) << QStringLiteral("State changed from [%1] to [%2]")
                              .arg(QVariant::fromValue(d->state).value<QString>(),
                                   QVariant::fromValue(newState).value<QString>());
        disconnect(d->core, &AtCore::stateChanged, this, &PrintThread::setState);
        d->state = newState;
        emit stateChanged(d->state);
        connect(d->core, &AtCore::stateChanged, this, &PrintThread::setState, Qt::QueuedConnection);
    }
}

void PrintThread::injectCommand(QString &command)
{
    //remove the ;
    command.remove(0, 1);
    command.prepend(QStringLiteral("0:"));
    QStringList cmd = command.split(QLatin1Char(':'));
    cmd.replace(1, cmd.at(1).simplified().toLower());
    cmd.replace(2, cmd.at(2).simplified());

    static QCommandLineParser parser;
    if (parser.optionNames().isEmpty()) {
        parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
        parser.addOptions(d->options);
    }

    qCDebug(PRINT_THREAD) << "attempting to inject " << cmd;
    parser.process(cmd);

    if (parser.isSet(QStringLiteral("pause"))) {
        d->core->pause(parser.positionalArguments().at(0));
    } else if (parser.isSet(QStringLiteral("extruder temperature"))) {
        QStringList args = parser.positionalArguments().at(0).split(QLatin1Char(','));
        bool wait = !QString::compare(args.at(2).simplified(), QStringLiteral("true"), Qt::CaseInsensitive);
        d->core->setExtruderTemp(args.at(0).toInt(), args.at(1).toInt(), wait);
    } else if (parser.isSet(QStringLiteral("bed temperature"))) {
        QStringList args = parser.positionalArguments().at(0).split(QLatin1Char(','));
        bool wait = !QString::compare(args.at(1).simplified(), QStringLiteral("true"), Qt::CaseInsensitive);
        d->core->setBedTemp(args.at(0).toInt(), wait);
    } else if (parser.isSet(QStringLiteral("print speed"))) {
        d->core->setPrinterSpeed(parser.positionalArguments().at(0).toInt());
    } else if (parser.isSet(QStringLiteral("fan speed"))) {
        d->core->setFanSpeed(parser.positionalArguments().at(0).toInt(), parser.positionalArguments().at(1).toInt());
    } else if (parser.isSet(QStringLiteral("flow rate"))) {
        d->core->setFlowRate(parser.positionalArguments().at(0).toInt());
    } else if (parser.isSet(QStringLiteral("message"))) {
        d->core->showMessage(parser.positionalArguments().at(0));
    } else if (parser.isSet(QStringLiteral("command"))) {
        d->core->pushCommand(parser.positionalArguments().at(0));
    } else {
        qCDebug(PRINT_THREAD) << "Attempted to inject unknown command: " << parser.positionalArguments();
    }
}
