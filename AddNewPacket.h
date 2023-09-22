#ifndef ADDNEWPACKET_H
#define ADDNEWPACKET_H

#include <QDialog>

#include "Classes/GlobalVars.h"

namespace Ui {
class AddNewPacket;
}

class AddNewPacket : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewPacket(QWidget *parent = nullptr);
    ~AddNewPacket();

private slots:
    void on_pbClose_clicked();

private:
    Ui::AddNewPacket *ui;
};

#endif // ADDNEWPACKET_H
