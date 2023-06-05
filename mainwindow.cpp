#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->PortBox->addItem(info.portName());
    }
    ui->PortBox->setCurrentIndex(1);

    ui->BaudBox->setCurrentIndex(5);


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
            ui->openButton->setText(tr("OPened"));
            qDebug()<<" Serial Port opend: name: "<<m_serialPort.portName();
        } else {
            ui->openButton->setText(tr("Error"));
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
        ui->openButton->setText(tr("Open"));
    }
}



void MainWindow::onTimerSingleShotElapsed()
{
    on_openButton_clicked();
}

void MainWindow::onSerialReadyData()
{
    receivedBytes = m_serialPort.readAll();
    mlocker.tryLock();
    serialData += receivedBytes;
    mlocker.unlock();
    processSerialData();

    //qDebug()<<" Read Data:: "<<receivedBytes;


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

    if(currentPacketType != 0){
        qDebug()<<receiveIndex<<". Single Packet ID: "<<singlePacket<<" -> "<<currentPacketType;
        ui->textBrowser->setTextColor( QColor( "red" ) );
        //ui->textBrowser->append(QString(QDateTime::currentDateTime().toString("HH:mm:ss:zzz")+" : " + receivedBytes.toHex().toUpper()));
        ui->textBrowser->append(QString(QString::number(receiveIndex)+" : " + singlePacket.toHex().toUpper()));
    }
    switch (currentPacketType) {
    case 0: {  sendHandShakePacket(); break; }
    case 1:
    case 2:  { sendSelfTestPacket(channel, status); break; }
    case 3: { sendResistanceTestPacket(); break; }
    case 4: { sendPolarityTestPacket(channel, randomNumberGeneratorFloat(1.3, 2.6)); break; }
    case 5: { sendInsulationTestPacket(channel, randomNumberGeneratorFloat(1.3, 2.6)); break; }
    case 6: { sendLaunchTestPacket(channel); break; }
    default:
        break;
    }
}



