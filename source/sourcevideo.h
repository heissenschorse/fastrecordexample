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


#ifndef SOURCEVIDEO_H
#define SOURCEVIDEO_H

//TODO implement something !
//TODO make it like SourceSensor::run
//TODO eliminate the includes witch be dont used
//TODO @see addDevice and specifyCam


//qt
#include <QSharedDataPointer>
#include <QTime>
#include <QTimer>
#include <QQueue>
#include <QImage>
#include <QMutex>

//own
#include "modulesource.h"
#include "data.h"
#include "dataframe.h"
#include "datavideo.h"
#include "devicecamera.h"
#include "cammatroxorionhd.h"

//! uncomment for compile in debug mode
//! will switch off the cam inits
//! will grab picutes from screen
#define SOURCE_VIDEO_DEBUG_MODE 0

//! example of video capture source
//! here it catch the screen permanent
class SourceVideo :  public ModuleSource
{
    Q_OBJECT

public:

    //! construct
    SourceVideo();

//    //! befor init TODO @see addDevice
//    void specifyCam(DeviceCamera::CAM_SORT sort);

    //! init all the stuff @see ModuleSource
    //! call for video specifyCam at first
    void init();

    //! @see ModuleSource TODO differ finish ot process, closing all modulChains or stopp ???
    void finishYourProcessing();

    //! get the info, datastream type is used @see DataStreamTypes
    DataStreamTypes::TYPE getDataStreamType();

    //! @see ModuleSource
    void processPendingData();

    //! @ return if is connected
    bool isCamConnected();


public slots:

    //! @see ModuleSource
    void sendYourProcessedData();

signals:

    //! modul signal to send Data, each Module must have this !
    void sendData(DataFrame::SPtr d);

private:

    //! @see ModuleSource::run()
    void run();

    quint64 m_debugCounter;

    //! the cam
    CamMatroxOrionHD *m_cam;

    //! @deprecated
    QQueue<QImage*> m_dataRead;

    //! for specify cam
    DeviceCamera::CAM_SORT m_camSort;

    //! queue of video data
    QQueue<DataVideo*> m_dataQueue;
};



#endif // SOURCEVIDEO_H
