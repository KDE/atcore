/* AtCore KDE Libary for 3D Printers
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
    SPDX-FileCopyrightText: 2016 Patrick José Pereira <patrickjp@kde.org>
    SPDX-FileCopyrightText: 2016-2018, 2020 Chris Rizzitello <rizzitello@kde.org>
    SPDX-FileCopyrightText: 2016, 2018 Tomaz Canabrava <tcanabrava@kde.org>
    SPDX-FileCopyrightText: 2018 Leandro Santiago <leandrosansilva@gmail.com>
*/

#pragma once

#include <QSerialPort>

#include "atcore_export.h"

/**
 * @brief The SerialLayer class.
 * Provide the low level serial operations
 */
class ATCORE_EXPORT SerialLayer : public QSerialPort
{
    Q_OBJECT

private:
    struct SerialLayerPrivate;
    SerialLayerPrivate *d;

    /**
     * @brief Read all available serial data
     *
     */
    void readAllData();
signals:

    /**
     * @brief Emit signal when command is pushed
     *
     * @param comm : Command
     */
    void pushedCommand(const QByteArray &comm);

    /**
     * @brief Emit signal when command is received
     *
     * @param comm : Command
     */
    void receivedCommand(const QByteArray &comm);

    /**
     * @brief Emit a signal if an error has happened.
     * @param error: the Error
     */
    void serialError(QSerialPort::SerialPortError error);

public:
    /**
     * @brief SerialLayer Class to realize communication
     *
     * @param port : Port (/dev/ttyUSB ACM)
     * @param baud : Baud rate (115200)
     * @param parent : Parent
     */
    SerialLayer(const QString &port, int32_t baud, QObject *parent = nullptr);

    /**
     * @brief Add command to be pushed
     *
     * @param comm : Command
     * @param term : Terminator
     */
    void add(const QByteArray &comm, const QByteArray &term);

    /**
     * @brief Add command to be pushed
     *
     * @param comm : Command, default terminator will be used
     */
    void add(const QByteArray &comm);

    /**
     * @brief handleError Handle Errors from the serial port
     * @param error: The reported error
     */
    void handleError(QSerialPort::SerialPortError error);

    /**
     * @brief Push command directly
     *
     * @param comm : Command
     * @param term : Terminator
     */
    void pushCommand(const QByteArray &comm, const QByteArray &term);

    /**
     * @brief Push command directly
     *
     * @param comm : Command, default terminator will be used
     */
    void pushCommand(const QByteArray &comm);

    /**
     * @brief Push all commands used in add to serial write
     *
     */
    void push();

    /**
     * @brief Check if is a command available
     *
     * @return bool
     */
    bool commandAvailable() const;

    /**
     * @brief Return a QStringList of valids serial baud rates
     *
     * @return QStringList
     */
    QStringList validBaudRates() const;
};
