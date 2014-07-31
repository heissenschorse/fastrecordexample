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


#ifndef MODULESOURCE_H
#define MODULESOURCE_H

//TODO: ensure that every child use them enum ModuleSource to inform, that errors
//      are occur
//TODO: perhaps a Type enum should be used to inform the child, what kind of child is is
//TODO: rename the recordStart recordStopp slots becaus it isn't recording that moment, either grabbing
//TODO: eliminate different kinds of state on device, on modulesource

//qt
#include <QThread>
#include <QDebug>
#include <QSharedDataPointer>
#include <QObject>

//own
#include "module.h"
#include "data.h"
#include "device.h"
#include "dataframe.h"

//! base class for all source, which could deliver data
//! for a new source, reimplement this
//! is an abstraction layer between hardware and framework
//! e.p. audio recording device and software
//! every child is a thread !
//! all childs are event driven objects, don't make anything in run() than call exec(); !!
class ModuleSource : public QThread, public Module
{
    //Q_OBJECT //not used

public:

    //! def a list
    typedef QList<ModuleSource*> list;

    //! defines the Error which all Child Sources
    //! can have
    enum MODERROR
    {
        //! error during init step
        ErrorWhileInitDevice,
        //! error init call failed
        ErrorInitFailed,
        //! should be use when device is not reachabel
        ErrorDeviceIsNotReachable,
        //! error while initializing the device
        ErrorWhileOpenDevice,
        //! error is used, when the device is not able to deliver data
        ErrorReceiveNoData,
        //! when an unexpected error is caused
        ErrorUndefined

    };

    //! differt internal basic states, for more details use Device::State
    enum STATE
    {
        //! state no known
        STATE_UNKNOWN,
        //! state init was called, and run without problems
        STATE_INITIALIZED,
        //! state when thread is state run and timer call processPending data periodicly and receive data
        STATE_PROCESSING,
        //! state after running when the modul is stopped, no processing is done
        STATE_STOPPED
    };


    explicit ModuleSource();

    //! set internal state
    void setState(STATE state);

    //! get internal state
    STATE getState();

    //! function to measure the receive frequenz
    //! please reimplement in child classes
    quint64 getTheReceiveFrequenzInHz();

    //! @see Module::getModuleType();
    Module::TYPE getModuleType();

    //! get the info, datastream type is used @see DataStreamTypes
    virtual DataStreamTypes::TYPE getDataStreamType();

    //! add the device to the source
    void addDevice(Device *dev);

    //! return ref to Device
    Device* getDevice();

    //! the function to stopp the source, and to free every ressource
    //! quit the thread in addition
    virtual void finishYourProcessing();

protected:

    //! the run function from QThread
    //! @see QThread
    virtual void run();

    //! make all init call in this function
    virtual void init();

    //! every child must process the data in this class
    virtual void processPendingData();



signals:
    
    //! emitted when the thread is started
    void started();

    //! is emitted when the thread is terminated
    void terminated();

    //! modul signal to send Data, each Module must have this !
    void sendData(DataFrame::SPtr d);

    //! when error detected is should be said
    void informAboutError(MODERROR er);

public slots:

     ///! function to start the thread
     // Dont ovveride start, or the thread is garbage
     //void start();

     ///! function to stopp the thread
     //void stopp();

    ////! modul slot to receive Data , each Module must have this !
    //not this modul, because its the entry point
    //void receiveData(QSharedData d)

//     //! start recording the data
//     void recordStart();

//     //! stopp recording the data of the source
//     void recordStopp()

    //! slot to inform the thread, that he should send his processed data
    //! @see ModuleSource
    void sendYourProcessedData();

protected:



    Device *m_dev;

    //! internal state of the modul
    STATE m_state;
};


//Q_DECLARE_INTERFACE(ThreadSource, "threadsourceInterface/tboy/1.0")

#endif // MODULESOURCE_H
