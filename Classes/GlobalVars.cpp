#include "GlobalVars.h"


ACTORtype GlobalVars::ACTORis = ActorUSER;

QString GlobalVars::filePathCOMportSettings = "";

GlobalVars::GlobalVars() { }


void GlobalVars::initEveryThing()
{
    filePathCOMportSettings = QString(QDir::homePath()+"/serialSettings");


}
