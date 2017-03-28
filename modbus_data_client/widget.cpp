#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    client = new ModbusDataClient;
    connect(client,SIGNAL(connected()),this,SLOT(clientConnected()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_connect_to_server_clicked()
{
    QHostAddress addr(ui->lineEdit_server_ip->text());
    int port = (quint16)  ui->lineEdit_server_port->text().toInt();
    client->connectToHost(addr , port);
}

void Widget::on_pushButton_getdata_clicked()
{
    ModbusMessage mod_msg(ui->lineEdit_modbus_ip->text()
                          ,ui->lineEdit_modbus_port->text().toInt()
                         ,(ModbusMessage::OPERATION)ui->comboBox_operation->currentIndex()
                         ,(ModbusMessage::REGISTER)ui->comboBox_register->currentIndex()
                         ,ui->spinBox_start->value()
                         ,ui->spinBox_count->value());
    client->write(mod_msg.capsulate().toUtf8());
}

void Widget::clientConnected()
{
    qDebug() << " client connected to server" << endl;
}
