/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2016-2018 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2016 Tomaz Canabrava <tcanabrava@kde.org>
    SPDX-FileCopyrightText: 2016-2018 Patrick José Pereira <patrickjp@kde.org>
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
    ~TeacupPlugin() = default;

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
