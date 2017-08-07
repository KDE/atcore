#include "atcoretests.h"
#include "../src/atcore.h"
#include <algorithm>

void AtCoreTests::initTestCase()
{
}

void AtCoreTests::cleanupTestCase()
{
}

void AtCoreTests::testStartAtCore()
{
    AtCore *core = new AtCore();
    Q_ASSERT(core->state() == AtCore::DISCONNECTED);

    QStringList fwPluginsFound = core->availableFirmwarePlugins();
    QStringList fwPluginsActual = {
        QStringLiteral("aprinter"),
        QStringLiteral("grbl"),
        QStringLiteral("marlin"),
        QStringLiteral("repetier"),
        QStringLiteral("smoothie"),
        QStringLiteral("sprinter"),
        QStringLiteral("teacup")
    };

    std::sort(fwPluginsFound.begin(), fwPluginsFound.end());
    std::sort(fwPluginsActual.begin(), fwPluginsActual.end());
    QVERIFY(fwPluginsFound == fwPluginsActual);

    //ERROR: Invalid Null Pointer but no way to test it as detectFirmware is void.
    // Also, it shouldn't fail gracefully without a good explanatory message like
    // "Serial Device is not open, maybe you forgot to call initSerial?"
    core->detectFirmware();

    //ERROR: initSerial can fail, but there's no way for us to test
    // if it actually opened or not.
    core->initSerial(QStringLiteral("/dev/ptyp5"), 9600);

    //ERROR: and because of that, the following line fails, as the initSerial also failed.
    QVERIFY(core->state() == AtCore::DISCONNECTED);

    core->detectFirmware();
}

QTEST_MAIN(AtCoreTests)
