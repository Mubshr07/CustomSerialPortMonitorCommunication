#include "AddNewPacket.h"
#include "ui_AddNewPacket.h"

AddNewPacket::AddNewPacket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewPacket)
{
    ui->setupUi(this);
}

AddNewPacket::~AddNewPacket()
{
    delete ui;
}

void AddNewPacket::on_pbClose_clicked()
{
    this->close();
}

