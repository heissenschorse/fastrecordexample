#include "moduledisplay.h"

ModuleDisplay::ModuleDisplay()
{
}


ModuleDisplay::ModuleDisplay(QWidget *parent) : QWidget( parent )
{
}


void ModuleDisplay::receiveData(DataFrame::SPtr d)
{
    Q_UNUSED(d);

    qDebug("ModuleDisplay::receiveData doens't do anything, plz override me");
}


Module::TYPE ModuleDisplay::getModuleType()
{
    return Module::DISPLAY;
}


DataStreamTypes::TYPE ModuleDisplay::getDataStreamType()
{
    return DataStreamTypes::UNDEFINED;
}


void ModuleDisplay::init()
{
     qDebug("ModuleDisplay::receiveData doens't do anything, plz override me");
}
