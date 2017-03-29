#include "modbusdataclient.h"

ModbusDataClient::ModbusDataClient(QObject *parent)
    :QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(processMessage()));
}

void ModbusDataClient::processMessage()
{
    QString msg;
    while(this->canReadLine())
    {
        msg = QString::fromUtf8(this->readLine().trimmed());
        QStringList list = msg.split("|");
        QString path (list[0]);
        int value = list.at(1).toInt();
        qDebug() << path<< " :: " << value;
    }
}
