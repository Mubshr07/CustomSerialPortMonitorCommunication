#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GlobalVars::initEveryThing();


    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->PortBox->addItem(info.portName());
    }
    ui->PortBox->setCurrentIndex(0);

    ui->BaudBox->setCurrentIndex(5); //


    timerSingleShot = new QTimer(this);
    timerSingleShot->setSingleShot(true);
    connect(timerSingleShot, SIGNAL(timeout()), this, SLOT(onTimerSingleShotElapsed()));
    timerSingleShot->start(1000);


    qDebug()<<"all cleared";
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pbClear_clicked()
{
    receiveIndex = 0;
    ui->textBrowser->clear();
}
void MainWindow::on_pbStart_clicked()
{

}
void MainWindow::on_pbClose_clicked()
{
    m_serialPort.close();
    this->close();
}
void MainWindow::on_openButton_clicked()
{
    toggleOpenSerial  = !toggleOpenSerial ;

    if(toggleOpenSerial ) {
        m_serialPort.setPortName(ui->PortBox->currentText());
        m_serialPort.setBaudRate(ui->BaudBox->currentText().toInt());
        m_serialPort.flush();
        m_serialPort.clear();

        if(m_serialPort.open(QIODevice::ReadWrite)){
            m_serialPort.flush();
            m_serialPort.clear();
            ui->openButton->setText(tr("listening\nMode"));
            ui->openButton->setStyleSheet("background-color:lime; color: black;");
            qDebug()<<" Serial Port opend: name: "<<m_serialPort.portName();
        } else {
            ui->openButton->setText(tr("Error"));
            ui->openButton->setStyleSheet("background-color:red; color: black;");
            qDebug()<<"+++++++++ Serial Port Error: name: "<<m_serialPort.portName();
        }
        m_serialPort.flush();
        m_serialPort.clear();
        connect(&m_serialPort, SIGNAL(readyRead()), this, SLOT(onSerialReadyData()));

        ui->PortBox->setEnabled(false);
        ui->BaudBox->setEnabled(false);

    }
    else {
        m_serialPort.close();
        m_serialPort.clear();

        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->openButton->setText(tr("Stopped"));
        ui->openButton->setStyleSheet("background-color:transparent; color: black; border:0px solid black; border-left:2px solid black; border-bottom:2px solid black;");
    }
}



void MainWindow::onTimerSingleShotElapsed()
{
    on_openButton_clicked();
}

void MainWindow::onSerialReadyData()
{
    receivedBytes = m_serialPort.readAll();
    qDebug()<<" Read Data:: "<<receivedBytes;


    ui->textBrowser->setTextColor( QColor( "red" ) );
    ui->textBrowser->append(QString(QDateTime::currentDateTime().toString("HH:mm:ss:zzz")+" : " + receivedBytes.toHex().toUpper()));

    /*
    mlocker.tryLock();
    serialData += receivedBytes;
    mlocker.unlock();
    processSerialData();
    */



}


void MainWindow::processSerialData(){

    while(serialData.length()>4){
        //qDebug()<<"SerialPort Process: "<<serialData;

        indexOfHeader = serialData.indexOf("\xEA\x92");
        if(indexOfHeader > 0)
            serialData.remove(0, indexOfHeader);

        indexOfHeader = serialData.indexOf("\xAA");
        //qDebug()<<" index of Header is: "<<indexOfHeader;
        singlePacket = serialData.left(indexOfHeader+1);
        processSerialSinglePacket(singlePacket);
        serialData.remove(0, indexOfHeader+1);
    }

}
void MainWindow::processSerialSinglePacket(QByteArray ba){

    currentPacketType = static_cast<int>(singlePacket[2]);
    int channel = static_cast<int>(singlePacket[3]);
    int status = static_cast<int>(singlePacket[4]);

    qDebug()<<receiveIndex<<". Single Packet ID: "<<singlePacket<<" -> "<<currentPacketType;
    ui->textBrowser->setTextColor( QColor( "red" ) );
    ui->textBrowser->append(QString(QDateTime::currentDateTime().toString("HH:mm:ss:zzz")+" : " + ba.toHex().toUpper()));
    //ui->textBrowser->append(QString(QString::number(receiveIndex)+" : " + singlePacket.toHex().toUpper()));

}




void MainWindow::serialSendTheseBytes(QByteArray ba)
{
    //qDebug()<<" ------------------------";
    //qDebug()<<" sending:: "<<ba<<" length:"<<ba.length();
    if(ba != serialBytesTxhandShake ){
        qDebug()<<" sending:: "<<ba<<" length:"<<ba.length();
        ui->textBrowser->setTextColor( QColor( "green" ) );
        //ui->textBrowser->append(QString(QDateTime::currentDateTime().toString("HH:mm:ss:zzz")+" : " + ba.toHex().toUpper()));
        ui->textBrowser->append(QString(QString::number(receiveIndex)+" : " + ba.toHex().toUpper()));
        receiveIndex++;
    }

    qDebug()<<"number of Bytes written: "<<m_serialPort.write(ba, ba.length());
    qDebug()<<"\n";

}

float MainWindow::randomNumberGeneratorFloat(double min, double max)
{
    //srand((unsigned)time(NULL));
    int n = max - min;
    int remainder = RAND_MAX % n;
    int x;
    do{ x = rand(); }while (x >= RAND_MAX - remainder);

    QRandomGenerator gen1 =  QRandomGenerator::securelySeeded();
    return (float)((min + x % n) + gen1.generateDouble());

}



void MainWindow::on_pbSettings_clicked()
{
    portSettingObj = new PortSettings(this);


    portSettingObj->show();

}

