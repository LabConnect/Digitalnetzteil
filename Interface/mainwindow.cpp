#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
extern "C" {              /*include C Header*/
#include "hiddata.h"
#include "../firmware/usbconfig.h"
}

usbDevice_t *dev = NULL;
char    buffer[3];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(isvalue()));
    timer->start(10);
    isvalue();
}

MainWindow::~MainWindow()
{
    delete ui;
}

static usbDevice_t  *openDevice(void)
{
usbDevice_t     *dev = NULL;
unsigned char   rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
char            vendorName[] = {USB_CFG_VENDOR_NAME, 0}, productName[] = {USB_CFG_DEVICE_NAME, 0};
int             vid = rawVid[0] + 256 * rawVid[1];
int             pid = rawPid[0] + 256 * rawPid[1];
    usbhidOpenDevice(&dev, vid, vendorName, pid, productName, 0);
    return dev;
}

void MainWindow::on_dialvoltage_valueChanged(int value)
{
    buffer[1]=value;
    ui->lcdvoltages->display(value);
    usbhidSetReport(dev,buffer,sizeof(buffer));
}

void MainWindow::on_dialampere_valueChanged(int value)
{
    buffer[2]=value;
    ui->lcdamperes->display(value);
    usbhidSetReport(dev,buffer,sizeof(buffer));
}
int i = 0;
void MainWindow::isvalue()
{
    char        buffer[3];
    if(dev != NULL){
    int len = sizeof(buffer);
    while(usbhidGetReport(dev,0,buffer,&len)!=0);
    ui->lcdvoltagei->display((u_int8_t)buffer[1]);
    ui->lcdamperei->display((u_int8_t)buffer[2]);
    }
}


void MainWindow::on_btnopen_clicked()
{
    if((dev=openDevice())==NULL)
        exit(1);
}

void MainWindow::on_btnclose_clicked()
{
    usbhidCloseDevice(dev);
    dev = NULL;
}
