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
 * @brief The AprinterPlugin class
 * Plugin for Aprinter
 */
class AprinterPlugin : public IFirmware
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.atelier.core.firmware")
    Q_INTERFACES(IFirmware)

public:
    /**
     * @brief Create new AprinterPlugin
     */
    AprinterPlugin();
    ~AprinterPlugin() = default;
    /**
     * @brief Check for plugin support of sd cards.
     * @return True if firmware plugin supports sd cards.
     */
    virtual bool isSdSupported() const override;

    /**
     * @brief Return Plugin name
     * @return Aprinter
     */
    QString name() const override;
};
