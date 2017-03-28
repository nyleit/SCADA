#ifndef MODBUSDATASERVER_H
#define MODBUSDATASERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <modbusmessage.h>
#include <modbusmonitor.h>
#include <QSettings>

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

private:
    int _port;
    //QModbusReply *lastRequest;
    QVector<modbusMonitor *> _modbusDevices;
    QSettings* _setting;
    QVector<QTcpSocket*> _users;
};

#endif // MODBUSDATASERVER_H
