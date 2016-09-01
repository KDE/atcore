#pragma once

#include "seriallayer.h"

class  ProtocolLayer : public SerialLayer
{
Q_OBJECT

public:
    ProtocolLayer(QString port, uint baud, QWidget *parent = 0);
    ~ProtocolLayer();

};
