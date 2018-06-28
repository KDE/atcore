/* AtCore
    Copyright (C) <2016>

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

#include <QLoggingCategory>

#include "seriallayer.h"

Q_LOGGING_CATEGORY(SERIAL_LAYER, "org.kde.atelier.core.serialLayer")

namespace
{
QByteArray _return = QByteArray("\r");
QByteArray _newLine = QByteArray("\n");
QByteArray _newLineReturn = QByteArray("\n\r");
QStringList _validBaudRates = {
    QStringLiteral("9600"),
    QStringLiteral("14400"),
    QStringLiteral("19200"),
    QStringLiteral("28800"),
    QStringLiteral("38400"),
    QStringLiteral("57600"),
    QStringLiteral("76800"),
    QStringLiteral("115200"),
    QStringLiteral("230400"),
    QStringLiteral("250000"),
    QStringLiteral("500000"),
    QStringLiteral("1000000")
};
}

/**
 * @brief The SerialLayerPrivate class
 */
class SerialLayerPrivate
{
public:
    bool _serialOpened;                 //!< @param _serialOpened: is serial port opened
    QByteArray _rawData;                //!< @param _rawData: the raw serial data
    QVector<QByteArray> _rByteCommands; //!< @param _rByteCommand: received Messages
    QVector<QByteArray> _sByteCommands; //!< @param _sByteCommand: sent Messages
};

SerialLayer::SerialLayer(const QString &port, uint baud, QObject *parent) :
    QSerialPort(parent), d(new SerialLayerPrivate())
{
    setPortName(port);
    setBaudRate(baud);
    if (open(QIODevice::ReadWrite)) {
        d->_serialOpened = true;
        connect(this, &QSerialPort::readyRead, this, &SerialLayer::readAllData);
    }
};

void SerialLayer::readAllData()
{
    d->_rawData.append(readAll());

    //Remove any \r in the string, then split by \n.
    //This removes any trailing \r or \n from the commands
    // Proper line endings are added when the command is pushed.
    d->_rawData = d->_rawData.replace(_return, QByteArray());

    QList<QByteArray> tempList = d->_rawData.split(_newLine.at(0));
    for (auto i = tempList.begin(); i != tempList.end(); ++i) {
        // Get finished line to _byteCommands
        if (i < tempList.end() - 1) {
            d->_rByteCommands.append(*i);
            emit receivedCommand(*i);
        } else {
            d->_rawData.clear();
            d->_rawData.append(*i);
        }
    }
}

void SerialLayer::pushCommand(const QByteArray &comm, const QByteArray &term)
{
    if (!isOpen()) {
        qCDebug(SERIAL_LAYER) << "Serial not connected !";
        return;
    }
    QByteArray tmp = comm + term;
    write(tmp);
    emit pushedCommand(tmp);

}

void SerialLayer::pushCommand(const QByteArray &comm)
{
    pushCommand(comm, _newLineReturn);
}

void SerialLayer::add(const QByteArray &comm, const QByteArray &term)
{
    QByteArray tmp = comm + term;
    d->_sByteCommands.append(tmp);
}

void SerialLayer::add(const QByteArray &comm)
{
    add(comm, _newLineReturn);
}

void SerialLayer::push()
{
    if (!isOpen()) {
        qCDebug(SERIAL_LAYER) << "Serial not connected !";
        return;
    }
    for (const auto &comm : qAsConst(d->_sByteCommands)) {
        write(comm);
        emit pushedCommand(comm);
    }
    d->_sByteCommands.clear();
}

bool SerialLayer::commandAvailable() const
{
    return !d->_rByteCommands.isEmpty();
}

QStringList SerialLayer::validBaudRates() const
{
    return _validBaudRates;
}
