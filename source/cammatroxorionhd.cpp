#include "cammatroxorionhd.h"


CamMatroxOrionHD::CamMatroxOrionHD()
{

    //specify
    m_name = QString("CamMatroxOrionHD");
    m_type = Device::TYPE_CAMERA;
    m_state = Device::STATE_NONE;
    m_sort = DeviceCamera::CAM_SORT_DEFAULT;
    m_uid = 0;

    //standard HD Solutin
    m_sol_x = 0;
    m_sol_y = 0;
    m_lastFrameTimeObject = 0;

}


CamMatroxOrionHD::~CamMatroxOrionHD()
{
//support MATROX_ORION_HD cam
#ifdef SUPPORT_ORION_HD
    //    /* Stop continuous grab. */
    //    MdigHalt(MilDigitizer);

    //free allocated stuff for this cam
    MbufFree(MilImage);
    MdigFree(MilDigitizer);
    MdispFree(MilDisplay);

    //call free for shared stuff
    CamMatroxOrionHDManager::instance()->freeAPPandSystem();

    //        MsysFree(MilSystem);
    //        MappFree(MilApplication);
    //

#endif
}

//Camera::STATE CamMatroxOrionHD::getCamState()
//{
//    return m_state;
//}

void CamMatroxOrionHD::stopp()
{

    m_state = Device::STATE_STOPPED;

    //support MATROX_ORION_HD cam
#ifdef SUPPORT_ORION_HD
    /* Stop continuous grab. */
    MdigHalt(MilDigitizer);

#endif
}

void CamMatroxOrionHD::init()
{

#ifndef SUPPORT_ORION_HD
    qWarning("CamMatroxOrionHD::init() called, but its not supported, use #define SUPPORT_ORION_HD Flag in cammatroxorionhd.h to support it");
#endif
//support MATROX_ORION_HD cam
#ifdef SUPPORT_ORION_HD

    //register cam
    CamMatroxOrionHDManager::instance()->registerCamMatroixOrionHD();

#ifndef ORION_HD
    qDebug("CamMatroxOrionHD::initTheCam() will init with default card, will take some time");

    /* Allocate defaults. */
    //printf("\tDefault");
    MappAllocDefault(M_SETUP, &MilApplication, &MilSystem,
                     &MilDisplay, &MilDigitizer, &MilImage);
#endif
#ifdef ORION_HD
    qDebug("CamMatroxOrionHD::initTheCam() will init with Orion HD card, will take some time");

    //if the MIL_IDS for application and system are initialized already, use them
    if(CamMatroxOrionHDManager::instance()->getMilIDApp()==0)
    {
        qDebug()<< "CamMatroxOrionHD::initTheCam() will init app and system";



        /* Allocate specific Orion HD */
        MappAlloc(M_DEFAULT, &MilApplication);

//        // Disable MIL error message to be displayed as the usual way [CALL TO MIL]
        MappControl(M_ERROR,M_PRINT_DISABLE);

        //alloc default system
        MsysAlloc(M_SYSTEM_ORION_HD, M_DEFAULT, M_DEFAULT, &MilSystem);

        CamMatroxOrionHDManager::instance()->registerMilIDApp(MilApplication);
        CamMatroxOrionHDManager::instance()->registerMilIDSystem(MilSystem);

    }
    else
    {
        qDebug()<< "CamMatroxOrionHD::initTheCam() will reuse app and system ids";

        MilApplication = CamMatroxOrionHDManager::instance()->getMilIDApp();
        MilSystem = CamMatroxOrionHDManager::instance()->getMilIDSystem();
    }


    //Digitizer

    //differ the digitizer in sort of device
    switch (m_sort)
    {
    case DeviceCamera::CAM_SORT_DEFAULT:
        qDebug("CamMatroxOrionHD::initTheCam() will use default");
        MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDigitizer);
           break;
    case DeviceCamera::CAM_SORT_MATROX_ORION_HD_DVI_INPUT:
        qDebug("CamMatroxOrionHD::initTheCam() will use DVI INPUT");
        MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDigitizer);
        break;
    case DeviceCamera::CAM_SORT_MATROX_ORION_HD_SDI_INPUT:
        qDebug("CamMatroxOrionHD::initTheCam() will use SDI INPUT");
        MdigAlloc(MilSystem, M_DEV2, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDigitizer);
        break;

    default:
        qWarning("CamMatroxOrionHD::initTheCam() will unknow cam, use setCamSort() method, try to use default");
        MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDigitizer);
        break;
    }
    //four dvi input use default DVI input
    //MdigAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDigitizer);
    //for SDI is needed 2 or 3 digitizer is needed @see api descripption
    //MdigAlloc(MilSystem, M_DEV2, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDigitizer);

    /* Put digitizer in asynchronous mode */
    //MdigControl(MilDigitizer, M_GRAB_MODE, M_ASYNCHRONOUS);
    //Display
    //default is first
    MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilDisplay);

    //Image Buffer for the Image
    MbufAllocColor(MilSystem,
                   3,
                   MdigInquire(MilDigitizer, M_SIZE_X, M_NULL),
                   MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL),
                   8 + M_UNSIGNED,
                   M_IMAGE+M_GRAB,//+M_DISP+M_BGR32+M_PACKED,
                   &MilImage);
    //second variante // not working !!
    //MbufAlloc2d(MilSystem, MdigInquire(MilDigitizer, M_SIZE_X, M_NULL), MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL),8L+M_UNSIGNED, M_IMAGE+M_GRAB, &MilImage);
    //clear the buffer
    MbufClear(MilImage, 0);
    //bind the image to the display, uncomment to not show the image
    //MdispSelect(MilDisplay, MilImage);

