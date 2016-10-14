#include "aprinterplugin.h"
#include <QLoggingCategory>
#include <QString>

QString AprinterPlugin::_ok = QStringLiteral("ok");
QString AprinterPlugin::_wait = QStringLiteral("wait");
QString AprinterPlugin::_extruderTemp = QStringLiteral("T:");
QString AprinterPlugin::_bedTemp = QStringLiteral("B:");

Q_LOGGING_CATEGORY(APRINTER_PLUGIN, "org.kde.atelier.core.firmware.aprinter");

QString AprinterPlugin::name() const
{
    return QStringLiteral("Aprinter");
}

AprinterPlugin::AprinterPlugin(QObject *parent) : QObject(parent)
{
    qCDebug(APRINTER_PLUGIN) << name() << " plugin loaded!";
}

void AprinterPlugin::extractTemp(const QString &lastMessage)
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

    qCDebug(APRINTER_PLUGIN) << "Extruder" << PrinterStatus.extruderTemp << "/" << PrinterStatus.extruderTargetTemp;
    qCDebug(APRINTER_PLUGIN) << "Bed" << PrinterStatus.bedTemp << "/" << PrinterStatus.bedTargetTemp;
}

bool AprinterPlugin::validateCommand(const QString &lastMessage)
{
    if (lastMessage.contains(_extruderTemp) || lastMessage.contains(_bedTemp)) {
        extractTemp(lastMessage);
    } else if (lastMessage.contains(_ok) || lastMessage.contains(_wait)) {
        return true;
    }
    return false;
}

QByteArray AprinterPlugin::translate(const QString &command)
{
    return command.toLocal8Bit();
}

bool AprinterPlugin::readyForNextCommand(const QString &lastMessage)
{
    return validateCommand(lastMessage);
}
