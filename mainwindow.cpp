#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataInit();
    timeInit();

    setAllButtonDisable();

    serialFlag=false;
    serial=new QSerialPort;//先给出一个串口类对象，稍后进行属性配置。
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 *数据初始化，用作给485串口命令的命令格式数据，发送给硬件。
 *界面初始化，给出端口号和波特率。
 */
void MainWindow::dataInit()
{
    CCStartDcm250b.resize(8);//DCM250B命令
    CCStartDcm250b[0]=0x03;
    CCStartDcm250b[1]=0x06;
    CCStartDcm250b[2]=0x00;
    CCStartDcm250b[3]=0x00;
    CCStartDcm250b[4]=0x00;
    CCStartDcm250b[5]=0X03;
    CCStartDcm250b[6]=0xC8;
    CCStartDcm250b[7]=0x29;

    CCDcm250b_Calibration.resize(8);//01
    CCDcm250b_Calibration[0]=0x03;
    CCDcm250b_Calibration[1]=0x06;
    CCDcm250b_Calibration[2]=0x00;
    CCDcm250b_Calibration[3]=0x00;
    CCDcm250b_Calibration[4]=0x00;
    CCDcm250b_Calibration[5]=0x01;
    CCDcm250b_Calibration[6]=0x49;
    CCDcm250b_Calibration[7]=0xE8;

    CCDcm250b_Save.resize(8);//02
    CCDcm250b_Save[0]=0x03;
    CCDcm250b_Save[1]=0x06;
    CCDcm250b_Save[2]=0x00;
    CCDcm250b_Save[3]=0x00;
    CCDcm250b_Save[4]=0x00;
    CCDcm250b_Save[5]=0x02;
    CCDcm250b_Save[6]=0x09;
    CCDcm250b_Save[7]=0xE9;

    ReadDcm250b.resize(8);//读取Dcm250B数据命令
    ReadDcm250b[0]=0x03;
    ReadDcm250b[1]=0x03;
    ReadDcm250b[2]=0x00;
    ReadDcm250b[3]=0x01;
    ReadDcm250b[4]=0x00;
    ReadDcm250b[5]=0x02;
    ReadDcm250b[6]=0x94;
    ReadDcm250b[7]=0x29;

    ReadGyroscope.resize(8);//读取Gyroscope数据命令
    ReadGyroscope[0]=0x01;
    ReadGyroscope[1]=0x03;
    ReadGyroscope[2]=0x00;
    ReadGyroscope[3]=0x00;
    ReadGyroscope[4]=0x00;
    ReadGyroscope[5]=0x02;
    ReadGyroscope[6]=0xC4;
    ReadGyroscope[7]=0x0B;

    ReadUltrasonicWave.resize(8);//读取UltrasonicWave数据命令
    ReadUltrasonicWave[0]=0x02;
    ReadUltrasonicWave[1]=0x03;
    ReadUltrasonicWave[2]=0x00;
    ReadUltrasonicWave[3]=0x00;
    ReadUltrasonicWave[4]=0x00;
    ReadUltrasonicWave[5]=0x08;
    ReadUltrasonicWave[6]=0x44;
    ReadUltrasonicWave[7]=0x3F;

    ReadPS2.resize(8);//读取PS2数据命令
    ReadPS2[0]=0x04;
    ReadPS2[1]=0x03;
    ReadPS2[2]=0x00;
    ReadPS2[3]=0x00;
    ReadPS2[4]=0x00;
    ReadPS2[5]=0x01;
    ReadPS2[6]=0x84;
    ReadPS2[7]=0x5F;

    QStringList baudList;
    baudList<<"2400"<<"4800"<<"9600"<<"14400"<<"19200"<<"38400"<<"57600"<<"115200";
    ui->listBaud->addItems(baudList);
    ui->listBaud->setCurrentIndex(2);

    QStringList portList;
    portList<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5"<<"COM6"<<"COM7"<<"COM8"<<"COM9"<<"COM10";
    ui->listPort->addItems(portList);
}

/*
 *超声波测试函数；
 */
void MainWindow::on_btnUltrasonic_clicked()
{
    serial->write(ReadUltrasonicWave);
}

/*
 * 陀螺仪测试函数；
 */
void MainWindow::on_btnGyroscope_clicked()
{
    serial->write(ReadGyroscope);
}

/*
 * 罗盘测试函数；
 */
void MainWindow::on_btnCompass_clicked()
{
    serial->write(CCStartDcm250b);
}

/*
 * 按键测试函数；
 */
