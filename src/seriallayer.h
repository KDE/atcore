/* AtCore
    Copyright (C) <2016>

    Authors:
        Patrick José Pereira <patrickelectric@gmail.com>
        Chris Rizzitello <sithlord48@gmail.com>
        Tomaz Canabrava <tcanabrava@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <QSerialPort>
#include <QVector>

#include "katcore_export.h"

class KATCORE_EXPORT SerialLayer : public QObject
{
    Q_OBJECT

private:
    QSerialPort *serial;

    /**
     * @brief Read serial data
     *
     */
    void readData();

    static QByteArray _return;
    static QByteArray _newLine;
    static QByteArray _newLineReturn;
    static QStringList _validBaudRates;

    bool _serialOpened;
    QByteArray _rawData;
    QVector<QByteArray> _rByteCommands;
    QVector<QByteArray> _sByteCommands;
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
public:

    /**
     * @brief SerialLayer Class to realize communication
     *
     * @param port : Port (/dev/ttyUSB ACM)
     * @param baud : Baud rate (115200)
     * @param parent : Parent
     */
    SerialLayer(const QString &port, uint baud, QObject *parent = 0);

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
     * @param comm : Commmand, default terminator will be used
     */
    void add(const QByteArray &comm);

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
    bool commandAvailable();

    /**
     * @brief Return FIFO command from printer
     *
     * @return QByteArray
     */
    QByteArray popCommand();

    /**
     * @brief Check if port is opened
     *
     * @return bool
     */
    bool opened();

    /**
     * @brief Close serial connection
     *
     */
    void closeConnection();

    /**
     * @brief Return a QStringList of valids serial baud rates
     *
     * @return QStringList
     */
    QStringList validBaudRates();
};
