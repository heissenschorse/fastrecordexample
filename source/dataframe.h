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


#ifndef DATAFRAME_H
#define DATAFRAME_H

//TODO refactor FrameTimeObject* getStartFrameTime();
//   that not on every add Data the frameTime will change !

//qt
#include <QQueue>
#include <QSharedPointer>
#include <QDebug>

//own
#include "data.h"
#include "datavideo.h"
#include "datastreamtypes.h"

//! is an set of Data
//! in a range of time
//! contains all methods to handle the saved data in frame
class DataFrame
{
public:

    //typedef
    typedef QSharedPointer<DataFrame> SPtr;
    typedef QSharedPointer<const DataFrame> ConstSPtr;

    //! construct dataFrame of choose Data::TYPE
    DataFrame(Data::TYPE type);

//    ~DataFrame();

    //! add data to queue, only add one type of data !
    //! else error will occure
    void addData(Data *dat); //only use Data, not DataSPtr, because DataFrame should be a DataFrame::SPTr !

    //!
    void addData(Data::SPtr dat);


//    //! add dataStream type
//    void addDataStreamType(DataStreamTypes type);

    //! return length of Queue
    int getSize();

    //! return queue element as sharedpointer
    //! is type safe
    DataVideo::SPtr getQueueElementAsVideoAt(int at);

    //! return queue element as sharedpointer
    //! more generell
    Data::SPtr getQueueElementAt(int at);

    //! return true if no packet in frame
    bool isEmpty();

    //! return  m_frametimeStart,
    //! on error return 0
    FrameTimeObject* getStartFrameTime();

private:

    //! warteschlange daten
    QQueue<Data::SPtr> m_dataQueue;

    //TODO choose one of these ! !!
    Data::TYPE m_dataTyp;
    DataStreamTypes m_dataStreamType;

    //! start time of frame
    FrameTimeObject *m_frametimeStart;

    //! end time of frame
    FrameTimeObject *m_frametimeEnd; // !!!! not written at the moment

    //! check the type
    //! return true if type is same like m_dataTyp
    bool isTypeSafe(Data *dat);

    //! check length of queue
    //! return true, if is not over size
    bool isNOTOverSizeOfQueue(int at);

};

#endif // DATAFRAME_H
