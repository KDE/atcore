#ifndef IFIRMWARE
#define IFIRMWARE

#include <QObject>
#include <QString>

class IFirmware {
public:
    virtual QString name() const = 0;
    virtual bool readyForNextCommand(const QString& lastMessage) = 0;
    // ADD THE METHODS HERE.
};

Q_DECLARE_INTERFACE(IFirmware, "org.kde.atelier.core.firmware")

#endif
