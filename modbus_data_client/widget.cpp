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
{/*
    "127.0.0.1:502::1::4::2"  ::  3
    "127.0.0.1:502::1::4::3"  ::  4
    "127.0.0.1:502::1::4::4"  ::  4
    "127.0.0.1:502::1::4::5"  ::  3
    "127.0.0.1:502::1::4::6"  ::  2
    "127.0.0.1:502::1::4::7"  ::  1
    "127.0.0.1:502::1::4::8"  ::  66
    "127.0.0.1:502::1::4::9"  ::  77
    */
    QString num;
    QString str = ui->lineEdit_server_ip->text() +":"
                + ui->lineEdit_modbus_port->text() + "::"
                + ui->spinBox_modbus_id->text() + "::"
                + num.setNum(ui->comboBox_register->currentIndex()) + "::"
                + ui->spinBox_start->text() + "|"
                + ui->spinBox_count->text() + "\n";
    client->write(str.toUtf8());
}

void Widget::clientConnected()
{
    qDebug() << " client connected to server" << endl;
}
