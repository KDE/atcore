/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2019 Lays Rodrigues <lays.rodrigues@kde.org>
*/

#include "atcoreextensionplugin.h"
#include <QQmlApplicationEngine>
#include <atcore.h>
#include <machineinfo.h>
#include <temperature.h>

void AtCoreExtensionPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QLatin1String(uri) == QLatin1String("org.kde.atcore"));
    qmlRegisterType<AtCore>("org.kde.atcore", 1, 0, "AtCore");
    qmlRegisterType<Temperature>("org.kde.atcore", 1, 0, "Temperature");
    qmlRegisterSingletonType<MachineInfo>("org.kde.atcore", 1, 0, "MachineInfo", [](QQmlEngine *engine, QJSEngine *jsEngine) -> QObject * { return MachineInfo::instance()->qmlSingletonRegister(engine, jsEngine); });
}
