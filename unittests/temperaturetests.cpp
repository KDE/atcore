#include <algorithm>

#include "temperaturetests.h"

void TemperatureTests::initTestCase()
{
    temperature = new Temperature(this);
}

void TemperatureTests::cleanup()
{
    temperature->setBedTemperature(0);
    temperature->setBedTargetTemperature(0);
    temperature->setExtruderTemperature(0);
    temperature->setExtruderTargetTemperature(0);
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

void TemperatureTests::testDecodeAprinter()
{
    temperature->decodeTemp(QByteArray("ok B:49.06 /55 T:64.78 /215"));
    QVERIFY(temperature->extruderTemperature() == float(64.78));
    QVERIFY(temperature->extruderTargetTemperature() == 215);
    QVERIFY(temperature->bedTemperature() == float(49.06));
    QVERIFY(temperature->bedTargetTemperature() == 55);
}

void TemperatureTests::testDecodeMarlin()
{
    temperature->decodeTemp(QByteArray("ok T:49.74 /60.00 B:36.23 /50.00 @:0 B@:0"));
    QVERIFY(temperature->extruderTemperature() == float(49.74));
    QVERIFY(temperature->extruderTargetTemperature() == 60);
    QVERIFY(temperature->bedTemperature() == float(36.23));
    QVERIFY(temperature->bedTargetTemperature() == 50);
}

void TemperatureTests::testDecodeRepetier()
{
    temperature->decodeTemp(QByteArray("T:25.47 /230 B:69.42 /80 B@:255 @:0"));
    QVERIFY(temperature->extruderTemperature() == float(25.47));
    QVERIFY(temperature->extruderTargetTemperature() == 230);
    QVERIFY(temperature->bedTemperature() == float(69.42));
    QVERIFY(temperature->bedTargetTemperature() == 80);
}
void TemperatureTests::testDecodeSmoothie()
{
    temperature->decodeTemp(QByteArray("ok T:76.36 /220.0 @0 B:24.1 /60.0 @"));
    QVERIFY(temperature->extruderTemperature() == float(76.36));
    QVERIFY(temperature->extruderTargetTemperature() == 220);
    QVERIFY(temperature->bedTemperature() == float(24.1));
    QVERIFY(temperature->bedTargetTemperature() == 60);

}
void TemperatureTests::testDecodeSprinter()
{
    temperature->decodeTemp(QByteArray("ok T:154 @:0 B:150"));
    QVERIFY(temperature->extruderTemperature() == 154);
    QVERIFY(temperature->bedTemperature() == 150);
}

void TemperatureTests::testDecodeTeacup()
{
    temperature->decodeTemp(QByteArray("T:15.50/210.0 B:46.80/82.0"));
    QVERIFY(temperature->extruderTemperature() == float(15.50));
    QVERIFY(temperature->extruderTargetTemperature() == 210);
    QVERIFY(temperature->bedTemperature() == float(46.80));
    QVERIFY(temperature->bedTargetTemperature() == 82);
}

QTEST_MAIN(TemperatureTests)
