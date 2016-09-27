#include "repetierplugin.h"
#include <QLoggingCategory>
#include <QString>

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
    if (lastMessage.contains("ok") || lastMessage.contains("wait")) {
        return true;
    }
    return false;
}