#endif



    //pretend the most init errors
    if(MilSystem == M_NULL|| MilDigitizer== M_NULL||MilImage== M_NULL) // Or Digitzer or .... or or
    {
        qWarning("CamMatroxOrionHD::initTheCam() error, not right initialized");
        m_state  = Device::STATE_ERROR_OCCURED;
    }
    else m_state = Device::STATE_INITIALIZED;

    //check for input

    if(isCamConnected())
    {

        m_state  = Device::STATE_INITIALIZED_AND_SOURCE_AVAILABLE;

        //get usefull information
        MdigInquire(MilDigitizer, M_SIZE_X, &m_sol_x);
        MdigInquire(MilDigitizer, M_SIZE_Y, &m_sol_y);

        qDebug()<<"CamMatroxOrionHD::grabAFrame() has solution "<<m_sol_x << " x "<< m_sol_y;

        printDigitizerInfo(MilDigitizer);
    }
    else {
        qWarning()<< "CamMatroxOrionHD::initTheCam() NO INPUT SOURCE is connected";
    }



#endif

}

void CamMatroxOrionHD::grabAFrame()
{
#ifndef SUPPORT_ORION_HD
    qWarning("CamMatroxOrionHD::grabAFrame() called, but its not supported, use #define SUPPORT_ORION_HD Flag in cammatroxorionhd.h to support it");
#endif
//support MATROX_ORION_HD cam
#ifdef SUPPORT_ORION_HD

    if(m_state==Device::STATE_INITIALIZED_AND_SOURCE_AVAILABLE||m_state==Device::STATE_GRABBING)
    {
        //clear the buffer
        //MbufClear(MilImage, 0);

        //qDebug()<<"CamMatroxOrionHD::grabAFrame() called";

        //make every grab a test if cam is availabel
        if(isCamConnected())//when something is connected
        {

        m_state = DeviceCamera::STATE_GRABBING;

        //get the pictures
        //* Grab ones */
        MdigGrab(MilDigitizer, MilImage);

        //create her the timestamp !!

        m_lastFrameTimeObject = FrameTimeObjectProducer::instance()->getFrameTimeObject();

        }
        else m_state = STATE_INITIALIZED; //it was initialized ! and atm no cam is connected

    }
    else
    {
        qWarning("CamMatroxOrionHD::grabAFrame() couldn't grab a Frame, not in the state");
        if(m_state==Device::STATE_INITIALIZED)
        {
            if(isCamConnected())//when something is connected
            {

                m_state  = Device::STATE_INITIALIZED_AND_SOURCE_AVAILABLE;

                //get usefull information
                MdigInquire(MilDigitizer, M_SIZE_X, &m_sol_x);
                MdigInquire(MilDigitizer, M_SIZE_Y, &m_sol_y);

                //qDebug()<<"CamMatroxOrionHD::grabAFrame() has solution "<<m_sol_x << " x "<< m_sol_y;

                printDigitizerInfo(MilDigitizer);
            }
        }//if already is inited
    }
#endif
}


QImage* CamMatroxOrionHD::getImage()
{
    QImage *image=0;
    quint64 imgSize = 0;

    //check the right state
    if(m_state==Device::STATE_GRABBING)
    {
#ifndef SUPPORT_ORION_HD
    qWarning("CamMatroxOrionHD::getImage() called, but its not supported, use #define SUPPORT_ORION_HD Flag in cammatroxorionhd.h to support it");
#endif
//support MATROX_ORION_HD cam
#ifdef SUPPORT_ORION_HD
    //calc the size of the buffer
    imgSize = m_sol_x * m_sol_y * 3 * sizeof(char); // 3 depends on color band of image RGB
    //qDebug()<< "CamMatroxOrionHD::getImage() imgSize" <<  imgSize ;
    unsigned char *buffer = (unsigned char*) malloc(imgSize ); // 3 depends on color band of image RGB


    if(buffer==NULL)
    {
        qWarning()<<"CamMatroxOrionHD::getImage error !! could allocate buffer";
        return image;
    }

    qDebug()<<" CamMatroxOrionHD::getImage() will construct a new image";
    //process frame , copy to the frame array with a function
    //copy the data array to the rgb_frame 	//working with good result, copy each picutes cost time by 1920 * 1080 * 3 byte ~ ca 6 mb

    //MbufGet(MilImage,(void*) buffer); //not working , deliever several smal pictures

    MbufGetColor(MilImage, M_BGR24+M_PACKED, M_ALL_BAND, (void*)buffer);
//    qDebug()<<" CamMatroxOrionHD::getImage() copied the data to buffer";

//    qDebug()<<" CamMatroxOrionHD::getImage() will construct a new image";

//    //make new image
    image = new QImage(buffer,(int) m_sol_x, (int) m_sol_y,QImage::Format_RGB888);


    qDebug()<<" CamMatroxOrionHD::getImage() will return";

    //get stuff free TODO; free after use !!
    //free(buffer); //not free it will be used
#endif
    }//if a image is grabbed
   return image;
}

