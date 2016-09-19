#include "grblplugin.h"
#include <QString>

QString GrblPlugin::name() const
{
    return QStringLiteral("Grbl");
}

GrblPlugin::GrblPlugin(QObject* parent)
{
}

bool GrblPlugin::readyForNextCommand(const QString& lastMessage)
{
    return true;
}
