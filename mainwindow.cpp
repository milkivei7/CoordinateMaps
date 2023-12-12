#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QStringView>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->quickWidget_MapView->setSource(QUrl(QStringLiteral("qrc:/QmlMap.qml")));
    ui->quickWidget_MapView->show();

    rex.setPattern("\\r\\n|\\r|\\n");
    updateComPorts();



    auto Obj = ui->quickWidget_MapView->rootObject();
    connect(this, SIGNAL(signalSetLocationMarker(QVariant,QVariant)),  Obj, SLOT(setLocationMarker(QVariant,QVariant)));
    connect(this, SIGNAL(signalSetCenterPosition(QVariant,QVariant)),  Obj, SLOT(setCenterPosition(QVariant,QVariant)));
    connect(ui->ButtConnect,  &QPushButton::clicked, this, &MainWindow::slotConnectPort);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateComPorts()
{
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &portInfo : serialPortInfos)
    {
        qDebug()<< "Port:" << portInfo.portName() << "\n";

        //ui->->addItem(portInfo.portName());
        ui->PortsComboBox->addItem(portInfo.portName());
    }

    if(ui->PortsComboBox->currentText().isEmpty())
        qWarning()<<"\n-----COM PORT IS EMPTY!----\n";


    qDebug()<<"\n-importPorts - Complete"
           << "\n-------------\n\n\n";
}

void MainWindow::slotConnectPort()
{

    qDebug()<<"-------------"
            <<"\n-slotConnectComPort";

    // ---Settings SerialPort
    serial.setPort(QSerialPortInfo(ui->PortsComboBox->currentText())); //Set name Port
    serial.setBaudRate(QSerialPort::Baud115200); // Set speed data
    serial.open(QIODevice::ReadWrite); // Open Port
    if(!serial.isOpen())
    {
         QMessageBox::warning(this, "Error Connect", "Error connect to COM Port" + serial.errorString(),QMessageBox::Ok);
         qWarning()<<"\nqWarning(): "<<"Error connect to COM Port:\n"<<serial.errorString();
         return;
    }
    connect(&serial, &QSerialPort::readyRead, this, &MainWindow::slotGetDataPort);

    ui->PortsComboBox->setEnabled(false);
    //ui->ButtConnect->setText(tr("Disconnect"));
    qDebug()<<"\n+Complete"
           <<"\n-------------\n\n\n";
}

void MainWindow::slotGetDataPort()
{
    qDebug()<<"-------------"
            <<"\n-slotGetDataFromCom";
    if(serial.canReadLine())
    {
    QByteArray data = serial.readLine(512);
    QString linePort = QString::fromStdString(data.toStdString());
    linePort = linePort.remove(rex);
    //qDebug()<<line;
    QStringList listLine = linePort.split(']');
    for(int i = 0; i<listLine.count(); i++)
    {
        if(listLine[i]== "")
            listLine.removeAt(i);
        else
        {
            QString line =  listLine[i];
            listLine[i] = listLine[i]+"]";
            //qDebug()<<listLine[i];
            if(line.startsWith(MainWindow::SatelitePrefix)){
                int i = line.indexOf(":");//Разделитель строки на подстроки
                if (i > 0)
                {
                    //qDebug()<<"SatelitePrefix "<< QString(line.mid(i + 1, line.length()));
                    countSatelite = QString(line.mid(i + 1, line.length())).toInt();
                }
            }
            else if(line.startsWith(MainWindow::SpeedPrefix))
            {
                int i = line.indexOf(":");//Разделитель строки на подстроки
                if (i > 0)
                {
                    //qDebug()<<"Speed "<< QString(line.mid(i + 1, line.length()));
                    speed = QString(line.mid(i + 1, line.length())).toInt();
                }
            }
            else if(line.startsWith(MainWindow::LatitudePrefix))
            {
                int i = line.indexOf(":");//Разделитель строки на подстроки
                if (i > 0)
                {
                    //qDebug()<<"Latitude "<< QString(line.mid(i + 1, line.length()));
                    latitude = QString(line.mid(i + 1, line.length())).toFloat();
                }
            }
            else if (line.startsWith(MainWindow::LongitudePrefix))
            {
                int i = line.indexOf(":");//Разделитель строки на подстроки
                if (i > 0)
                {
                    //qDebug()<<"Longitude "<< QString(line.mid(i + 1, line.length()));
                    longitude = QStringView(line.mid(i + 1, line.length())).toFloat();
                }
            }

        }

    }
    if(longitude!= 0.0f && latitude!= 0.0f)
    {
        emit signalSetLocationMarker(latitude, longitude);
        emit signalSetCenterPosition(latitude, longitude);

    }
    qDebug()<<speed<<" "<<longitude<<" "<<latitude<<" "<<countSatelite;


    }

}

