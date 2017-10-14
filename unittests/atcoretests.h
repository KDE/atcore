#include <QtTest>
#include <QObject>

#include "../src/atcore.h"

class AtCoreTests: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void testInitState();
    void testPluginDetect();
    void testConnectInvalidDevice();
    void cleanupTestCase();
    void testPluginAprinter_load();
    void testPluginAprinter_validate();
    void testPluginGrbl_load();
    void testPluginGrbl_validate();
    void testPluginMarlin_load();
    void testPluginMarlin_validate();
    void testPluginRepetier_load();
    void testPluginRepetier_validate();
    void testPluginSmoothie_load();
    void testPluginSmoothie_validate();
    void testPluginSprinter_load();
    void testPluginSprinter_validate();
    void testPluginTeacup_load();
    void testPluginTeacup_validate();
private:
    AtCore *core = nullptr;
};
