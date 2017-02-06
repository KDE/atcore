#include "printthread.h"
#include "gcodecommands.h"
#include <QLoggingCategory>

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
    PrinterState state = IDLE;
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
    switch (d->state) {
    case STARTPRINT:
    case IDLE:
    case BUSY:
        setState(BUSY);
        connect(d->core, &AtCore::stateChanged, this, &PrintThread::stateChanged, Qt::QueuedConnection);
        nextLine();
        while (d->cline.isEmpty() && !d->gcodestream->atEnd()) {
            nextLine();
        }
        if (!d->cline.isEmpty()) {
            qCDebug(PRINT_THREAD) << "cline:" << d->cline;
            emit nextCommand(d->cline);
        }
        break;

    case ERROR:
        qCDebug(PRINT_THREAD) << "Error State";
        break;

    case STOP: {
        QString stopString(GCode::toCommand(GCode::M112));
        d->gcodestream->setString(&stopString);
        setState(IDLE);
        break;
    }

    case PAUSE:
        break;

    default:
        qCDebug(PRINT_THREAD) << "Unknown State";
        break;
    }

    if (d->gcodestream->atEnd()) {
        emit(printProgressChanged(100));
        qCDebug(PRINT_THREAD) << "atEnd";
        disconnect(d->core->plugin(), &IFirmware::readyForCommand, this, &PrintThread::commandReady);
        disconnect(this, &PrintThread::nextCommand, d->core, &AtCore::pushCommand);
        disconnect(d->core, &AtCore::stateChanged, this, &PrintThread::setState);
        emit(stateChanged(FINISHEDPRINT));
        emit(stateChanged(IDLE));
        disconnect(this, &PrintThread::stateChanged, d->core, &AtCore::setState);
        emit finished();
    }
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

void PrintThread::setState(const PrinterState &newState)
{
    if (newState != d->state) {
        qCDebug(PRINT_THREAD) << "State Changed from [" << d->state << "] to [" << newState << ']';
        disconnect(d->core, &AtCore::stateChanged, this, &PrintThread::setState);
        d->state = newState;
        emit(stateChanged(d->state));
        connect(d->core, &AtCore::stateChanged, this, &PrintThread::setState, Qt::QueuedConnection);
    }
}
