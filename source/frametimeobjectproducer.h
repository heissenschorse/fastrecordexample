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

#ifndef FRAMETIMEOBJECTPRODUCER_H
#define FRAMETIMEOBJECTPRODUCER_H

//TODO: perhaps every produced should be counted and managed
//TODO: rename: FrameTime is not right, every atomar data has one moment of time
//              not every frame


//qt
#include <QMutex>
#include <QDateTime>

//own
#include "frametimeobject.h"

class FrameTimeObject;

//! singelton to produce
//! FrameTimeObject's
//! is thread safe
class FrameTimeObjectProducer
{

public:

    //! function to get the instance
    static FrameTimeObjectProducer* instance();

    //! construct fto object from date patter "dd.MM.yyyy"
    static FrameTimeObject* fromQString(QString date);

    //! function to destroy the instance
    static void drop();

    //! return a FrameTimeObject instanced
    //! @warning must be freed at one time
    static FrameTimeObject* getFrameTimeObject();

private:

    //! the one instance witch can exist
    static FrameTimeObjectProducer *m_instance;

    //! private
    FrameTimeObjectProducer();

    //! hide copy constructor
    FrameTimeObjectProducer(const FrameTimeObjectProducer&);

    //! override the copy constructor
    FrameTimeObjectProducer& operator=(const FrameTimeObjectProducer &); // hide assign op
                         // we leave just the declarations, so the compiler will warn us
                         // if we try to use those two functions by accident

    //! counter of frames
    static quint64 m_frameCounter;

//    //! is an complete list of all frametimeobjects which where poduced
//    //TODO better use timeline !
//    QList<FrameTimeObject::SPtr> m_frmtList;
};

#endif // FRAMETIMEOBJECTPRODUCER_H
