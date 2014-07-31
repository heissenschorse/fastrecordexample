#include "modulelogger.h"

void ModuleLogger::receiveData(DataFrame::SPtr d)
{
    Q_UNUSED(d);

    qDebug("ModuleLogger::receiveData doens't do anything, plz implement me");
}


Module::TYPE ModuleLogger::getModuleType()
{
    return Module::LOGGER;
}

DataStreamTypes::TYPE ModuleLogger::getDataStreamType()
{
    return DataStreamTypes::UNDEFINED;
}


void ModuleLogger::startLogging()
{
    qDebug("ModuleLogger::startLogging doens't do anything, plz implement me");
}

void ModuleLogger::stoppLogging()
{
    qDebug("ModuleLogger::stoppLogging doens't do anything, plz implement me");
}

