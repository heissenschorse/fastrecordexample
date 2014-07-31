#include "frametimeobject.h"



FrameTimeObject::FrameTimeObject()
{
   setToCurrentTime();
   // m_datRef = 0;
}

FrameTimeObject::FrameTimeObject(quint64 time, quint64 ms, quint64 framenumber)
    :m_frame_nr(framenumber),
     m_time(time),
     m_time_ms(ms)
{

   // m_datRef = 0;
}

//void FrameTimeObject::dependOn(Data::SPtr ref)
//{
//    m_dataRef = ref;
//}


QDateTime FrameTimeObject::toDateTime()
{
    QDateTime date;
    date.setTime_t(m_time);
    date.addMSecs(m_time_ms);
    return date;
}

void FrameTimeObject::setToCurrentTime()
{

    //m_date_time_object = new QDateTime(QDate.currentDate(),QTime.currentTime(),Qt::LocalTime);

    //this way is correct, because it cost lower than 1 millisecond to call
    //QTime::currentTime();
    //and  QDateTime::currentDateTime();

    //construct the time
//    QTime myTimer;

//    myTimer.start();
    const QTime thisTime = QTime::currentTime();
    //qDebug()<< "FrameTimeObject::setToCurrentTime() time for call QTime::currentTime(); "<< myTimer.elapsed();

//    myTimer.start();
    const QDateTime thisDateTime =  QDateTime::currentDateTime();
//    qDebug()<< "FrameTimeObject::setToCurrentTime() time for call QDateTime::currentDateTime(); "<< myTimer.elapsed();
    //qDebug()<< "FrameTimeObject::currentDateTime() time for call "<<thisDateTime.toString("dd.MM.yyyy") << " seconds" <<  thisDateTime.toTime_t();

    //test the timer
//    double foo=1;
//    myTimer.start();
//    for(int i;i<100000;i++)
//        foo = foo*2 +foo;
//    qDebug()<< "FrameTimeObject::setToCurrentTime() test timer "<< myTimer.elapsed();


    //copy to this
    m_time = thisDateTime.toTime_t();
    m_time_ms = thisTime.msec();

    //no frame number ist given
    m_frame_nr = 0;


}


int FrameTimeObject::compareTo(FrameTimeObject *obj)
{

    //when unix time equal compare the
    if(obj->getTimeUnix()==this->getTimeUnix())
    {
        //ms compare
        return (obj->getTimeMs()-this->getTimeMs());

    }else //compare unixtimestamp
        return (obj->getTimeUnix()-this->getTimeUnix());

}


void FrameTimeObject::subtractSeconds(quint64 amountSeconds)
{
    if(m_time-amountSeconds>=0)
    {
        m_time = m_time - amountSeconds;
    }
    else qWarning()<<"FrameTimeObject::subtractSeconds error";
}

void FrameTimeObject::reset()
{
    m_frame_nr = 0;
    m_time = 0;
    m_time_ms = 0;
}

bool FrameTimeObject::secondsAreEqual(FrameTimeObject *x)
{
    if(m_time == x->getTimeUnix())
        return true;

    return false;
}

bool FrameTimeObject::isEarlierAs(FrameTimeObject *obj)
{
    //qDebug("FrameTimeObject::isEarlierAs debug: ");

    //qDebug()<< "FrameTimeObject::isEarlierAs compare "<< this->toString() << "  to " << obj->toString();
    //qDebug()<< "FrameTimeObject::isEarlierAs compareTo "<<this->compareTo(obj);

    if(this->compareTo(obj)>0)
        return true;

    return false;
}

bool FrameTimeObject::isZero()
{
    if(m_frame_nr&&m_time==0&&m_time_ms==0)
        return true;

    return false;
}

quint64 FrameTimeObject::getTimeMs()
{
    return m_time_ms;
}

quint64 FrameTimeObject::getTimeUnix()
{
    return m_time;
}

quint64 FrameTimeObject::getFrameNumber()
{
    // qDebug()<< "FrameTimeObject::getFrameNumber :" << m_frame_nr;
    return m_frame_nr;
}


void FrameTimeObject::setFrameNumber(quint64 framenum)
{
    //qDebug()<< "FrameTimeObject::setFrameNumber :" << framenum;
    m_frame_nr = framenum;

}

void FrameTimeObject::setTimeMs(quint64 ms)
{
    m_time_ms = ms;
}

void FrameTimeObject::setTimeUnix(quint64 seconds)
{
    m_time = seconds;
}

QString FrameTimeObject::toString()
{
    QString str;
    str = QString("%1;%2;%3")
            .arg(m_time)
            .arg(m_time_ms)
            .arg(m_frame_nr);

    return str;
}

QString FrameTimeObject::toStringTimeUnix()
{
    //qDebug()<< "FrameTimeObject::toStringTimeUnix(), value" <<m_time << " string" <<  QString("%1").arg(m_time);
    return QString("%1").arg(m_time);
}

QString FrameTimeObject::toStringTimeMS()
{
    return QString("%1").arg(m_time_ms);
}

QString FrameTimeObject::toStringFramenumber()
{
    return QString("%1").arg(m_frame_nr);
}

QString FrameTimeObject::toStringReadable()
{
    QString str;
    str = QString("FTO: %1 s %2 ms %3 framenumber\n")
            .arg(m_time)
            .arg(m_time_ms)
            .arg(m_frame_nr);

    return str;
}


QString FrameTimeObject::toStringHumanReadableTime()
{
    QString str;

    //convert
    QDateTime time = QDateTime::fromTime_t(m_time);

    //append
    str.append(time.toString("hh:mm:ss"));

    //qDebug()<< " FrameTimeObject::toStringHumanReadableTime() value("<< m_time << ") string("<<str<<")";

    return str;
}

QString FrameTimeObject::toStringHumanReadableDate()
{
    QString str;

    //convert
    QDateTime time = QDateTime::fromTime_t(m_time);

    //append
    str.append(time.toString("dd.MM.yy"));

    //qDebug()<< " FrameTimeObject::toStringHumanReadableDate() value("<< m_time << ") string("<<str<<")";

    return str;
}
//******************* for test  here is the problem
void FrameTimeObject::addRefToData(Data *datRef)
{
    m_datRef = QSharedPointer<Data>(datRef);
}


void FrameTimeObject::addRefToData(QSharedPointer<Data> datRef)
{
    m_datRef = datRef;
}


QSharedPointer<Data> FrameTimeObject::getRefToData()
{
    return m_datRef;
}