//uchar* CamMatroxOrionHD::getRawDataOfOnePicture()
//{

//}

DataVideo* CamMatroxOrionHD::getImageAsDataVideo()
{
    DataVideo *myDataVid = 0;


#ifndef SUPPORT_ORION_HD
    qWarning("CamMatroxOrionHD::getImageAsDataVideo() called, but its not supported, use #define SUPPORT_ORION_HD Flag in cammatroxorionhd.h to support it");
#endif
//support MATROX_ORION_HD cam
#ifdef SUPPORT_ORION_HD


    //check the right state
    if(m_state==Device::STATE_GRABBING)
    {
        myDataVid = new DataVideo((uint) m_sol_y, (uint) m_sol_x, m_lastFrameTimeObject );
        //if(!myDataVid->isBuffNull())qWarning("CamMatroxOrionHD::getImageAsDataVideo() error buff is not right initialized");
        //process frame , copy to the frame array with a function
        //copy the data array to the rgb_frame 	//working with good result, copy each picutes cost time by 1920 * 1080 * 3 byte ~ ca 6 mb
        MbufGetColor(MilImage, M_RGB24+M_PACKED, M_ALL_BAND, (void*) myDataVid->getPointerToBuff());
    }
#endif

    return myDataVid;
}

#ifdef SUPPORT_ORION_HD
void CamMatroxOrionHD::printDigitizerInfo(MIL_ID MilDigitizer)
{

       MIL_TEXT_CHAR  DCFFormat[255];
       MIL_INT        ColorMode;
       MIL_INT        InputMode;
       MIL_INT        ScanMode;
       MIL_INT        SizeX, SizeY, SizeBand;
       MIL_DOUBLE     FrameRate;
       MIL_INT        DigitizerNumber;

       MdigInquire(MilDigitizer, M_NUMBER, &DigitizerNumber);
       MdigInquire(MilDigitizer, M_SIZE_X, &SizeX);
       MdigInquire(MilDigitizer, M_SIZE_Y, &SizeY);
       MdigInquire(MilDigitizer, M_SIZE_BAND, &SizeBand);
       MdigInquire(MilDigitizer, M_INPUT_MODE, &InputMode);
       MdigInquire(MilDigitizer, M_COLOR_MODE, &ColorMode);
       MdigInquire(MilDigitizer, M_SCAN_MODE, &ScanMode);
       MdigInquire(MilDigitizer, M_SELECTED_FRAME_RATE, &FrameRate);
       MdigInquire(MilDigitizer, M_FORMAT_DETECTED, DCFFormat);

       MosPrintf(MIL_TEXT("CamMatroxOrionHD::printDigitizerInfo:                                   \r"));
       MosPrintf(MIL_TEXT("\t%d x %d, %d band(s). "), SizeX, SizeY, SizeBand);

       switch(ColorMode)
          {
          case M_DVI: MosPrintf(MIL_TEXT("DVI "));break;
          case M_SDI: MosPrintf(MIL_TEXT("SDI "));break;
          case M_COMPOSITE: MosPrintf(MIL_TEXT("Composite "));break;
          case M_RGB: MosPrintf(MIL_TEXT("RGB "));break;
          case M_EXTERNAL_CHROMINANCE: MosPrintf(MIL_TEXT("Y\\C "));break;
          case M_MONOCHROME: MosPrintf(MIL_TEXT("Monochrome "));break;
          default: MosPrintf(MIL_TEXT(" "));
          }
       switch(InputMode)
          {
          case M_ANALOG: MosPrintf(MIL_TEXT("analog "));break;
          case M_DIGITAL: MosPrintf(MIL_TEXT("digital "));break;
          default: MosPrintf(MIL_TEXT(" "));
          }
       switch(ScanMode)
          {
          case M_PROGRESSIVE: MosPrintf(MIL_TEXT("progressive "));break;
          case M_INTERLACE: MosPrintf(MIL_TEXT("interlaced "));break;
          default: MosPrintf(MIL_TEXT(" "));
          }

       MosPrintf(MIL_TEXT("@ %0.2f fps.\n"), FrameRate);
       MosPrintf(MIL_TEXT("                                                                              \r"));
       MosPrintf(MIL_TEXT("\tDCF: %s.\n\n"), DCFFormat);

}
#endif

bool CamMatroxOrionHD::isCamConnected()
{

#ifdef SUPPORT_ORION_HD
     if(MdigInquire(MilDigitizer, M_CAMERA_PRESENT, M_NULL) == M_TRUE)
         return true;
#endif

     return false;
}


