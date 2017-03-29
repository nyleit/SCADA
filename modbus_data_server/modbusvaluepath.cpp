#include "modbusvaluepath.h"

modbusValuePath::modbusValuePath(QString u,int i,int t,int n)
    :modbus_url(u), modbus_id(i), register_type(t), register_number(n)
{

}

modbusValuePath::modbusValuePath(QString ip,int p,int i,int t,int n)
    :modbus_ip(ip),modbus_port(p), modbus_id(i), register_type(t), register_number(n)
{

}

modbusValuePath::modbusValuePath(QString path)
{
    QStringList list = path.split("::");
    modbus_url = list.at(0);
    modbus_id = list.at(1).toInt();
    register_type = list.at(2).toInt();
    register_number = list.at(3).toInt();
}

QString modbusValuePath::path()
{
    QString p,num;
    p += modbus_url + "::";
    p += num.setNum(modbus_id) + "::";
    p += num.setNum(register_type) + "::";
    p += num.setNum(register_number);
    return p;
}

QString modbusValuePath::path(int n)
{
    register_number = n;
    QString p,num;
    p += modbus_url + "::";
    p += num.setNum(modbus_id) + "::";
    p += num.setNum(register_type) + "::";
    p += num.setNum(register_number);
    return p;
}

QString modbusValuePath::url()
{
    return modbus_url;
}

int modbusValuePath::registerNumber()
{
    return register_number;
}
