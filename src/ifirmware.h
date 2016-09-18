#ifndef IFIRMWARE
#define IFIRMWARE

#include <QObject>
#include <QString>

class IFirmware : public QObject {
    Q_OBJECT
public:
    IFirmware(QObject *parent = 0);
    virtual QString name() const = 0;
    // ADD THE METHODS HERE.
};

Q_DECLARE_INTERFACE(IFirmware, "org.kde.atelier.core.firmware")

#endif
