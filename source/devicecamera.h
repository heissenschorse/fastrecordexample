#ifndef CAMERA_H
#define CAMERA_H

//qt
#include <QDebug>

//own
#include "device.h"

//! a class to interface all implemented cams
//! each new camera build an subclass
//! alias is DeviceVideo
class DeviceCamera : public Device
{

public:

//    //! in which state is the camera
//    enum STATE
//    {
//        //! when this camera is not
//        NOT_AVAILABLE,
//        //! state camera is  initialized, without any errors
//        INITIALIZED,
//        //! when the camera is delivering pictures
//        GRABBING,
//        //! when the cam is doing nothing
//        STOPPED

//    };

    //! List of producers
    enum CAM_SORT
    {
        CAM_SORT_DEFAULT, //read from config file
        CAM_SORT_MATROX_ORION_HD_DVI_INPUT,
        CAM_SORT_MATROX_ORION_HD_SDI_INPUT,
        CAM_SORT_HDC_302E,
        CAM_SORT_DEBUG_CAM, // only needed for debugging, fake cam
        CAM_SORT_FILE_LOADER
    };

    //! constructor
    DeviceCamera();

    ////! deliver the state of the camera @see Camera::STATE
    //STATE getCamState();

    //! call before init, what sort of cam is it
    void setCamSort(CAM_SORT sort);

    CAM_SORT getCamSort();

protected:



    //! init function, call functions, load libs for
    //! get decive in state STATE::INITIALIZED
    virtual void init();

    //! grab a Frame
    virtual void grabAFrame();

    //! a sort of camera , like webcam X3423.u
    CAM_SORT m_sort;
    //STATE m_state;

//no here, because not qobject
//signals:
//    //! emitted when state of the cam changed
//    void stateChanged(STATE state);

};

#endif // CAMERA_H
