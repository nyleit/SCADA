#ifndef MODBUSDATASERVER_H
#define MODBUSDATASERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <modbusmonitor.h>
#include <QSettings>
#include <modbusvaluepath.h>

class ModbusDataServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ModbusDataServer(int,QSettings *,QObject *parent = 0);

public slots:
    void login();
    void logout();
    void processMessage();
    void operateData();
    void loadModbusDevicesSettings();

    void printvalues();

private:
    int _port;
    QVector<modbusMonitor *> _modbusDevices;
    QSettings* _setting;
    QVector<QTcpSocket*> _users;
    QMap<QString,quint16> *all_modbus_values;
};

#endif // MODBUSDATASERVER_H
