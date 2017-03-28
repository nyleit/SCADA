#ifndef MODBUSDATACLIENT_H
#define MODBUSDATACLIENT_H

#include <QObject>
#include <QTcpSocket>

class ModbusDataClient : public QTcpSocket
{
    Q_OBJECT
public:
    ModbusDataClient(QObject *parent=0);

};

#endif // MODBUSDATACLIENT_H
