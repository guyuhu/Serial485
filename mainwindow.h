#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QStringList>
#include <QTimer>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        void setAllButtonEnable();//将串口打开和关闭应该有的按钮使能和取消效果。
        void setAllButtonDisable();//

        void dataInit();  //将视图列表应有的初始化数据添加到上面，其次是填充传感器命令。

        void timeInit();//各种传感器，自动发送效果。将所有定时器放到一起进行初始化。

    private slots:
        void on_btnUltrasonic_clicked();//以下几个查询各个传感器按钮的单击事件。包含视图变化效果，和发送命令信号。

        void on_btnGyroscope_clicked();

        void on_btnCompass_clicked();

        void on_btnButton_clicked();

        void on_btnInfrared_clicked();

        void on_btnRFID_clicked();

        void on_pushButton_clicked();

        void slotData();//串口接收数据，判断接收信号的种类，然后进行显示。

        void on_btnUltTime_clicked();//以下几个分别为不同传感器的定时发送按钮效果。启动定时器，发送命令。
        void on_btnGraTime_clicked();
        void on_btnComTime_clicked();
        void on_btnButTime_clicked();
        void on_btnInfTime_clicked();
        void on_btnRFIDTime_clicked();
        void slotAllTimer();

        void on_btnALLTime_clicked();

        void on_pushButton_2_clicked();

        void on_btnAllSensor_clicked();

    private:
        Ui::MainWindow *ui;

        QSerialPort *serial;
        bool serialFlag;//用作判断串口是否开启

        QByteArray CCStartDcm250b;//DCM250B命令
        QByteArray CCDcm250b_Calibration;//01
        QByteArray CCDcm250b_Save;//02
        QByteArray ReadDcm250b;//读取Dcm250B数据命令

        QByteArray ReadGyroscope;//读取Gyroscope数据命令
        QByteArray ReadUltrasonicWave;//读取UltrasonicWave数据命令
        QByteArray ReadPS2;//读取PS2数据命令

        bool ultFlag;//超声波定时发送
        bool graFlag;//陀螺仪定时发送
        bool comFlag;//罗盘
        bool butFlag;//键盘

        QTimer *ultTimer;
        QTimer *graTimer;
        QTimer *comTimer;
        QTimer *butTimer;

        QTimer *allTimer;
        bool allFlag;//全局

        QString stopTime;
        QString startTime;

};

#endif // MAINWINDOW_H
