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
#include <QDebug>

#include "seriallayer.h"

QByteArray SerialLayer::_return = QByteArray("\r");
QByteArray SerialLayer::_newLine = QByteArray("\n");
QByteArray SerialLayer::_newLineReturn = QByteArray("\n\r");
QStringList SerialLayer::_validBaudRates = {
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

SerialLayer::SerialLayer(const QString &port, uint baud, QObject *parent) :
    QObject(parent),
    serial(new QSerialPort(this)),
    _serialOpened(false)
{
    serial->setPortName(port);
    serial->setBaudRate(baud);
    _serialOpened = serial->open(QIODevice::ReadWrite);

    connect(serial, &QSerialPort::readyRead, this, &SerialLayer::readData);
};

bool SerialLayer::opened()
{
    return _serialOpened;
}

void SerialLayer::readData()
{
    _rawData.append(serial->readAll());

    /*
     * Check if \r exist and remove
     * Both \n\r and \n are used in string's end and some protocols
     */
    if (_rawData.contains(_return)) {
        _rawData = _rawData.replace(_return, QByteArray());
    }

    QByteArray tempArray;
    QList<QByteArray> tempList = _rawData.split(_newLine.at(0));
    for (auto i = tempList.begin(); i != tempList.end(); ++i) {
        // Get finished line to _byteCommands
        if (i < tempList.end() - 1) {
            _rByteCommands.append(*i);
            emit(receivedCommand(*i));
        } else {
            _rawData.clear();
            _rawData.append(*i);
        }
    }
}

void SerialLayer::pushCommand(const QByteArray &comm, const QByteArray &term)
{
    QByteArray tmp = comm + term;
    serial->write(tmp);
    emit(pushedCommand(tmp));

}

void SerialLayer::pushCommand(const QByteArray &comm)
{
    pushCommand(comm, _newLineReturn);
}

void SerialLayer::add(const QByteArray &comm, const QByteArray &term)
{
    QByteArray tmp = comm + term;
    _sByteCommands.append(tmp);
}

void SerialLayer::add(const QByteArray &comm)
{
    add(comm, _newLineReturn);
}

void SerialLayer::push()
{
    foreach (const auto &comm, _sByteCommands) {
        serial->write(comm);
        emit(pushedCommand(comm));
    }
    _sByteCommands.clear();
}

QByteArray SerialLayer::popCommand()
{
    return commandAvailable() ? _rByteCommands.takeFirst() : QByteArray();
}

bool SerialLayer::commandAvailable()
{
    return !_rByteCommands.isEmpty();
}

void SerialLayer::closeConnection()
{
    if (_serialOpened) {
        serial->close();
    }
}

QStringList SerialLayer::validBaudRates()
{
    return _validBaudRates;
}

