#include "teacupplugin.h"
#include <QLoggingCategory>
#include <QString>

QString TeacupPlugin::_ok = QStringLiteral("ok");
QString TeacupPlugin::_wait = QStringLiteral("wait");
QString TeacupPlugin::_extruderTemp = QStringLiteral("T:");
QString TeacupPlugin::_bedTemp = QStringLiteral("B:");

Q_LOGGING_CATEGORY(TEACUP_PLUGIN, "org.kde.atelier.core.firmware.teacup");

QString TeacupPlugin::name() const
{
    return QStringLiteral("Teacup");
}

TeacupPlugin::TeacupPlugin(QObject *parent) : QObject(parent)
{
    qCDebug(TEACUP_PLUGIN) << name() << " plugin loaded!";
}

void TeacupPlugin::extractTemp(const QString &lastMessage)
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

    qCDebug(TEACUP_PLUGIN) << "Extruder" << PrinterStatus.extruderTemp << "/" << PrinterStatus.extruderTargetTemp;
    qCDebug(TEACUP_PLUGIN) << "Bed" << PrinterStatus.bedTemp << "/" << PrinterStatus.bedTargetTemp;
}

bool TeacupPlugin::validateCommand(const QString &lastMessage)
{
    if (lastMessage.contains(_extruderTemp) || lastMessage.contains(_bedTemp)) {
        extractTemp(lastMessage);
    } else if (lastMessage.contains(_ok) || lastMessage.contains(_wait)) {
        return true;
    }
    return false;
}

QByteArray TeacupPlugin::translate(const QString &command)
{
    return command.toLocal8Bit();
}

bool TeacupPlugin::readyForNextCommand(const QString &lastMessage)
{
    return validateCommand(lastMessage);
}
