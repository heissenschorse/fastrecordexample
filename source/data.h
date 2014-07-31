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


#ifndef DATA_H
#define DATA_H

//TODO: implement me
//TODO: be carefull to positions with DataStreamTypes, and Data::TYPE
//TODO make own implementation of qshareddatapaointer
//TODO: use shared pointer for the data in child classes !!
//TODO: toString behaves in a not good way problem with string concat
//TODO: refactor getAsVideo() name getAsVideoSPtr or so

//!qt
#include <QSharedData>
#include <QMetaType>
#include <QSharedPointer>



//own
//#include "frametimeobject.h"
#include "frametimeobjectproducer.h"
//#include "datasensor.h"
//#include "dataaudio.h"
//#include "datavideo.h"


// say that their will be at once such a class
class Data;
class DataSensor;
class DataVideo;
class DataAudio;

//
class FrameTimeObject;


//move to public class data
// make a usefull typedef
//typedef QSharedPointer<Data> SharedData;

//declare metatype
//Q_DECLARE_METATYPE(SharedData)

//! represent one value at one time (atomar)
//! implicit shared Data Object,
//! exchanged trough the moduls
class Data //: public QSharedData -> the QSharedPointer is the better choise
{

public:

    //typedef
    typedef QSharedPointer<Data> SPtr;
    typedef QSharedPointer<const Data> ConstSPtr;

    //! standard constructor
    Data();

    virtual ~Data();

    //TODO double type see DataStreamTypes !!  refactor use
    enum TYPE
    {
        //! everything which is order of picutre ist a VIDEO Data
        TYPE_VIDEO,
        //! everything which is a modulatet signal in the frequenz of audible speach is an AUDIO Data
        TYPE_AUDIO,
        //! everything which deliver cyclically measuredata is a SENSOR Data
        TYPE_SENSOR,
        //! never to be used, unless it's a base class
        TYPE_UNDEFINED
    };


    enum STATE
    {
        //! when construtor is called, no data were added
        STATE_CONSTRUCTED,
        //! data were added
        STATE_DATA_ADDED,
        //! data change in form and / or manner
        STATE_DATA_TRANSFORMED,
        //! when data where deleted
        STATE_DATA_CLEARED
    };

    //! return the info which data type the data container is
    virtual Data::TYPE getDataType();

    //! for debug, java like
    virtual QString toString();

   //! make a dynamic cast and pretend type safety
   //! @attention is written in datasensor.cpp !!
   static QSharedPointer<DataSensor> getAsSensor(Data::SPtr ptr);

   //! make a dynamic cast and pretend type safety
   //! @attention is written in dataaudio.cpp !!
   static QSharedPointer<DataAudio> getAsAudio(Data::SPtr ptr);

   //! make a dynamic cast and pretend type safety
   //! @attention is written in datasvideo.cpp !!
   static QSharedPointer<DataVideo> getAsVideo(Data::SPtr ptr);

   //! to String method
   QString getFrameTimeAsString();

   //! return number of frame from FrameTimeObject
   quint64 getFrameNumber();

   //! get m_frametime
   FrameTimeObject* getFrameTime();

   //! set frameTime from external
   void setFrameTime(FrameTimeObject *x);

   //! will construct a new FrameTimeObject and calc the diff of both
   FrameTimeObject* diffFrameTimeObject(FrameTimeObject *x); //TODO

protected:
    FrameTimeObject *m_frametime;
    TYPE m_dataType;
    STATE m_dataState;

};

#endif // DATA_H
