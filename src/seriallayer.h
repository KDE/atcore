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
    static QByteArray _newLineReturn;

    bool _serialOpened;
    QByteArray _rawData;
    QVector<QByteArray> _rByteCommands;
    QVector<QByteArray> _sByteCommands;
signals:
    void pushedCommand(QString);
public:
    SerialLayer(QString port, uint baud, QWidget *parent = 0);
    ~SerialLayer();

    void add(QByteArray comm, QByteArray term);
    void add(QByteArray comm);
    void pushCommand(QByteArray comm, QByteArray term);
    void pushCommand(QByteArray comm);
    void push();
    bool commandAvailable();
    QByteArray popCommand();
    bool opened();
    void closeConnection();
};
