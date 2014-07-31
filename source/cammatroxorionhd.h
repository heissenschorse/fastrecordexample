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


#ifndef CAMMATROXORIONHD_H
#define CAMMATROXORIONHD_H

//TODO whats the same in the base class ?
//TODO find a way getImage to copy one less the pix
//TODO make it possible to specify the digitizer of orion hd card
//TODO getImageAsDataVideo called new, free it !
//TODO Configuration ORION_HD and LOGGER_USE_FFMPEG  in config state, refactor ! use CONFIG_USE_XXX CONFIG_WITH_GRABBER_ORION_HD or so


//qt
#include <QImage>

//std
#include <stdlib.h>

//own
//#include "phase.h"
#include "device.h"
#include "devicecamera.h"
#include "datavideo.h"
#include "cammatroxorionhdmanager.h"


//uncomment to support the matrox orion hd card
//@see CamMatroxOrionHDManager file has also this define !
//#define SUPPORT_ORION_HD
//#endif

// without the MIL default is loaded
//define witch card is used
#define ORION_HD

// when  it will be used include
#ifdef SUPPORT_ORION_HD
//for matrox ordion hd card
#include "../include/cam_orion_hd/mil.h"
#endif

//! the hardware abstraction layer to the
//! matrox orion hd framegrabber card
//! is an konret implementation for specific cam
class CamMatroxOrionHD : public DeviceCamera
{

public:

    //! constructor
    CamMatroxOrionHD();

    virtual ~CamMatroxOrionHD();

    //! do the init steps
    void init();

//    //! @see Camera
//    Camera::STATE getCamState();

    //! @see Camera
    void grabAFrame();

    //! get a image from the card
    QImage* getImage();

    //! return DataVideo (saved as rawData in the class)
    DataVideo* getImageAsDataVideo();

//    //!  return pointer of buffer (size sol_x * sol_y * 3 byte)
//    uchar* getRawDataOfOnePicture();

    //! stop the grabbing and free the content
    void stopp();

    //! do a check if a source is connected
    //! @return true if cam is connected
    bool isCamConnected();



private:

    //!stores frameTimeObject from last grabAFrame call
    FrameTimeObject *m_lastFrameTimeObject;

// when  it will be used include
#ifdef SUPPORT_ORION_HD


    //*****Solution****/
    MIL_INT m_sol_x, m_sol_y;

//**** MIL Stuff***/
    MIL_ID MilApplication,  /* Application identifier.  */
        MilSystem,       /* System identifier.       */
        MilDisplay,      /* Display identifier.      */
        MilDigitizer,    /* Digitizer identifier.    */
        MilImage;        /* Image buffer identifier. */


    //! Prints to the console the details of the detected input.
    void printDigitizerInfo(MIL_ID MilDigitizer);

#else
    int m_sol_x, m_sol_y;
#endif



};

#endif // CAMMATROXORIONHD_H
