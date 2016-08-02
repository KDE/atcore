#include <QDebug>

#include "seriallayer.h"

QByteArray SerialLayer::_return = QByteArray("\r");
QByteArray SerialLayer::_newLine = QByteArray("\n");

SerialLayer::SerialLayer(QString port, uint baud, QWidget *parent) :
    serial(new QSerialPort()),
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
    if (_rawData.contains(_return))
    {
        _rawData = _rawData.replace(_return, QByteArray());
    }

    QByteArray tempArray;
    QList<QByteArray> tempList = _rawData.split(_newLine.at(0));
    for (auto i = tempList.begin(); i != tempList.end(); ++i)
    {
        // Get finished line to _byteCommands
        if (i < tempList.end()-1)
        {
            _byteCommands.append(*i);
        }
        else
        {
            _rawData.clear();
            _rawData.append(*i);
        }
    }
}

QByteArray SerialLayer::popCommand()
{
    return _byteCommands.takeFirst();
}

bool SerialLayer::commandAvailable()
{
    return !_byteCommands.isEmpty();
}

SerialLayer::~SerialLayer()
{
    delete serial;
}
