#include "repetierplugin.h"
#include <QString>

QString RepetierPlugin::name() const
{
    return QStringLiteral("Repetier");
}

RepetierPlugin::RepetierPlugin(QObject* parent)
{
}

bool RepetierPlugin::readyForNextCommand(const QString& lastMessage)
{
    if (lastMessage.contains("ok") || lastMessage.contains("wait")){
        return true;
    }
    return false;
}
