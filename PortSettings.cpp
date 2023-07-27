#include "PortSettings.h"
#include "ui_PortSettings.h"

PortSettings::PortSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PortSettings)
{
    ui->setupUi(this);
    this->setWindowTitle("Port Settings");


    //this->setMinimumSize(guiWidth, guiHeight);
    this->setGeometry(0,0, 440, 390);
    if(GlobalVars::ACTORis == ActorUSER) {
        //this->setWindowState(Qt::WindowMaximized);
        //this->setWindowState(Qt::WindowFullScreen);
        this->setWindowFlag(Qt::WindowStaysOnTopHint);
    } else if(GlobalVars::ACTORis == ActorAdministrator) {

    }
    //this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);

    timerSingleShot = new QTimer(this);
    timerSingleShot->setSingleShot(true);
    connect(timerSingleShot, SIGNAL(timeout()), this, SLOT(onTimerSingleShotElapsed()));
    timerSingleShot->start(400);



}

PortSettings::~PortSettings()
{
    delete ui;
}


void PortSettings::onTimerSingleShotElapsed()
{
    ui->cmbCOMportName->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        //qDebug()<<"--------------------------------------";
        //qDebug()<<"Name        : "<< info.portName();
        //qDebug()<<"Description : "<< info.description();
        //qDebug()<<"Manufacturer: "<< info.manufacturer();
        // Example use QSerialPort
        ui->cmbCOMportName->addItem(info.portName());
    }

    qDebug()<<"--------------------------------------";


}




void PortSettings::on_pbCancel_clicked()
{

    this->close();
}


void PortSettings::on_pbConfiguration_clicked()
{
    QString fileData = QString("1,"+ui->cmbCOMportName->currentText()+",\n ");
    QFile file(GlobalVars::filePathCOMportSettings);
    if(file.open(QIODevice::WriteOnly)){
        qDebug()<<" Settings File Data:: \n"<<fileData;
        file.write(fileData.toUtf8(), fileData.toUtf8().length());
        file.close();
    } else {
        qDebug()<<" Error in Opening SettingsFile: "<<GlobalVars::filePathCOMportSettings;
    }


    emit txNewSerialPortConfig(ui->cmbCOMportName->currentText());
    this->close();
}

