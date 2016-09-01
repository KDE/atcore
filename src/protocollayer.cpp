#include "protocollayer.h"

ProtocolLayer::ProtocolLayer(QString port, uint baud, QWidget *parent) :
	SerialLayer(port, baud, parent)
{
}

ProtocolLayer::~ProtocolLayer()
{
}