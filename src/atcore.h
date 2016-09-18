#ifndef ATCORE_H
#define ATCORE_H

#include <QObject>
#include <QList>
#include <QSerialPortInfo>
#include <QLoggingCategory>

class ProtocolLayer;
struct AtCorePrivate;

Q_DECLARE_LOGGING_CATEGORY(Received)
Q_DECLARE_LOGGING_CATEGORY(Sended)
Q_DECLARE_LOGGING_CATEGORY(FirmwarePlugin)

class AtCore : public QObject {
Q_OBJECT
public:
    AtCore(QObject *parent = 0);
    QList<QSerialPortInfo> serialPorts() const;

    bool initFirmware(const QString& port, int baud);
    bool isInitialized();

    ProtocolLayer *protocol() const;
private:
    void findFirmware(const QByteArray& message);
    AtCorePrivate *d;
};

#endif
