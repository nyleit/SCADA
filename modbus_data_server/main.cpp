#include <QCoreApplication>
#include <QSettings>
#include <modbusdataserver.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSettings *settings = new QSettings("./setting.conf" ,QSettings::IniFormat);
    settings->beginGroup("slave_1");
    settings->setValue("ip","127.0.0.1:502");
    settings->beginGroup("1");
    settings->beginGroup("holding");
    settings->setValue("start",0);
    settings->setValue("end",15);
    settings->endGroup();
    settings->beginGroup("coil");
    settings->setValue("start",15);
    settings->setValue("end",20);
    settings->endGroup();
    settings->endGroup();

    settings->beginGroup("2");
    settings->beginGroup("holding");
    settings->setValue("start",0);
    settings->setValue("end",8);
    settings->endGroup();
    settings->beginGroup("coil");
    settings->setValue("start",8);
    settings->setValue("end",12);
    settings->endGroup();
    settings->endGroup();

    settings->endGroup();
    settings->sync();
    ModbusDataServer *mds = new ModbusDataServer(5000,settings);
    return a.exec();
}
