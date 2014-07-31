#include "sourcevideo.h"


SourceVideo::SourceVideo()
{
     //qDebug("SourceVideo::Constructor doesn't do anything, please implement me");
     m_debugCounter = 0;
     m_dev =0;
     //call init
     //init();
}

void SourceVideo::processPendingData()
{
    if(SOURCE_VIDEO_DEBUG_MODE)qDebug("SourceVideo::processPendingData() ");
    //take a picutre
    m_cam->grabAFrame();
//    //save it in queue
//    m_dataRead.append(m_cam->getImage());


    //make a better error handling
    //test device state --> set module state
    Device::STATE state =m_cam->getState();
    if(state == Device::STATE_ERROR_OCCURED)
    {
        qDebug()<< "SourceVideo::processPendingData error!! while "<< m_cam->getStateHumanReadable() ;
        m_state = STATE_UNKNOWN;
        //TODO inform about modul error  !!

        return ;
    }

    if(state == Device::STATE_INITIALIZED)
    {
        qDebug()<< "SourceVideo::processPendingData error!! while "<< m_cam->getStateHumanReadable() ;
        m_state = STATE_INITIALIZED;
        //TODO inform about modul error  !!

        return ;
    }

    if(state == Device::STATE_DEVICE_NOT_AVAILABLE)
    {
        qDebug()<< "SourceVideo::processPendingData error!! while "<< m_cam->getStateHumanReadable() ;
        m_state = STATE_UNKNOWN;
        //TODO inform about modul error  !!
        return;
    }
    if(state == Device::STATE_STOPPED)
    {
        qDebug()<< "SourceVideo::processPendingData error!! while "<< m_cam->getStateHumanReadable() ;
        m_state = STATE_STOPPED;
        //TODO inform about modul error  !!
        return;
    }

    //get a image
    DataVideo *image =  m_cam->getImageAsDataVideo();

     //check for null pointer
    if(image!=0)
    {
            //save it as DataVideo
             m_dataQueue.append(image);

             m_state = ModuleSource::STATE_PROCESSING;

    }
    else   qDebug()<<"SourceVideo::processPendingData() nothing to process";

}
bool SourceVideo::isCamConnected()
{
    return m_cam->isCamConnected();
}

void SourceVideo::sendYourProcessedData()
{
    if(SOURCE_VIDEO_DEBUG_MODE)
        qDebug("SourceVideo::sendYourProcessedData() called");

    //call my own
    processPendingData();

    //send it
    //qDebug("SourceVideo::sendYourProcessedData() has %i picture in queue, will sent one",m_dataRead.size());

    //pretend errors while reading and writing
    QMutex mutex; //lock to be save
    mutex.lock();

    //check if their are data
    if(m_dataQueue.isEmpty())
    {
        qWarning("SourceSensor::sendYourProcessedData() has no data to be send");
    }
    else
    {

        //DataVideo vidData = m_dataQueue.dequeue();
        //TODO here error on windows
        // C:\Qt\Qt5.0.1\5.0.1\mingw47_32\include\QtCore\qsharedpointer_impl.h:209: required from 'static void QtSharedPointer::ExternalRefCountWithCustomDeleter<T, Deleter>::deleter(QtSharedPointer::ExternalRefCountData*) [with T = DataVideo; Deleter = QtSharedPointer::NormalDeleter; QtSharedPointer::ExternalRefCountData = QtSharedPointer::ExternalRefCountData]'

        //construct the container, dataFrame
        DataFrame::SPtr senDataFrame( new DataFrame(Data::TYPE_VIDEO));

        //add data
        senDataFrame->addData((Data*) m_dataQueue.dequeue());

        //DataVideo vidData = m_dataQueue.dequeue();

        //hard way
        //memcpy(vidDataPtr->getPointerToBuff(),vidData.getPointerToBuff(),(size_t)(vidData.getSizeOfBuff()* sizeof(uchar)));
        //make a test with data

        //qDebug("SourceVideo::sendData the %i time with %i left to send  ",(int) m_debugCounter++, (int)m_dataQueue.size());
        emit sendData( senDataFrame);

        //emit sendData(DataVideo::SPtr());

        //delete the elements which were sent
        //m_dataRead.clear();
        //qDebug("SourceVideo::sendData done ");

        //set state
        m_state = ModuleSource::STATE_PROCESSING;
    }

    //oppen the mutex
    mutex.unlock();

}

void SourceVideo::run()
{
    if(SOURCE_VIDEO_DEBUG_MODE)qDebug()<< "SourceVideo will start  id: (" <<this->thread()->currentThreadId() << ")" ;

    //this->event(QEvent *foo);
    //start timer

    this->exec();
}

void SourceVideo::init()
{
    m_cam = new CamMatroxOrionHD();

    if(m_dev==0)
    {
        qWarning()<<"SourceVideo::init() error while init Modul SourceVideo with NO device";
        m_state =ModuleSource::STATE_UNKNOWN;
        return;
    }

//    //TODO differ the device
    if(m_dev->getType()!=Device::TYPE_CAMERA)
    {
        qWarning()<<"SourceVideo::init() error while init Modul SourceVideo with wrong Device of type:"<<m_dev->getType()<<  " name: "<<m_dev->toString();
        m_state =ModuleSource::STATE_UNKNOWN;
        return;
    }
    //else

    //cast to
    DeviceCamera *myCamDev = (DeviceCamera*) m_dev;
    DeviceCamera::CAM_SORT camSort = myCamDev->getCamSort();

    qDebug()<< "SourceVideo::init() will construct type cam" << camSort;

    //TODO use the  myCamDev for init and the rest !!
    m_cam->setCamSort(camSort);


    //set sort of device from

    m_cam->init();

    if(m_cam->isCamConnected())
        m_state = ModuleSource::STATE_INITIALIZED;
    else
    {
        qWarning()<<"SourceVideo::init() no cam is connected";
        m_state =ModuleSource::STATE_UNKNOWN;
    }
}



DataStreamTypes::TYPE SourceVideo::getDataStreamType()
{
    return DataStreamTypes::VIDEO;
}

void SourceVideo::finishYourProcessing()
{

    //halt the cam
    m_cam->stopp();

    delete m_cam;

    //finish thread
    this->quit();

    //set state to stopped
    m_state = ModuleSource::STATE_STOPPED;

}
