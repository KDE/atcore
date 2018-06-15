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
#pragma once

#include <QObject>

#include "ifirmware.h"
/**
 * @brief The TeacupPlugin class
 * Plugin for Teacup
 */
class TeacupPlugin : public IFirmware
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.atelier.core.firmware")
    Q_INTERFACES(IFirmware)

public:
    /**
     * @brief Create new TeacupPlugin
     */
    TeacupPlugin();

    /**
     * @brief Check for plugin support of sd cards.
     * @return True if firmware plugin supports sd cards.
     */
    virtual bool isSdSupported() const override;

    /**
     * @brief Return Plugin name
     * @return Teacup
     */
    QString name() const override;

    /**
     * @brief Translate common commands to firmware specific command.
     * @param command: command to translate
     * @return firmware specific translated command
     */
    QByteArray translate(const QString &command) override;
};
