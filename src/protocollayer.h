#pragma once

#include "seriallayer.h"
#include <QObject>
#include <QString>

class  ProtocolLayer : public SerialLayer
{
Q_OBJECT

public:
    ProtocolLayer(const QString& port, uint baud, QWidget *parent = 0);
    /**
     * @brief Print a file
     *
     * @param fileName : path of file to print
     */
    void print(const QString& fileName);
    ~ProtocolLayer();

signals:
    /**
     * @brief Emit signal when message is received from the printer
     *
     * @param msg : Message
     */
    void receivedMessage(const QByteArray& msg);

private:
    QByteArray lastMessage;
    void newMessage(const QByteArray& msg);

};
