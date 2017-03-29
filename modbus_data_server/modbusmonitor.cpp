#include "modbusmonitor.h"
#include <QTimer>
#include <QDebug>
#include <QUrl>

modbusMonitor::modbusMonitor(QString u,QMap<QString,quint16>* m,QObject *parent)
    : QObject(parent),_url(u),values(m)
{
    int index = 1;
    modbusDevice = 0;
    modbusDevice = 0;
    if (modbusDevice) {
        modbusDevice->disconnectDevice();
        delete modbusDevice;
        modbusDevice = nullptr;
    }

    auto type = static_cast<ModbusConnection> (index);
    if (type == Serial) {
        modbusDevice = new QModbusRtuSerialMaster(this);
    } else if (type == Tcp) {
        modbusDevice = new QModbusTcpClient(this);
    }

   /* connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        statusBar()->showMessage(modbusDevice->errorString(), 5000);
    });
*/
    if (!modbusDevice) {
        if (type == Serial)
            qDebug() << tr("Could not create Modbus master.");
        else
            qDebug() << tr("Could not create Modbus client.");
    } else {
        connect(modbusDevice, SIGNAL(stateChanged(QModbusDevice::State))
                ,this , SLOT(onStateChanged(QModbusDevice::State)));
    }

    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
        if (type == Serial) {
            modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                "");
            modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
                "");
            modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                "");
            modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                "");
            modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                "");
        } else {
            const QUrl url = QUrl::fromUserInput(_url);
            modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
            modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());
        }
        modbusDevice->setTimeout(300);
        modbusDevice->setNumberOfRetries(2);
        if (!modbusDevice->connectDevice()) {
            qDebug() << tr("Connect failed: ") + modbusDevice->errorString();
        } else {
            qDebug() << tr("Connect Success ... ") ;
            timer = new QTimer;
            connect(timer,SIGNAL(timeout()),this,SLOT(startRead()));
        }
    } else {
        modbusDevice->disconnectDevice();
        qDebug() << tr("Device disConnected ... ") ;
    }
}

void modbusMonitor::startMonitor()
{
    timer->start(1000);
}

void modbusMonitor::stopMonitor()
{
    timer->stop();
}

void modbusMonitor::onStateChanged(QModbusDevice::State state)
{
    //bool connected = (state != QModbusDevice::UnconnectedState);
    if (state == QModbusDevice::UnconnectedState)
        qDebug() << tr("Connect");
    else if (state == QModbusDevice::ConnectedState)
        qDebug() << tr("Disconnect");
}

void modbusMonitor::addReadRequest(int id,int type,int start,int end)
{
    modbusDataReadRequest req;
    req._id = id;
    req._type = type;
    req._start = start;
    req._end = end;
    readRequestTable.push_back(req);
    qDebug() << "Read Request Added : id = " << id << " ,type = " << type << " ,start = " << start << " ,end = " << end ;

}

void modbusMonitor::startRead()
{
    if (!modbusDevice)
        return;
    foreach (modbusDataReadRequest req , readRequestTable) {
        if (auto *reply = modbusDevice->sendReadRequest(
                    readRequest(req._type,req._start,req._end), req._id)) {
            if (!reply->isFinished())
                connect(reply, &QModbusReply::finished, this, &modbusMonitor::readReady);
            else
                delete reply; // broadcast replies return immediately
        } else {
            qDebug() << tr("Read error: ") + modbusDevice->errorString();
        }
    }

}

void modbusMonitor::readReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        for (uint i = 0; i < unit.valueCount(); i++) {
            modbusValuePath mod_path(_url,reply->serverAddress(),unit.registerType(),unit.startAddress()+i);
            values->insert(mod_path.path(),unit.value(i));
        }
    } else if (reply->error() == QModbusDevice::ProtocolError) {
        qDebug() <<tr("Read response error: %1 (Mobus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, 10);
    } else {
        qDebug() <<tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16);
    }

    reply->deleteLater();
}


QModbusDataUnit modbusMonitor::readRequest(int t,int s,int c) const
{
    const auto table =
        static_cast<QModbusDataUnit::RegisterType> (t);

    int startAddress = s;
    Q_ASSERT(startAddress >= 0 && startAddress < 20);

    // do not go beyond 10 entries
    int numberOfEntries = qMin(c, c - startAddress);
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}

QModbusDataUnit modbusMonitor::writeRequest() const
{
    const auto table =
            static_cast<QModbusDataUnit::RegisterType> (4);

    int startAddress = 0;
    Q_ASSERT(startAddress >= 0 && startAddress < 10);

    // do not go beyond 10 entries
    int numberOfEntries = qMin(10, 10 - startAddress);
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}
