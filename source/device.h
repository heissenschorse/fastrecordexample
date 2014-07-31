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

#ifndef DEVICE_H
#define DEVICE_H

// qt
#include <QString>
#include <QList>

//own
//#include "devicestate.h"

//! entity which represent a  device , for example: camera
//! is for describing and handling from gui
class Device
{
public:

    //! type of device
    enum TYPE
    {
        //! camera, video socket
        TYPE_CAMERA,
        //! sensor
        TYPE_SENSOR,
        //! all audio inputs which are availabel
        TYPE_ADUIO, //TODO rename !!
        //! default
        TYPE_NONE
    };


    //! state in which state  device could be
    enum STATE
    {
        //! when this camera is not
        STATE_DEVICE_NOT_AVAILABLE,
        //! state camera is  initialized, without any errors
        STATE_INITIALIZED,
        //! state camera is  initialized and an source is connected
        STATE_INITIALIZED_AND_SOURCE_AVAILABLE,
        //! when the camera is delivering pictures
        STATE_GRABBING,
        //! when the cam is doing nothing
        STATE_STOPPED,
        //! when error occured and not in stable state
        STATE_ERROR_OCCURED,
        //! when no state in known
        STATE_NONE
    };

    //! a def for dev list
    typedef QList<Device*> List;

    //! a def for using states as list
    typedef QList<Device::STATE> StateList;

    //******* methods
    Device();

    Device(Device::TYPE type);

//    //! add a other state to dev
//    void addState(DeviceState *state);

    //! change state of an device
    void changeState(STATE state);

    //! get
    STATE getState();

    //! for debug
    QString toString();

    //! void setName
    void setName(QString s);

    //! get
    QString getName();

    //! get
    TYPE getType();

    //! for debug
    QString getStateHumanReadable();


//    //! set
//    void setDevNum(quint64 x);

//    //! get
//    quint64 getDevNum();

protected:

    //! name gerät
    QString m_name;

    //! type
    TYPE m_type;

    //! state of device
    STATE m_state;

    //! each konkret device has an
    //! unique id
    quint64 m_uid;

//    //! number of of this TYPE, use StreamDescription::STREAM_NUM insted
//    quint64 m_devNumber;

    //! is implemented flag
    bool m_implementFlag;

};

#endif // DEVICE_H
