#include "repetierplugin.h"
#include <QString>

QString RepetierPlugin::name() const
{
    return QStringLiteral("Repetier");
}

RepetierPlugin::RepetierPlugin(QObject* parent)
{
}
