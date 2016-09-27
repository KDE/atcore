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

RepetierPlugin::RepetierPlugin(QObject *parent)
{
    qCDebug(REPETIER_PLUGIN) << name() << " plugin loaded!";
}

bool RepetierPlugin::readyForNextCommand(const QString &lastMessage)
{
    if (lastMessage.contains(_ok) || lastMessage.contains(_wait)) {
        return true;
    }
    return false;
}
