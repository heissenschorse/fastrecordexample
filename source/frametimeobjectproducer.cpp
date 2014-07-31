#include "frametimeobjectproducer.h"

//declare static memembers ************

//must be for declerate the singelton
FrameTimeObjectProducer* FrameTimeObjectProducer::m_instance = 0;
//
quint64 FrameTimeObjectProducer::m_frameCounter = 0;


//methods*************************************
FrameTimeObjectProducer* FrameTimeObjectProducer::instance()
{

    static QMutex mutex;
    if (!m_instance)
    {
        mutex.lock();

        if (!m_instance)
            m_instance = new FrameTimeObjectProducer();

        mutex.unlock();
    }

    return m_instance;
}

FrameTimeObject* FrameTimeObjectProducer::fromQString(QString dateStr)
{
    FrameTimeObject *fto = new FrameTimeObject();
    QDateTime date = QDateTime::fromString(dateStr,"dd.MM.yyyy");

    if(!date.isValid())
    {
        qWarning()<< "FrameTimeObjectProducer::fromQString invalid date pattern:" <<dateStr;
        return 0; //skip if wrong format
    }
    fto->setTimeUnix(date.toTime_t());
    fto->setTimeMs(0);

    return fto;
}

void FrameTimeObjectProducer::drop()
{
    static QMutex mutex;
    mutex.lock();
    delete m_instance;
    m_instance = 0;
    mutex.unlock();
}

FrameTimeObject* FrameTimeObjectProducer::getFrameTimeObject()
{
    FrameTimeObject *foo =  new FrameTimeObject();

    //lock to be safe
//    static QMutex mutex;
//    mutex.lock();
//    //set frame number
//    foo->setFrameNumber(m_frameCounter);

//    m_frameCounter = m_frameCounter +1 ;
//    //qDebug()<< "FrameTimeObjectProducer::getFrameTimeObject: frameCounter" << m_frameCounter;

//    mutex.unlock();

    return foo;
}

FrameTimeObjectProducer::FrameTimeObjectProducer()
{
    //m_frameCounter = 0;
}