void MainWindow::sendHandShakePacket()
{
    serialSendTheseBytes(serialBytesTxhandShake);
}
void MainWindow::sendSelfTestPacket(int channel, double data)
{
    serialBytesTxSelfTestSupplyVoltages[3] = channel;

    switch (channel) {
    case 1: {
        //serialBytesTxSelfTestSupplyVoltages[4] = randomNumberGeneratorFloat(toleranceSelfTestPowerSupply[0],toleranceSelfTestPowerSupply[1]);
        float d = randomNumberGeneratorFloat(toleranceSelfTestPowerSupply[0],toleranceSelfTestPowerSupply[1]);
        QByteArray dd = QByteArray(QString::number(d, 'f', 2).toUtf8()); //.toHex().toUpper();

        char data[sizeof(float)];
        memcpy(data, &d, sizeof(d));    // send data
        dd.clear();
        dd = QByteArray(data,sizeof(data));
        //qDebug()<<" ss:"<<data<<" data:"<<sizeof(data)<<" 0:"<<data[0]<<" 1:"<<data[1]<<" 2:"<<data[2]<<" 3:"<<data[3];

        QString ss = QString::number(d, 'f', 2);


        unsigned char const * p = reinterpret_cast<unsigned char const *>(&d);
        for (std::size_t i = 0; i != sizeof(float); ++i)
        {
            qDebug("The byte #%zu is 0x%02X\n", i, p[i]);
        }



        bool okk = false;
        uint32_t dword = d;
        //memcpy(&dword, &d, 4);
        qDebug()<<" dWord: "<<dword<<" -> "<<&dword<<QString::number(dword, 10)<<QString::number(dword, 16)<<QString::number(dword, 16).toFloat(&okk)<<sizeof(dword);

        qDebug()<<" byts: "<<dd.data()<<" "<<QString(dd.toHex())<<" ."<<dd.data()[0];
        qDebug()<<"\n Power Supply Value: "<<d<<" -> "<<dd.toFloat(&okk)<<" l:"<<dd.length()<<serialBytesTxSelfTestSupplyVoltages.length()<<" = "<<dd[0]<<dd.at(0)<<", "<<dd[1]<<", "<<dd[2]<<", "<<dd[3];
        serialBytesTxSelfTestSupplyVoltages[4] = (dword>>24) &0x00FF;
        serialBytesTxSelfTestSupplyVoltages[5] = (dword>>16) &0x00FF;
        serialBytesTxSelfTestSupplyVoltages[6] = (dword>>8) &0x00FF;
        serialBytesTxSelfTestSupplyVoltages[7] = (dword) &0x00FF;

        break;
    }
    case 2: {
        int32_t d = randomNumberGeneratorFloat(toleranceSelfTestPowerSupply[0],toleranceSelfTestPowerSupply[1]);
        qDebug()<<" Power Supply Value: "<<d;
        serialBytesTxSelfTestSupplyVoltages[4] = (d >> 8) & 0xFF;
        serialBytesTxSelfTestSupplyVoltages[5] = (d) & 0xFF;

        break;
    }
    case 3:{
        float d = randomNumberGeneratorFloat(4.5, 5.5);
        QByteArray *b = (QByteArray *) &d;
        qDebug()<<" ChnL:"<<channel<<" length:"<<b->length()<<" bytes:"<<b;
        for(int i=0; i<b->length(); i++){
            serialBytesTxSelfTestSupplyVoltages[i+4] = b->at(i);
        }
        break;
    }
    case 4: {
        float d = randomNumberGeneratorFloat(3.5, 5.0);
        QByteArray *b = (QByteArray *) &d;
        for(int i=0; i<b->length(); i++){ serialBytesTxSelfTestSupplyVoltages[i+4] = b->at(i); }
        break;
    }
    case 5: {
        float d = randomNumberGeneratorFloat(10.5, 12.5);
        QByteArray *b = (QByteArray *) &d;
        for(int i=0; i<b->length(); i++){ serialBytesTxSelfTestSupplyVoltages[i+4] = b->at(i); }
        break;
    }
    default: serialBytesTxSelfTestSupplyVoltages[4] = randomNumberGeneratorFloat(12.0, 20.0); break;
    }

    serialSendTheseBytes(serialBytesTxSelfTestSupplyVoltages);
}
void MainWindow::sendResistanceTestPacket()
{
    serialBytesTxResistanceTest[3] = 1;
    serialBytesTxResistanceTest[4] = 22;

    serialSendTheseBytes(serialBytesTxResistanceTest);
}
void MainWindow::sendPolarityTestPacket(int channel, double data)
{
    serialBytesTxPolarityTest[3] = channel;
    serialBytesTxPolarityTest[4] = data;
    /*
    switch (channel) {
    case 1: { serialBytesTxSelfTestSupplyVoltages[4] = randomNumberGeneratorFloat(toleranceSelfTestPowerSupply[0],toleranceSelfTestPowerSupply[1]); break; }
    case 2: { serialBytesTxSelfTestSupplyVoltages[4] = randomNumberGeneratorFloat(toleranceSelfTestPowerSupply[0],toleranceSelfTestPowerSupply[1]); break; }
    case 3: { serialBytesTxSelfTestSupplyVoltages[4] = randomNumberGeneratorFloat(4.5, 5.5); break; }
    case 4: { serialBytesTxSelfTestSupplyVoltages[4] = randomNumberGeneratorFloat(3.5, 5.0); break; }
    case 5: { serialBytesTxSelfTestSupplyVoltages[4] = randomNumberGeneratorFloat(12.0, 20.0); break; }
    default: serialBytesTxSelfTestSupplyVoltages[4] = randomNumberGeneratorFloat(12.0, 20.0); break;
    }
    */

    serialSendTheseBytes(serialBytesTxPolarityTest);
}
void MainWindow::sendInsulationTestPacket(int channel, double data)
{

}
void MainWindow::sendLaunchTestPacket( double time)
{

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


