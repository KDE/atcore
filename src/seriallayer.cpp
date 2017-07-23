/* AtCore
    Copyright (C) <2016>

    Authors:
        Patrick José Pereira <patrickelectric@gmail.com>
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
#include <QDebug>

#include "seriallayer.h"

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
};

class SerialLayerPrivate
{
public:
    bool _serialOpened;
    QByteArray _rawData;
    QVector<QByteArray> _rByteCommands;
    QVector<QByteArray> _sByteCommands;
};

SerialLayer::SerialLayer(const QString &port, uint baud, QObject *parent) :
    QSerialPort(parent), d(new SerialLayerPrivate())
{
    setPortName(port);
    setBaudRate(baud);
    open(QIODevice::ReadWrite);
    connect(this, &QSerialPort::readyRead, this, &SerialLayer::readAllData);
};

void SerialLayer::readAllData()
{
    d->_rawData.append(readAll());

    /*
     * Check if \r exist and remove
     * Both \n\r and \n are used in string's end and some protocols
     */
    if (d->_rawData.contains(_return)) {
        d->_rawData = d->_rawData.replace(_return, QByteArray());
    }

    QByteArray tempArray;
    QList<QByteArray> tempList = d->_rawData.split(_newLine.at(0));
    for (auto i = tempList.begin(); i != tempList.end(); ++i) {
        // Get finished line to _byteCommands
        if (i < tempList.end() - 1) {
            d->_rByteCommands.append(*i);
            emit(receivedCommand(*i));
        } else {
            d->_rawData.clear();
            d->_rawData.append(*i);
        }
    }
}

void SerialLayer::pushCommand(const QByteArray &comm, const QByteArray &term)
{
    QByteArray tmp = comm + term;
    write(tmp);
    emit(pushedCommand(tmp));

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
    foreach (const auto &comm, d->_sByteCommands) {
        write(comm);
        emit(pushedCommand(comm));
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

