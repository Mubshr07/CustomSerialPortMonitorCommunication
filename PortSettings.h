#ifndef PORTSETTINGS_H
#define PORTSETTINGS_H

#include <QMainWindow>
#include "Classes/GlobalVars.h"

namespace Ui {
class PortSettings;
}

class PortSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit PortSettings(QWidget *parent = nullptr);
    ~PortSettings();

signals:
    void txNewSerialPortConfig(QString portName);


private slots:
    void onTimerSingleShotElapsed();
    void on_pbCancel_clicked();
    void on_pbConfiguration_clicked();


private:
    Ui::PortSettings *ui;


    QTimer *timerSingleShot;

    void processSettingsLine(QString line);


};

#endif // PORTSETTINGS_H
