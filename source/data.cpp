#include "data.h"

Data::Data()
{
    //called from every child class
    m_frametime = FrameTimeObjectProducer::instance()->getFrameTimeObject();
    m_dataType = TYPE_UNDEFINED;
}

Data::~Data()
{

}

Data::TYPE Data::getDataType()
{
    return m_dataType;
}

QString Data::toString()
{
    QString string = QString("Data with Type %1 recorded at ").arg(getDataType());

    //append time
    string.append(m_frametime->toString());

    return string;
}

quint64 Data::getFrameNumber()
{
     qDebug()<< " Data::getFrameNumber:" << m_frametime->getFrameNumber();

     return m_frametime->getFrameNumber();
}

QString Data::getFrameTimeAsString()
{
    return m_frametime->toString();
}

FrameTimeObject* Data::getFrameTime()
{
    return m_frametime;
}

void Data::setFrameTime(FrameTimeObject *x)
{
    m_frametime = x;
}


//DataAudio::SPtr Data::getAsAudio(Data::SPtr ptr)
//{
//    switch(ptr->getDataType())
//    {
//        case AUDIO:
//                return  ptr.dynamicCast<DataAudio>();
//         default:
//            qWarning("Data::getAsAudio type mismatch during cast");
//              break;
//    }

//    return 0;
//}

//DataVideo::SPtr Data::getAsVideo(Data::SPtr ptr)
//{
//    switch(ptr->getDataType())
//    {
//        case VIDEO:
//                return  ptr.dynamicCast<DataVideo>();
//         default:
//            qWarning("Data::getAsVideo type mismatch during cast");
//              break;
//    }

//    return 0;
//}
