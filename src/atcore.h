#ifndef ATCORE_H
#define ATCORE_H

#include <QObject>
#include <QList>
#include <QSerialPortInfo>

class ProtocolLayer;
struct AtCorePrivate;

class AtCore : public QObject {
Q_OBJECT
public:
    AtCore(QObject *parent = 0);
    QList<QSerialPortInfo> serialPorts() const;

    bool initFirmware(const QString& port, int baud);
    bool isInitialized();

    ProtocolLayer *protocol() const;

    /**
     * @brief Print a file
     *
     * @param fileName : path of file to print
     */
    void print(const QString& fileName);
signals:
    /**
     * @brief Emit signal when message is received from the printer
     *
     * @param msg : Message
     */
    void receivedMessage(const QByteArray& msg);
private:
    void findFirmware(const QByteArray& message);
    void newMessage(const QByteArray& msg);
    QByteArray lastMessage;
    AtCorePrivate *d;
};

#endif
