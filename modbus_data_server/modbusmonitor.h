#ifndef MODBUSMONITOR_H
#define MODBUSMONITOR_H

#include <QObject>
#include <QModbusDataUnit>
#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QTimer>

enum ModbusConnection {
    Serial,
    Tcp
};

struct modbusDataReadRequest
{
    int _id;        // modbus id
    int _type;      // holding , input ,coil
    int _start;     // start register
    int _end;       // end register
};

class modbusMonitor : public QObject
{
    Q_OBJECT
public:
    explicit modbusMonitor(QString,QObject *parent = 0);

    void addReadRequest(int id,int type,int start,int end);
    void startMonitor();
    void stopMonitor();

signals:

public slots:

private:
    QModbusDataUnit readRequest(int,int,int) const;
    QModbusDataUnit writeRequest() const;

private slots:
    void onStateChanged(QModbusDevice::State);
    void startRead();
    void readReady();

private:
    QTimer *timer;
    QString _url;
    QModbusReply *lastRequest;
    QModbusClient *modbusDevice;
    QVector<modbusDataReadRequest> readRequestTable;
};

#endif // MODBUSMONITOR_H
