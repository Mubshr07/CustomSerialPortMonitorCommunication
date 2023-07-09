#include "PortSettings.h"
#include "ui_PortSettings.h"

PortSettings::PortSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PortSettings)
{
    ui->setupUi(this);
}

PortSettings::~PortSettings()
{
    delete ui;
}
