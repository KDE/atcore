#include <QtTest>
#include <QObject>
#include "../src/temperature.h"

class TemperatureTests: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanup();
    void setExtruderTemperature();
    void setExtruderTargetTemperature();
    void setBedTemperature();
    void setBedTargetTemperature();
    void testDecodeAprinter();
    void testDecodeRepetier();
    void testDecodeMarlin();
    void testDecodeTeacup();

private:
    Temperature *temperature;
};
