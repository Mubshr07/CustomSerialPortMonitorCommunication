#ifndef GLOBALVARS_H
#define GLOBALVARS_H



enum ACTORtype{
    ActorUSER,
    ActorAdministrator,
    ActorDebugger
};

enum GUImodules{
    guiNull,
    guiPortSettings,
    guiMessageBox,

};


#include <QObject>
#include <QFile>
#include <QIODevice>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QElapsedTimer>
#include <qmath.h>
#include <QDateTime>
#include <QKeyEvent>
#include <QSerialPort>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QIcon>
#include <QFileDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>

#include <QTextDocument>


#include <qdebug.h>

#define guiWidth 1024
#define guiHeight 768
#define guiDelay 200




#define COMPORTname "COM1"



class GlobalVars
{
public:
    GlobalVars();
    static void initEveryThing();


    static ACTORtype ACTORis;
    static QString filePathCOMportSettings;





};

#endif // GLOBALVARS_H
