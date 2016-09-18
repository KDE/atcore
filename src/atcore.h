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
private:
    void findFirmware(const QByteArray& message);
    AtCorePrivate *d;
};

#endif
