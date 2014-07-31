#include "cammatroxorionhdmanager.h"


//************ singelton stuff **** START

//must be for declerate the singelton
CamMatroxOrionHDManager* CamMatroxOrionHDManager::m_instance = 0;


CamMatroxOrionHDManager::CamMatroxOrionHDManager()
{
    m_camCounter = 0;
    m_milAppID =0;
    m_milSystemID =0;
}
CamMatroxOrionHDManager* CamMatroxOrionHDManager::instance()
{

    static QMutex mutex;
    if (!m_instance)
    {
        mutex.lock();

        if (!m_instance)
            m_instance = new CamMatroxOrionHDManager();

        mutex.unlock();
    }

    return m_instance;
}

void CamMatroxOrionHDManager::drop()
{
    static QMutex mutex;
    mutex.lock();
    delete m_instance;
    m_instance = 0;
    mutex.unlock();
}
//************ singelton stuff **** END
void CamMatroxOrionHDManager::registerMilIDApp(long long id)
{
    m_milAppID = id;
}

void CamMatroxOrionHDManager::registerMilIDSystem(long long id)
{
    m_milSystemID =id;
}

void CamMatroxOrionHDManager::registerCamMatroixOrionHD()
{
    m_camCounter++;
}

//void CamMatroxOrionHDManager::unRegisterMilIDApp()
//{
//    m_milAppID = 0;
//}

//void CamMatroxOrionHDManager::unRegisterMilIDSystem()
//{
//    m_milSystemID =0;
//}


void CamMatroxOrionHDManager::freeAPPandSystem()
{
#ifdef SUPPORT_ORION_HD
    QMutex mutex;

    mutex.lock();

    m_camCounter--;

    //qDebug()<< "CamMatroxOrionHDManager::freeAPPandSystem()::counter: " << m_camCounter;

    if(m_camCounter==0)
    {
        MsysFree(m_milSystemID);
        MappFree(m_milAppID);
        m_milAppID = 0;
        m_milSystemID = 0;
    }

    mutex.unlock();

#endif
}

long long CamMatroxOrionHDManager::getMilIDApp()
{
    return m_milAppID;
}

long long CamMatroxOrionHDManager::getMilIDSystem()
{
    return m_milSystemID;
}
