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
    Q_UNUSED(lastMessage);
    return true;
}
