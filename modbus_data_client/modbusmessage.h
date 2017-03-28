#ifndef MODBUSMESSAGE_H
#define MODBUSMESSAGE_H

#include <QString>
#include <QStringList>

class ModbusMessage
{
public:
    enum OPERATION{
        READ,
        WRITE
    };

    enum REGISTER{
        INPUT,
        HOLDING,
        COIL
    };

public:
    ModbusMessage();
    ModbusMessage(QString,int,OPERATION,REGISTER,int,int);

    void encapsulate(QString);
    QString capsulate();

    QString _ip;
    int _port;
    OPERATION _operation;
    REGISTER  _register;
    int _start;
    int _count;
};

#endif // MODBUSMESSAGE_H
