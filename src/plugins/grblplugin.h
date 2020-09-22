/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2016 Tomaz Canabrava <tcanabrava@kde.org>
    SPDX-FileCopyrightText: 2016-2018 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2016-2018 Patrick José Pereira <patrickjp@kde.org>
*/

#pragma once

#include <QObject>

#include "ifirmware.h"
/**
 * @brief The GrblPlugin class
 * Plugin for Grbl
 */
class GrblPlugin : public IFirmware
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.atelier.core.firmware")
    Q_INTERFACES(IFirmware)

public:
    /**
     * @brief Create new GrblPlugin
     */
    GrblPlugin();
    ~GrblPlugin() = default;

    /**
     * @brief Return Plugin name
     * @return Grbl
     */
    QString name() const override;

    /**
     * @brief sdSupport
     * @return True if plugins supports sd cards
     */
    bool isSdSupported() const override;

    /**
     * @brief Translate common commands to firmware specific command.
     * @param command: command to translate
     * @return firmware specific translated command
     */
    QByteArray translate(const QString &command) override;

    /**
     * @brief Grbl does not return anything on command execution
     * @param lastMessage: last message from printer
     */
    void validateCommand(const QString &lastMessage) override;
};
