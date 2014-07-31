#include "devicecamera.h"

DeviceCamera::DeviceCamera()
{
    m_sort = CAM_SORT_DEFAULT;
    m_type = TYPE_CAMERA;
    //m_state = NOT_AVAILABLE;
}


//DeviceCamera::STATE Camera::getCamState()
//{
//    return  m_state;
//}


void DeviceCamera::grabAFrame()
{
    qDebug("Camera::getCamState() is base class, plz overide me");
}

void DeviceCamera::init()
{
   qDebug("Camera::init() is base class, plz overide me");
}


void DeviceCamera::setCamSort(DeviceCamera::CAM_SORT sort)
{
    m_sort = sort;
}


DeviceCamera::CAM_SORT DeviceCamera::getCamSort()
{
    return m_sort;
}
