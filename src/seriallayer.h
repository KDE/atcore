#pragma once

#include <QSerialPort>

class  SerialLayer : public QObject
{
Q_OBJECT

private:
    QSerialPort *serial;

    void readData();

    static QByteArray _return;
    static QByteArray _newLine;

    bool _serialOpened;
    QByteArray _rawData;
    QList<QByteArray> _byteCommands;
public:
    SerialLayer(QString port, uint baud, QWidget *parent = 0);
    ~SerialLayer();

    bool commandAvailable();
    QByteArray popCommand();
    bool opened();
};
