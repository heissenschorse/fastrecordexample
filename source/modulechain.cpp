#include "modulechain.h"

ModuleChain::ModuleChain()
{
    m_timer = 0;
}

Module* ModuleChain::getModuleAt(int index)
{
    //check the bound
    if(index<0||index>=m_modList.size())
        return 0;
    //return module
    return m_modList.at(index);

}

bool ModuleChain::appendModulToSource( ModuleDisplay *modDispla)
{
    bool success = false;

    //get source
    ModuleSource *source = getSourceModule();

    if(source==0)
    {
        qWarning()<< "ModuleChain::appendModulToSource warning: NO ModulSource found";
        return false;
    }
    //connect module
    //connect the signals, make a chain
    // source -> display
    success = QObject::connect(source, SIGNAL(sendData(DataFrame::SPtr)),
                            modDispla, SLOT(receiveData(DataFrame::SPtr)));
    //check
    if(!success)qWarning("!! error ModuleChain::appendModulToSource error while connect the new moduls");

    //add to chain
    appendModule(modDispla);

    return success;
}

void ModuleChain::toString()
{
    //qDebug("ModuleChain of the Type: %i",m_chainType);
    for(int i=0;i<m_modList.size();i++)
    {
        qDebug()<< "ModuleChain::toString() list["<<i<<"] Module::Type::"<< m_modList.at(i)->getModuleType() << " DataStream::Type::" <<  m_modList.at(i)->getDataStreamType() ;
    }

}


void ModuleChain::appendModule(Module *m)
{
    m_modList.append(m);
}


void ModuleChain::startModuleChain()
{
        //start all, which is a thread
        //for timer intervall @see modulchainfactory

        if(m_timer!=0)
            m_timer->start();
        else
        {
            qWarning("ModuleChain::startModuleChain() error while start source, no timer is added for chain, will abort");
            return;
        }

        //start source
        ModuleSource *modSource = getSourceModule();
        modSource->start();

}


void ModuleChain::stoppModulChain()
{
    //call halt
    haltModulChain();

    //start source
    ModuleSource *modSource = getSourceModule();
    //here call halt
    modSource->finishYourProcessing();
}


void ModuleChain::haltModulChain()
{
    if(m_timer!=0)
        m_timer->stop();
    else
        qWarning("ModuleChain::startModuleChain() error while stop source, no timer is added for chain");

}

ModuleSource* ModuleChain::getSourceModule()
{
    //chech the type
    if(m_modList.at(0)->getModuleType() != Module::SOURCE)
    {
        return 0;
    }

    return (ModuleSource*) m_modList.at(0);
}


ModuleLogger* ModuleChain::getModuleLogger()
{
    for( int i=0; i<m_modList.size();i++)
    {
        if(m_modList.at(i)->getModuleType()==Module::LOGGER)
        {

            return (ModuleLogger*) m_modList.at(i);
        }
    }

    return 0;
}

ModuleDisplay* ModuleChain::getModulDisplay()
{
    for( int i=0; i<m_modList.size();i++)
    {
        if(m_modList.at(i)->getModuleType()==Module::DISPLAY)
        {
            return (ModuleDisplay*) m_modList.at(i);
        }
    }

    return 0;
}


int ModuleChain::getIndexOfModul(Module *mod)
{
    for( int i=0; i<m_modList.size();i++)
    {
        if(m_modList.at(i)== mod)
        {
            return i;
        }
    }

    return -1;
}


void ModuleChain::addTimerReference(QTimer *timer)
{
    m_timer = timer;
}
