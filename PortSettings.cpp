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

    if(QFile::exists(GlobalVars::filePathCOMportSettings)){
        QFile file(GlobalVars::filePathCOMportSettings);
        if(file.open(QIODevice::ReadOnly)){
            QTextStream in(&file);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                //qDebug()<<" file LIne:: "<<line;
                processSettingsLine(line);
            }
            file.close();

        } else {
            qDebug()<<" Error in Opening SettingsFile: "<<GlobalVars::filePathCOMportSettings;
        }
    }
    else {
        qDebug()<<" COM Port Setting File did not exist ";
    }


}


void PortSettings::processSettingsLine(QString line)
{
    if(line.length() < 3) return;
    //qDebug()<<" Serial COM Port Processing Line: "<<line;
    int index = line.indexOf(",");
    //qDebug()<<" ID Index : "<<index;
    if(index < 0) return;
    bool okBool = false;
    int parameterID = line.left(index).toInt(&okBool);
    line.remove(0, index+1);

    index = line.indexOf(",");
    //qDebug()<<" Value Index : "<<index<<" line: "<<line;
    if(index < 0) return;
    okBool = false;
    QString parameterValue = line.left(index);

    //qDebug()<<" paraID: "<<parameterID<<" Serial Port Name Value: "<<parameterValue;


    int findIndex = -1;
    switch (parameterID) {
    case 1:{
        findIndex = ui->cmbCOMportName->findText(parameterValue);
        if(findIndex>-1) ui->cmbCOMportName->setCurrentIndex(findIndex);
        break;
    }
    case 2:{
        findIndex = ui->cmbBaudRate->findText(parameterValue);
        if(findIndex>-1) ui->cmbBaudRate->setCurrentIndex(findIndex);
        break;
    }
    case 3:{
        findIndex = ui->cmbDataBits->findText(parameterValue);
        if(findIndex>-1) ui->cmbDataBits->setCurrentIndex(findIndex);
        break;
    }
    case 4:{
        findIndex = ui->cmbParity->findText(parameterValue);
        if(findIndex>-1) ui->cmbParity->setCurrentIndex(findIndex);
        break;
    }
    case 5:{
        findIndex = ui->cmbStopBits->findText(parameterValue);
        if(findIndex>-1) ui->cmbStopBits->setCurrentIndex(findIndex);
        break;
    }
    case 6:{
        findIndex = ui->cmbParityErrorChar->findText(parameterValue);
        if(findIndex>-1) ui->cmbParityErrorChar->setCurrentIndex(findIndex);
        break;
    }
    default:
        break;
    }
    //rxInitializeSerialPort(parameterValue);

}



void PortSettings::on_pbCancel_clicked()
{

    this->close();
}


void PortSettings::on_pbConfiguration_clicked()
{
    QString fileData = QString("1,"+ui->cmbCOMportName->currentText()+",\n ");
    fileData += QString("2,"+ui->cmbBaudRate->currentText()+",\n ");
    fileData += QString("3,"+ui->cmbDataBits->currentText()+",\n ");
    fileData += QString("4,"+ui->cmbParity->currentText()+",\n ");
    fileData += QString("5,"+ui->cmbStopBits->currentText()+",\n ");
    fileData += QString("6,"+ui->cmbParityErrorChar->currentText()+",\n ");


    QFile file(GlobalVars::filePathCOMportSettings);
    if(file.open(QIODevice::WriteOnly)){
        //qDebug()<<" Settings File Data:: \n"<<fileData;
        file.write(fileData.toUtf8(), fileData.toUtf8().length());
        file.close();
    } else {
        qDebug()<<" Error in Opening SettingsFile: "<<GlobalVars::filePathCOMportSettings;
    }


    emit txNewSerialPortConfig(ui->cmbCOMportName->currentText());
    this->close();
}

