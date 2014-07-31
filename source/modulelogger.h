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


#ifndef MODULELOGGER_H
#define MODULELOGGER_H

//!own
#include "module.h"
#include "dataframe.h"

//! base class for all datastream logger
//! logger moduls are the end in working order (data processing order, they send no data
class ModuleLogger :  public QObject, public Module //first QObject then the rest @see http://doc.qt.digia.com/4.5/moc.html#multiple-inheritance-requires-qobject-to-be-first
{

public:

    enum STATE
    {
        //! state, that nothing is recording Type, Device Layer abstraction Type
        STOPPED,
        //! state that the module is recording
        RECORDING,
        //!  state when an complicated error occurs, something like error on init encoder
        UNDEFINED,
        INITIALIZED
    };

    //! @see Module::getModuleType();
    Module::TYPE getModuleType();

    //! get the info, datastream type is used @see DataStreamTypes
    virtual DataStreamTypes::TYPE getDataStreamType();


public slots:
    //! modul slot to receive Data , each Modul must have this !
    void receiveData(DataFrame::SPtr d);

    //! start writing Data
    //! @warning must be implement from every child !, state must be changed !
    void startLogging();

    //! stopp writing Data
    //! @warning must be implement from every child !  state must be changed !
    void stoppLogging();


protected:

    STATE m_state;

};

#endif // MODULELOGGER_H
