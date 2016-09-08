#pragma once

#include "seriallayer.h"

class  ProtocolLayer : public SerialLayer
{
Q_OBJECT

public:
    ProtocolLayer(QString port, uint baud, QWidget *parent = 0);
    /**
     * @brief Print a file
     *
     * @param fileName : path of file to print
     */
    void print(QString fileName);
    ~ProtocolLayer();
signals:
    /**
     * @brief Emit signal when message is received from the printer
     *
     * @param msg : Message
     */
    void receivedMessage(QByteArray msg);
private:
    QByteArray lastMessage;
    void newMessage(QByteArray msg);

};
