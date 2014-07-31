#include "modulesource.h"

ModuleSource::ModuleSource()
{
    m_state = STATE_UNKNOWN;
    m_dev = 0;
}

void ModuleSource::init()
{
    qDebug("ModuleSource::init() does nothing please ovveride this method");
}


void ModuleSource::run()
{
    qDebug("ModuleSource::init() does nothing please ovveride this method");
    this->exec();
}

void ModuleSource::addDevice(Device *dev)
{
    m_dev = dev;
}

Device* ModuleSource::getDevice()
{
    return m_dev;
}

Module::TYPE ModuleSource::getModuleType()
{
    return Module::SOURCE;
}


DataStreamTypes::TYPE ModuleSource::getDataStreamType()
{
    return DataStreamTypes::UNDEFINED;
}

void ModuleSource::processPendingData()
{
    qDebug("ModuleSource::processPendingData() doesnt do anything, plz OVERRIDE me");
}

void ModuleSource::setState(STATE state)
{
       m_state = state;
}

ModuleSource::STATE ModuleSource::getState()
{
    return m_state;
}

void ModuleSource::sendYourProcessedData()
{
    qDebug("ModuleSource::sendYourProcessedData() doesnt do anything, plz OVERRIDE me");
}

void ModuleSource::finishYourProcessing()
{
    qDebug("ModuleSource::finishYourProcessing() doesnt do anything, plz OVERRIDE me");
}

