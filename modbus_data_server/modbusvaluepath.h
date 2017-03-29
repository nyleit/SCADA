#ifndef MODBUSVALUEPATH_H
#define MODBUSVALUEPATH_H
#include <QString>
#include <QStringList>

class modbusValuePath
{
public:
    modbusValuePath(QString modbus_url,int modbus_id,int register_type,int register_number);
    modbusValuePath(QString modbus_ip,int modbus_port,int modbus_id,int register_type,int register_number);
    modbusValuePath(QString path);

    QString path();
    QString path(int register_number);
    int registerNumber();
    QString url();

private:
    QString modbus_url;
    QString modbus_ip;
    int modbus_port;
    int modbus_id;
    int register_type;
    int register_number;
};

#endif // MODBUSVALUEPATH_H
