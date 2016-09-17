#include "protocollayer.h"

#include <QFile>
#include <QEventLoop>
#include <QTextStream>

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

void ProtocolLayer::print(const QString& fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    QTextStream gcodestream(&file);
    QString cline;
    QEventLoop loop;
    connect(this, &ProtocolLayer::receivedCommand, &loop, &QEventLoop::quit);

    while(!gcodestream.atEnd()){
        cline = gcodestream.readLine();
        cline = cline.simplified();
        if(cline.contains(QChar(';'))){
            cline.resize(cline.indexOf(QChar(';')));
        }
        if(!cline.isEmpty()){
            pushCommand(cline.toLocal8Bit());
            bool waiting = true;
            while(waiting){
                if(!commandAvailable()){
                    loop.exec();
                }
                //Handle Messages from printer.
                if (lastMessage == "ok 0"){
                    waiting = false;
                }

                else if (lastMessage == "wait"){
                    waiting = false;
                }
                /* Handle resend and ok 1 soon*/
            }
        }
    }
    disconnect(this, &ProtocolLayer::receivedCommand, &loop, &QEventLoop::quit);
}
