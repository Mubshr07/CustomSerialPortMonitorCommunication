QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT       += serialport

CONFIG += c++17

TARGET = CustomSerialPortPackets
TEMPLATE = app


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AddNewPacket.cpp \
    Classes/GlobalVars.cpp \
    PortSettings.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    AddNewPacket.h \
    Classes/GlobalVars.h \
    PortSettings.h \
    mainwindow.h

FORMS += \
    AddNewPacket.ui \
    PortSettings.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
