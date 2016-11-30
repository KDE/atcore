#include "sprinterplugin.h"
#include "temperature.h"
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

SprinterPlugin::SprinterPlugin()
{
    qCDebug(SPRINTER_PLUGIN) << name() << " plugin loaded!";
}

void SprinterPlugin::extractTemp(const QString &lastMessage)
{
    // ok T:185.4 @:0 B:60.5
    QStringList list = lastMessage.split(QChar::fromLatin1(' '));

    // T:185.4 - current temperature
    temperature()->setExturderTemperature(list[0].mid(2).toFloat());
    // /185.0 - target temperature
    temperature()->setExturderTargetTemperature(list[1].mid(1).toFloat());
    // B:185.4 - current temperature
    temperature()->setBedTemperature(list[2].mid(2).toFloat());
    // /60.0 - target temperature
    temperature()->setBedTargetTemperature(list[3].mid(1).toFloat());
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
