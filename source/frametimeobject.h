/*

The MIT License (MIT)

Copyright (c) 2014 Thomas Boy, HTWK Leipzig, university of applied sciences,
European Social Fund (ESF) prototed the project: TOPINUS

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifndef FRAMETIMEOBJECT_H
#define FRAMETIMEOBJECT_H

//TODO: rename FrameTimeObject, because relation frame <-> data <-> time changed !
// TODO: test the accuracy on the system
//TODO: not work    m_time = thisDateTime.toSecSinc...(); in cpp, line 51 not work, because of  qt 4.6 version

// qt
#include <QTime>
#include <QDateTime>
#include <QDate>
#include <QDebug>
#include <QSharedPointer>

//own
class Data; //for cross include

//! the class represents a moment of time
//! it is also including the framenumber of a recorded picture
class FrameTimeObject
{

public:


    //! type def for shared pointer
    typedef QSharedPointer<FrameTimeObject> SPtr;

    //! contruct the Object
    FrameTimeObject(quint64 time,quint64 ms, quint64 framenumber);

    //! contruct the Object
    //! will set the Object time to Creation Date
    FrameTimeObject();

    //! compare the FrameTimeObject time with this
    //! lower means earlier
    //! @return  this < obj -> return negativ
    //! @return  this == obj -> return 0
    //! @return  this > obj -> return positiv
    //! < = > are over a time line
    int  compareTo(FrameTimeObject *obj);

    //! @return true if x->time == this->time
    bool secondsAreEqual(FrameTimeObject *x);


    //! compare to a FrameTimeObject, which is earliear
    //! @return return true when this is earliear
    bool isEarlierAs(FrameTimeObject *obj);

    //! reset to zero
    void reset();

    //! set this to the current time
    void setToCurrentTime();

    //! perform a zero check
    bool isZero();

    //! convert to DateTime
    QDateTime toDateTime();

    //! get as writable String
    QString toString();

    //! get seconds sice epoche as string
    QString toStringTimeUnix();

    //! get ms as string
    QString toStringTimeMS();

    //! get ms as string
    QString toStringFramenumber();

    //! get as human readabel String. all members
    QString toStringReadable();

    //! get as human readabel String, just the time
    QString toStringHumanReadableTime();

    //! get as human readabel String, just the date
    QString toStringHumanReadableDate();


    //! add dataReference , TEST case sensor data
    void addRefToData(Data *datRef);
    void addRefToData(QSharedPointer<Data> datRef);

    //! get dataReference , TEST case sensor data
    QSharedPointer<Data> getRefToData();


//    //! declare that this frame depend to ref Data
//    void dependOn(Data::SPtr ref);

    //get set
    //! get
    quint64 getTimeUnix();
    //! get
    quint64 getTimeMs();
    //! get
    quint64 getFrameNumber();
    //! set
    void setTimeUnix(quint64 seconds);
    //! set
    void setTimeMs(quint64 ms);
    //! set
    void setFrameNumber(quint64 framenum);

    //! substract seconds
    void subtractSeconds( quint64 amountSeconds);

private:

    //! current Framenumber
    quint64 m_frame_nr; // video ref

    //! current time,
    //! a unix timestamp
    //! seconds since epoch
    quint64 m_time;

    //! milli seconds
    quint64 m_time_ms;

//    //! an ref to data object
//    Data::SPtr m_dataRef;

    ////! the Qtime object
    //QDateTime *m_date_time_object;

    //****TEST
    QSharedPointer<Data> m_datRef;

    //methods
   };

#endif // FRAMETIMEOBJECT_H
