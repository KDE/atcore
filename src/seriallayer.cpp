#include <QDebug>

#include "seriallayer.h"

QByteArray SerialLayer::_return = QByteArray("\r");
QByteArray SerialLayer::_newLine = QByteArray("\n");
QByteArray SerialLayer::_newLineReturn = QByteArray("\n\r");

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
