#include <QtTest>
#include <QObject>

class AtCoreTests: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void testStartAtCore();
    void cleanupTestCase();
};
