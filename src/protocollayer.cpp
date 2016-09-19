#include "protocollayer.h"

//#include <QFile>
//#include <QEventLoop>
//#include <QTextStream>
//#include <QDebug>

ProtocolLayer::ProtocolLayer(const QString& port, uint baud, QWidget *parent) :
    SerialLayer(port, baud, parent)
{
    connect(this, &SerialLayer::receivedCommand, this, &ProtocolLayer::newMessage);
}

ProtocolLayer::~ProtocolLayer()
{
}
void ProtocolLayer::newMessage(const QByteArray& msg)
{
    lastMessage = msg;
    emit(receivedMessage(lastMessage));
}
