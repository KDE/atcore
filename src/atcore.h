#ifndef ATCORE_H
#define ATCORE_H

#include <QObject>
#include <QList>
#include <QSerialPortInfo>

class SerialLayer;

enum PrinterState {
    DISCONNECTED, //Not Connected to a printer, initial state
    CONNECTING, //Attempting to connect, Fw not probed
    IDLE, //Connected to printer and ready for commands
    BUSY, //Printer is working
    PAUSE, //Printer is paused
    ERROR, // Printer Returned Error
    STOP // Emergency Stop
};

struct AtCorePrivate;

class AtCore : public QObject
{
    Q_OBJECT
public:
    AtCore(QObject *parent = 0);
    QList<QSerialPortInfo> serialPorts() const;

    void initFirmware(const QString &port, int baud);
    bool isInitialized();

    SerialLayer *serial() const;

    /**
     * @brief Print a file
     *
     * @param fileName : path of file to print
     */
    void print(const QString &fileName);

    /**
     * @brief Get Printer state
     * @return State of the printer
     */
    PrinterState state(void);

    /**
     * @brief setState: set Printer state
     * @param state : printer state.
     */
    void setState(PrinterState state);
signals:
    /**
     * @brief Emit signal when message is received from the printer
     *
     * @param msg : Message
     */
    void receivedMessage(const QByteArray &message);
private:
    void findFirmware(const QByteArray &message);
    void newMessage(const QByteArray &message);
    QByteArray lastMessage;
    AtCorePrivate *d;
    PrinterState printerState;
};

#endif
