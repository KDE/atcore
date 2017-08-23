#include "temperaturetests.h"
#include <algorithm>

void TemperatureTests::initTestCase()
{
    temperature = new Temperature(this);
}

void TemperatureTests::cleanupTestCase()
{

}

void TemperatureTests::setExtruderTemperature()
{
    temperature->setExtruderTemperature(20);
    QVERIFY(temperature->extruderTemperature() == 20);

    temperature->setExtruderTemperature(20.25);
    QVERIFY(temperature->extruderTemperature() == 20.25);
}

void TemperatureTests::setExtruderTargetTemperature()
{
    temperature->setExtruderTargetTemperature(20);
    QVERIFY(temperature->extruderTargetTemperature() == 20);

    temperature->setExtruderTargetTemperature(20.25);
    QVERIFY(temperature->extruderTargetTemperature() == 20.25);
}

void TemperatureTests::setBedTemperature()
{
    temperature->setBedTemperature(20);
    QVERIFY(temperature->bedTemperature() == 20);

    temperature->setBedTemperature(20.25);
    QVERIFY(temperature->bedTemperature() == 20.25);
}

void TemperatureTests::setBedTargetTemperature()
{
    temperature->setBedTargetTemperature(20);
    QVERIFY(temperature->bedTargetTemperature() == 20);

    temperature->setBedTargetTemperature(20.25);
    QVERIFY(temperature->bedTargetTemperature() == 20.25);
}

void TemperatureTests::testDecodeRepetier()
{
    temperature->decodeTemp(QByteArray("T:25.47 /230 B:69.42 /80 B@:255 @:0"));
    QVERIFY(temperature->extruderTemperature() == float(25.47));
    QVERIFY(temperature->extruderTargetTemperature() == 230);
    QVERIFY(temperature->bedTemperature() == float(69.42));
    QVERIFY(temperature->bedTargetTemperature() == 80);
    temperature->setExtruderTemperature(0);
    temperature->setExtruderTargetTemperature(0);
    temperature->setBedTemperature(0);
    temperature->setBedTargetTemperature(0);
}

void TemperatureTests::testDecodeMarlin()
{
    temperature->decodeTemp(QByteArray("ok T:49.74 /60.00 B:36.23 /80.00 @:0 B@:0"));
    QVERIFY(temperature->extruderTemperature() == float(49.74));
    QVERIFY(temperature->extruderTargetTemperature() == 60);
    QVERIFY(temperature->bedTemperature() == float(36.23));
    QVERIFY(temperature->bedTargetTemperature() == 80);
    temperature->setExtruderTemperature(0);
    temperature->setExtruderTargetTemperature(0);
    temperature->setBedTemperature(0);
    temperature->setBedTargetTemperature(0);
}

void TemperatureTests::testDecode1()
{
    temperature->decodeTemp(QByteArray("T:20 /60 B:40 /80"));
    QVERIFY(temperature->extruderTemperature() == 20);
    QVERIFY(temperature->extruderTargetTemperature() == 60);
    QVERIFY(temperature->bedTemperature() == 40);
    QVERIFY(temperature->bedTargetTemperature() == 80);
    temperature->setExtruderTemperature(0);
    temperature->setExtruderTargetTemperature(0);
    temperature->setBedTemperature(0);
    temperature->setBedTargetTemperature(0);
}

void TemperatureTests::testDecode2()
{
    temperature->decodeTemp(QByteArray("T:20 / 60 B:40 / 80"));
    QVERIFY(temperature->extruderTemperature() == 20);
    QVERIFY(temperature->extruderTargetTemperature() == 60);
    QVERIFY(temperature->bedTemperature() == 40);
    QVERIFY(temperature->bedTargetTemperature() == 80);
    temperature->setExtruderTemperature(0);
    temperature->setExtruderTargetTemperature(0);
    temperature->setBedTemperature(0);
    temperature->setBedTargetTemperature(0);
}

void TemperatureTests::testDecode3()
{
    temperature->decodeTemp(QByteArray("T:20/60 ?:0/8 B:40/80"));
    QVERIFY(temperature->extruderTemperature() == 20);
    QVERIFY(temperature->extruderTargetTemperature() == 60);
    QVERIFY(temperature->bedTemperature() == 40);
    QVERIFY(temperature->bedTargetTemperature() == 80);
    temperature->setExtruderTemperature(0);
    temperature->setExtruderTargetTemperature(0);
    temperature->setBedTemperature(0);
    temperature->setBedTargetTemperature(0);
}
QTEST_MAIN(TemperatureTests)
