#include "modbusdataserver.h"
#include <QTimer>
#include <QDebug>
#include <QUrl>

ModbusDataServer::ModbusDataServer(int p,QSettings *s,QObject *parent)
    :QTcpServer(parent),_setting(s),_port(p)
{
    loadModbusDevicesSettings();
    listen(QHostAddress::Any,_port);
    connect(this,SIGNAL(newConnection()),this,SLOT(login()));
}

void ModbusDataServer::loadModbusDevicesSettings()
{
    qDebug() << _setting->childGroups();
    foreach (QString group, _setting->childGroups()) {
        _setting->beginGroup(group);
        modbusMonitor* dev = new modbusMonitor(_setting->value("ip").toString());
        _modbusDevices.push_back(dev);
        foreach (QString group, _setting->childGroups()) {
            _setting->beginGroup(group);
            int id = group.toInt();
            foreach (QString group, _setting->childGroups()) {
                _setting->beginGroup(group);
                if(group == "holding")
                dev->addReadRequest(id,4
                                    ,_setting->value("start").toInt()
                                    ,_setting->value("end").toInt());
                if(group == "coil")
                dev->addReadRequest(id,2
                                    ,_setting->value("start").toInt()
                                    ,_setting->value("end").toInt());
                _setting->endGroup();
            }
            _setting->endGroup();
        }
        _setting->endGroup();
        dev->startMonitor();
    }
}

void ModbusDataServer::operateData()
{

}

void ModbusDataServer::login()
{
    QTcpSocket *current_connection = this->nextPendingConnection();
    connect(current_connection,SIGNAL(disconnected()),this,SLOT(logout()));
    connect(current_connection,SIGNAL(readyRead()),this,SLOT(processMessage()));
    _users.push_back(current_connection);
    qDebug() << "login : " << current_connection->peerAddress().toString();
}

void ModbusDataServer::logout()
{
    QTcpSocket *current_connection = (QTcpSocket *) sender();
    _users.removeAll(current_connection);
    qDebug() << "logout : " << current_connection->peerAddress().toString();
    current_connection->deleteLater();
}

void ModbusDataServer::processMessage()
{
    QTcpSocket *current_connection = (QTcpSocket *) sender();
    QString msg;
    ModbusMessage modbus_msg;

    while(current_connection->canReadLine())
    {
        msg = QString::fromUtf8(current_connection->readLine().trimmed());
        modbus_msg.encapsulate(msg);
        qDebug() << modbus_msg._ip;
        qDebug() << modbus_msg._port;
        qDebug() << modbus_msg._operation;
        qDebug() << modbus_msg._register;
        qDebug() << modbus_msg._start;
        qDebug() << modbus_msg._count;
    }

}
