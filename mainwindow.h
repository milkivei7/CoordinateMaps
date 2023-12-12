#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGeoAddress>
#include <QGeoCoordinate>

#include <QtQuick>
#include <QtCore>
#include <QtGui>

#include <QGeoLocation>
#include <QGeoServiceProvider>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void signalSetCenterPosition(QVariant, QVariant);
    void signalSetLocationMarker(QVariant, QVariant);
private:
    Ui::MainWindow *ui;
    QSerialPort serial;
    QRegularExpression rex;


    // Preffix
    QString SpeedPrefix     = "[Speed:";
    QString SatelitePrefix  = "[Satelite:";
    QString LatitudePrefix  = "[Latitude:";
    QString LongitudePrefix = "[Longitude:";


    // Parameters
private:
    int speed;
    float latitude, longitude;
    int countSatelite;


private:
   void updateComPorts();
private slots:
   void slotConnectPort();
   void slotGetDataPort();

};
#endif // MAINWINDOW_H