void MainWindow::on_btnButton_clicked()
{
    serial->write(ReadPS2);
}

/*
 * 红外测试函数；
 */
void MainWindow::on_btnInfrared_clicked()
{
    ui->txtInfrared->setText("还没有加入红外！");
}

/*
 * RFID测试函数；
 */
void MainWindow::on_btnRFID_clicked()
{
    ui->txtRFID->setText("还没有加入RFID");
}

/*
 * 打开485串口,配置485串口基本属性。
 * 根据之前写过的串口及经验，需要先声明一个串口对象，然后设置打开端口号，
 * 再调用open方法，然后在成功打开函数中设置属性。
 */
void MainWindow::on_pushButton_clicked()
{
    if(!serialFlag)
    {
       bool ok=false;
       QString portName= this->ui->listPort->currentText();
       qint32 baud=this->ui->listBaud->currentText().toInt(&ok,10);

       serial->setPortName(portName);
        serial->open(QIODevice::ReadWrite);
        if(!serial->isOpen())
        {
            QString tempString="打开串口失败！请检查端口号！";
            this->ui->labelMessage->setText(tempString);
        }
        else
        {
            serialFlag=true;

            serial->setBaudRate(baud);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);

            connect(serial,SIGNAL(readyRead()),this,SLOT(slotData()));

            this->ui->labelMessage->setText("");
            this->ui->pushButton->setText("关闭485串口");
            setAllButtonEnable();
        }
    }
    else
    {
        serialFlag=false;
        serial->close();
        this->ui->labelMessage->setText("");
        this->ui->pushButton->setText("打开485串口");
        setAllButtonDisable();
    }
}

/*
 *串口接受数据，用作处理，稍后加入判断，分别判断是哪个传感器的数据，然后进行显示。
 * 这里存在将收到的hex格式数据，以hex样子显示出来。
 */
void MainWindow::slotData()
{
    QByteArray receiveData;
    receiveData=serial->readAll();
    if(receiveData!=NULL)
    {
        char tempReceiveData=receiveData.toHex().at(1);
        switch (tempReceiveData) {
            case '1':
                ui->txtGyroscope->append(receiveData.toHex());
            break;

            case '2':
                ui->txtUltrasonic->append(receiveData.toHex());
            break;

            case '3':
                ui->txtCompass->append(receiveData.toHex());
            break;

            case '4':
                ui->txtButton->append(receiveData.toHex());
            break;

            default:
            break;
        }
    }
    receiveData.clear();
}

void MainWindow::setAllButtonEnable()
{
    this->ui->btnButton->setEnabled(true);
    this->ui->btnCompass->setEnabled(true);
    this->ui->btnGyroscope->setEnabled(true);
    this->ui->btnInfrared->setEnabled(true);
    this->ui->btnRFID->setEnabled(true);
    this->ui->btnUltrasonic->setEnabled(true);

    this->ui->btnALLTime->setEnabled(true);
    this->ui->btnButTime->setEnabled(true);
    this->ui->btnComTime->setEnabled(true);
    this->ui->btnGraTime->setEnabled(true);
    this->ui->btnInfTime->setEnabled(true);
    this->ui->btnRFIDTime->setEnabled(true);
    this->ui->btnUltTime->setEnabled(true);
    this->ui->btnAllSensor->setEnabled(true);

    this->ui->editALLTime->setEnabled(true);
    this->ui->editButTime->setEnabled(true);
    this->ui->editComTime->setEnabled(true);
    this->ui->editGraTime->setEnabled(true);
    this->ui->editInfTime->setEnabled(true);
    this->ui->editRFIDTime->setEnabled(true);
    this->ui->editUltTime->setEnabled(true);


}

void MainWindow::setAllButtonDisable()
{
    this->ui->btnButton->setEnabled(false);
    this->ui->btnCompass->setEnabled(false);
    this->ui->btnGyroscope->setEnabled(false);
    this->ui->btnInfrared->setEnabled(false);
    this->ui->btnRFID->setEnabled(false);
    this->ui->btnUltrasonic->setEnabled(false);

    this->ui->btnALLTime->setEnabled(false);
    this->ui->btnButTime->setEnabled(false);
    this->ui->btnComTime->setEnabled(false);
    this->ui->btnGraTime->setEnabled(false);
    this->ui->btnInfTime->setEnabled(false);
    this->ui->btnRFIDTime->setEnabled(false);
    this->ui->btnUltTime->setEnabled(false);
    this->ui->btnAllSensor->setEnabled(false);

    this->ui->editALLTime->setEnabled(false);
    this->ui->editButTime->setEnabled(false);
    this->ui->editComTime->setEnabled(false);
    this->ui->editGraTime->setEnabled(false);
    this->ui->editInfTime->setEnabled(false);
    this->ui->editRFIDTime->setEnabled(false);
    this->ui->editUltTime->setEnabled(false);
}

