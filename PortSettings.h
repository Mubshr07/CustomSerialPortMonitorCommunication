#ifndef PORTSETTINGS_H
#define PORTSETTINGS_H

#include <QMainWindow>

namespace Ui {
class PortSettings;
}

class PortSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit PortSettings(QWidget *parent = nullptr);
    ~PortSettings();

private:
    Ui::PortSettings *ui;
};

#endif // PORTSETTINGS_H
