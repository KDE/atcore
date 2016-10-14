#include "sprinterplugin.h"
#include <QLoggingCategory>
#include <QString>

QString SprinterPlugin::_ok = QStringLiteral("ok");
QString SprinterPlugin::_wait = QStringLiteral("wait");
QString SprinterPlugin::_extruderTemp = QStringLiteral("T:");
QString SprinterPlugin::_bedTemp = QStringLiteral("B:");

Q_LOGGING_CATEGORY(SPRINTER_PLUGIN, "org.kde.atelier.core.firmware.sprinter");

QString SprinterPlugin::name() const
{
    return QStringLiteral("Sprinter");
}

SprinterPlugin::SprinterPlugin(QObject *parent) : QObject(parent)
{
    qCDebug(SPRINTER_PLUGIN) << name() << " plugin loaded!";
}

void SprinterPlugin::extractTemp(const QString &lastMessage)
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

    qCDebug(SPRINTER_PLUGIN) << "Extruder" << PrinterStatus.extruderTemp << "/" << PrinterStatus.extruderTargetTemp;
    qCDebug(SPRINTER_PLUGIN) << "Bed" << PrinterStatus.bedTemp << "/" << PrinterStatus.bedTargetTemp;
}

bool SprinterPlugin::validateCommand(const QString &lastMessage)
{
    if (lastMessage.contains(_extruderTemp) || lastMessage.contains(_bedTemp)) {
        extractTemp(lastMessage);
    } else if (lastMessage.contains(_ok) || lastMessage.contains(_wait)) {
        return true;
    }
    return false;
}

QByteArray SprinterPlugin::translate(const QString &command)
{
    return command.toLocal8Bit();
}

bool SprinterPlugin::readyForNextCommand(const QString &lastMessage)
{
    return validateCommand(lastMessage);
}
