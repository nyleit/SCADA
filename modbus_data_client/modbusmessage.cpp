#include "modbusmessage.h"

ModbusMessage::ModbusMessage()
{

}

ModbusMessage::ModbusMessage(QString i ,int p ,OPERATION o ,REGISTER r ,int s ,int c)
    :_ip(i),_port(p),_operation(o),_register(r),_start(s),_count(c)
{

}

void ModbusMessage::encapsulate(QString str)
{
    QStringList str_list = str.split(",");
    _ip  =   str_list.at(0);
    _port      =   str_list.at(1).toInt();
    _operation  =   (OPERATION)str_list.at(2).toInt();
    _register   =   (REGISTER)str_list.at(3).toInt();
    _start      =   str_list.at(4).toInt();
    _count      =   str_list.at(5).toInt();
}

QString ModbusMessage::capsulate()
{
    QString str;
    str = _ip;
    str = str + ","+ str.setNum(_port);
    str = str + ","+ str.setNum(_operation);
    str = str + ","+ str.setNum(_register);
    str = str + ","+ str.setNum(_start);
    str = str + ","+ str.setNum(_count);
    str = str + "\n";
    return str;
}
