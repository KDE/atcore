/* AtCore Imports
    Copyright (C) <2019>

    Authors:
        Lays Rodrigues <lays.rodrigues@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "atcoreextensionplugin.h"
#include <atcore.h>
#include <temperature.h>
#include <machineinfo.h>
#include <QQmlApplicationEngine>

void AtCoreExtensionPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QLatin1String(uri) == QLatin1String("org.kde.atcore"));
    qmlRegisterType<AtCore>("org.kde.atcore", 1, 0, "AtCore");
    qmlRegisterType<Temperature>("org.kde.atcore", 1, 0, "Temperature");
    qmlRegisterSingletonType<MachineInfo>("org.kde.atcore", 1, 0, "MachineInfo", [](QQmlEngine *engine, QJSEngine *jsEngine) -> QObject * {
        return MachineInfo::instance()->qmlSingletonRegister(engine, jsEngine);
    });
}
