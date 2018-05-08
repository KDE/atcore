/* AtCore KDE Libary for 3D Printers
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Chris Rizzitello <rizzitello@kde.org>
        Patrick José Pereira <patrickjp@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QString>
#include <QLoggingCategory>

#include "sprinterplugin.h"
#include "atcore.h"

Q_LOGGING_CATEGORY(SPRINTER_PLUGIN, "org.kde.atelier.core.firmware.sprinter")

QString SprinterPlugin::name() const
{
    return QStringLiteral("Sprinter");
}

SprinterPlugin::SprinterPlugin()
{
    qCDebug(SPRINTER_PLUGIN) << name() << " plugin loaded!";
}
