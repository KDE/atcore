/* AtCore
    Copyright (C) <2016 - 2018>

    Authors:
        Patrick José Pereira <patrickjp@kde.org>
        Chris Rizzitello <rizzitello@kde.org>
        Tomaz Canabrava <tcanabrava@kde.org>

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

#include <QSerialPort>
#include <QVector>

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
