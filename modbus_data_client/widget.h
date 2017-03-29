#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <modbusdataclient.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    ModbusDataClient *client;

public slots:
    void on_pushButton_connect_to_server_clicked();
    void on_pushButton_getdata_clicked();
    void clientConnected();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
