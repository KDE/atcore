#include "grblplugin.h"
#include <QString>

QString GrblPlugin::name() const
{
    return QStringLiteral("Grbl");
}

GrblPlugin::GrblPlugin(QObject *parent) : QObject(parent)
{

}

QByteArray GrblPlugin::translate(const QString &command)
{
    return command.toLocal8Bit();
}

bool GrblPlugin::readyForNextCommand(const QString &lastMessage)
{
    Q_UNUSED(lastMessage);
    return true;
}
