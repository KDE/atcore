#include "repetierplugin.h"
#include <QLoggingCategory>
#include <QString>

QString RepetierPlugin::_ok = QStringLiteral("ok");
QString RepetierPlugin::_wait = QStringLiteral("wait");
QString RepetierPlugin::_extruderTemp = QStringLiteral("T:");
QString RepetierPlugin::_bedTemp = QStringLiteral("B:");

Q_LOGGING_CATEGORY(REPETIER_PLUGIN, "org.kde.atelier.core.firmware.repetier");

QString RepetierPlugin::name() const
{
    return QStringLiteral("Repetier");
}

RepetierPlugin::RepetierPlugin(QObject *parent) : QObject(parent)
{
    qCDebug(REPETIER_PLUGIN) << name() << " plugin loaded!";
}

void RepetierPlugin::extractTemp(const QString &lastMessage)
{
    // ok T:185.4 /185.0 B:60.5 /60.0
    QStringList list = lastMessage.split(QChar::fromLatin1(' '));
    // T:185.4 - current temperature
    PrinterStatus.extruderTemp = list[0].mid(2).toFloat();
    // /185.0 - target temperature
    PrinterStatus.extruderTargetTemp = list[1].mid(1).toFloat();
    // B:185.4 - current temperature
    PrinterStatus.bedTemp = list[2].mid(2).toFloat();
    // /60.0 - target temperature
    PrinterStatus.bedTargetTemp = list[3].mid(1).toFloat();

    qCDebug(REPETIER_PLUGIN) << "Extruder" << PrinterStatus.extruderTemp << "/" << PrinterStatus.extruderTargetTemp;
    qCDebug(REPETIER_PLUGIN) << "Bed" << PrinterStatus.bedTemp << "/" << PrinterStatus.bedTargetTemp;
}

bool RepetierPlugin::validateCommand(const QString &lastMessage)
{
    if (lastMessage.contains(_extruderTemp) || lastMessage.contains(_bedTemp)) {
        extractTemp(lastMessage);
    } else if (lastMessage.contains(_ok) || lastMessage.contains(_wait)) {
        return true;
    }
    return false;
}

QByteArray RepetierPlugin::translate(const QString &command)
{
    return command.toLocal8Bit();
}

bool RepetierPlugin::readyForNextCommand(const QString &lastMessage)
{
    return validateCommand(lastMessage);
}
