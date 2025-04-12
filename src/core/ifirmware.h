/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2016, 2018 Tomaz Canabrava <tcanabrava@kde.org>
    SPDX-FileCopyrightText: 2016-2019 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2016-2019 Patrick José Pereira <patrickjp@kde.org>
    SPDX-FileCopyrightText: 2016, 2019 Lays Rodrigues <lays.rodrigues@kde.org>
    SPDX-FileCopyrightText: 2018 Leandro Santiago <leandrosansilva@gmail.com>
*/

#pragma once

#include <QObject>
#include <QString>

#include "atcore_export.h"

class Temperature;
class AtCore;

/**
 * @brief The IFirmware class
 * Base Class for Firmware Plugins
 */
class ATCORE_EXPORT IFirmware : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(bool sdSupport READ isSdSupported CONSTANT)
public:
    IFirmware();
    void init(AtCore *parent);
    ~IFirmware() override;

    /**
     * @brief Check for plugin support of sd cards.
     * @return True if firmware plugin supports sd cards.
     */
    virtual bool isSdSupported() const = 0;

    /**
     * @brief Virtual name to be reimplemented by Firmware plugin
     *
     * Return the name the firmware the plugin is for
     * @return Firmware Name
     */
    virtual QString name() const = 0;

    /**
     * @brief Virtual validateCommand to filter commands from messages
     * @param lastMessage: last Message from printer
     */
    virtual void validateCommand(const QString &lastMessage);

    /**
     * @brief Virtual translate to be reimplemented by Firmwareplugin
     *
     * Translate common commands to firmware specific command.
     * @param command: Command command to translate
     * @return firmware specific translated command
     */
    virtual QByteArray translate(const QString &command);

    /**
     * @brief AtCore Parent of the firmware plugin
     * @return
     */
    AtCore *core() const;

private:
    struct IFirmwarePrivate;
    IFirmwarePrivate *d;
public Q_SLOTS:
    /**
     * @brief call Validate Command
     * @param lastMessage: last message from printer
     */
    void checkCommand(const QByteArray &lastMessage);
Q_SIGNALS:
    /**
     * @brief emit when firmware is ready for a command
     */
    void readyForCommand(void);
};

Q_DECLARE_INTERFACE(IFirmware, "org.kde.atelier.core.firmware")
