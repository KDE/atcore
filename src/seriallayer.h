#pragma once

#include <QSerialPort>

class  SerialLayer : public QObject
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
    void pushedCommand(QByteArray comm);

    /**
     * @brief Emit signal when command is received
     *
     * @param comm : Command
     */
    void receivedCommand(QByteArray comm);
public:

    /**
     * @brief SerialLayer Class to realize communication
     *
     * @param port : Port (/dev/ttyUSB ACM)
     * @param baud : Baud rate (115200)
     * @param parent : Parent
     */
    SerialLayer(QString port, uint baud, QWidget *parent = 0);
    ~SerialLayer();

    /**
     * @brief Add command to be pushed
     *
     * @param comm : Command
     * @param term : Terminator
     */
    void add(QByteArray comm, QByteArray term);

    /**
     * @brief Add command to be pushed
     *
     * @param comm : Commmand, default terminator will be used
     */
    void add(QByteArray comm);

    /**
     * @brief Push command directly
     *
     * @param comm : Command
     * @param term : Terminator
     */
    void pushCommand(QByteArray comm, QByteArray term);

    /**
     * @brief Push command directly
     *
     * @param comm : Command, default terminator will be used
     */
    void pushCommand(QByteArray comm);

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
};