void MainWindow::timeInit()
{
    stopTime="停止发送";
    startTime="自动发送";

    ultTimer=new QTimer(this);//此段作用是用一个定时器。
    connect(ultTimer,SIGNAL(timeout()),this,SLOT(on_btnUltrasonic_clicked()));
    ultFlag=false;

    graTimer=new QTimer(this);//此段作用是用一个定时器。
    connect(graTimer,SIGNAL(timeout()),this,SLOT(on_btnGyroscope_clicked()));
    graFlag=false;

    comTimer=new QTimer(this);//此段作用是用一个定时器。
    connect(comTimer,SIGNAL(timeout()),this,SLOT(on_btnCompass_clicked()));
    comFlag=false;

    butTimer=new QTimer(this);//此段作用是用一个定时器。
    connect(butTimer,SIGNAL(timeout()),this,SLOT(on_btnButton_clicked()));
    butFlag=false;

    allTimer=new QTimer(this);//此段作用是用一个定时器。
    connect(allTimer,SIGNAL(timeout()),this,SLOT(slotAllTimer()));
    allFlag=false;
}

/*
 *定时自动查询超声波
 */
void MainWindow::on_btnUltTime_clicked()
{
    if(!ultFlag)
    {
        ultFlag=true;
        this->ui->btnUltTime->setText(stopTime);

        bool ok=false;
        int timeNumber=ui->editUltTime->text().toInt(&ok,10);
        ultTimer->start(timeNumber);
    }
    else
    {
        ultFlag=false;
        ultTimer->stop();
        this->ui->btnUltTime->setText(startTime);
    }
}

void MainWindow::on_btnGraTime_clicked()
{
    if(!graFlag)
    {
        graFlag=true;
        this->ui->btnGraTime->setText(stopTime);

        bool ok=false;
        int timeNumber=ui->editGraTime->text().toInt(&ok,10);
        graTimer->start(timeNumber);
    }
    else
    {
        graFlag=false;
        graTimer->stop();
        this->ui->btnGraTime->setText(startTime);
    }
}

void MainWindow::on_btnComTime_clicked()
{
    if(!comFlag)
    {
        comFlag=true;
        this->ui->btnComTime->setText(stopTime);

        bool ok=false;
        int timeNumber=ui->editComTime->text().toInt(&ok,10);
        comTimer->start(timeNumber);
    }
    else
    {
        comFlag=false;
        comTimer->stop();
        this->ui->btnComTime->setText(startTime);
    }
}

void MainWindow::on_btnButTime_clicked()
{
    if(!butFlag)
    {
        butFlag=true;
        this->ui->btnButTime->setText(stopTime);

        bool ok=false;
        int timeNumber=ui->editButTime->text().toInt(&ok,10);
        butTimer->start(timeNumber);
    }
    else
    {
        butFlag=false;
        butTimer->stop();
        this->ui->btnButTime->setText(startTime);
    }
}

void MainWindow::on_btnInfTime_clicked()
{
    ui->btnInfTime->setText("还没加入");
}

void MainWindow::on_btnRFIDTime_clicked()
{
    ui->btnRFIDTime->setText("还没加入");
}

/*
 * 全局发送，需要定时，间隔发送。
 */
void MainWindow::slotAllTimer()
{
    serial->write(ReadGyroscope);
    serial->write(ReadUltrasonicWave);
    serial->write(ReadDcm250b);
    serial->write(ReadPS2);
}

void MainWindow::on_btnALLTime_clicked()
{
    if(!allFlag)
    {
        allFlag=true;
        this->ui->btnALLTime->setText(stopTime);

        bool ok=false;
        int timeNumber=ui->editALLTime->text().toInt(&ok,10);
        allTimer->start(timeNumber);
    }
    else
    {
        allFlag=false;
        allTimer->stop();
        this->ui->btnALLTime->setText(startTime);
    }
}


void MainWindow::on_btnAllSensor_clicked()
{
    serial->write(ReadGyroscope);
    serial->write(ReadUltrasonicWave);
    serial->write(ReadDcm250b);
    serial->write(ReadPS2);
}
