#include "modbusdataserver.h"
#include <QTimer>
#include <QDebug>
#include <QUrl>

ModbusDataServer::ModbusDataServer(int p,QSettings *s,QObject *parent)
    :QTcpServer(parent),_setting(s),_port(p)
{
    all_modbus_values = new QMap<QString,quint16>;
    loadModbusDevicesSettings();
    listen(QHostAddress::Any,_port);
    connect(this,SIGNAL(newConnection()),this,SLOT(login()));
    QTimer *timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(printvalues()));
   // timer->start(1000);
}

void ModbusDataServer::printvalues()
{
    foreach (QString p, all_modbus_values->keys()) {
        qDebug() << p << " :: " << all_modbus_values->value(p);
    }
}

void ModbusDataServer::loadModbusDevicesSettings()
{
    qDebug() << _setting->childGroups();
    foreach (QString group, _setting->childGroups()) {
        _setting->beginGroup(group);
        modbusMonitor* dev = new modbusMonitor(_setting->value("ip").toString(),all_modbus_values);
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

    while(current_connection->canReadLine())
    {
        msg = QString::fromUtf8(current_connection->readLine().trimmed());
        QStringList list = msg.split("|");
        modbusValuePath path (list[0]);
        int start = path.registerNumber();
        int count = list.at(1).toInt();
        QString num;
        for(int i = start; i < start+count ;i++)
        {
            msg = path.path(i) + "|"+ num.setNum(all_modbus_values->value(path.path(i))) + "\n";
            current_connection->write(msg.toUtf8());
        }
      //  qDebug() << path.path(i) << " :: " << all_modbus_values->value(path.path(i));
    }
}
