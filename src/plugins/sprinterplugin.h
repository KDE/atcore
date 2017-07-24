/* AtCore KDE Libary for 3D Printers
    Copyright (C) <2016>

    Authors:
        Tomaz Canabrava <tcanabrava@kde.org>
        Chris Rizzitello <rizzitello@kde.org>
        Patrick José Pereira <patrickelectric@gmail.com>

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

#include "ifirmware.h"
#include <QObject>
/**
 * @brief The SprinterPlugin class
 * Plugin for Sprinter
 */
class SprinterPlugin : public IFirmware
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.atelier.core.firmware" FILE "sprinter.json")
    Q_INTERFACES(IFirmware)

private:
    /**
     * @brief command finished string
     */
    static QString _ok;

public:
    /**
     * @brief Create new SprinterPlugin
     */
    SprinterPlugin();

    /**
     * @brief Return Plugin name
     * @return Sprinter
     */
    QString name() const override;

    /**
     * @brief Check if command contains SprinterPlugin::_ok
     * @param lastMessage: last message from printer
     */
    void validateCommand(const QString &lastMessage) override;

    /**
     * @brief Translate common commands to firmware specific command.
     * @param command: command to translate
     * @return firmware specific translated command
     */
    QByteArray translate(const QString &command) override;
};
