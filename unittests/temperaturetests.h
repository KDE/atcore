#include <QtTest>
#include <QObject>
#include "../src/temperature.h"

class TemperatureTests: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void setExtruderTemperature();
    void setExtruderTargetTemperature();
    void setBedTemperature();
    void setBedTargetTemperature();
    void testDecodeRepetier();
    void testDecode1();
    void testDecode2();
    void testDecode3();
    void testDecode4();

private:
    Temperature *temperature;
};
