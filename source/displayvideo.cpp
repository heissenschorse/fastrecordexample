#include "displayvideo.h"

DisplayVideo::DisplayVideo(QWidget *parent)
{
    Q_UNUSED(parent);

    init();
}


DisplayVideo::DisplayVideo()
{

    init();
}


//! get the info, datastream type is used @see DataStreamTypes
DataStreamTypes::TYPE DisplayVideo::getDataStreamType()
{
    return DataStreamTypes::VIDEO;
}


void DisplayVideo::receiveData(DataFrame::SPtr dataFrame)
{

    if(DISPLAY_VIDEO_DEBUG_MODE)qDebug("DisplayVideo[%i]::receiveData  received data",(int)m_personalID);

    //when the widget is showed
    if(this->isVisible())
    {

        DataVideo::SPtr vidData = dataFrame->getQueueElementAsVideoAt(0); //should be one in the frame

        QImage image =  vidData.data()->getAsImage();

        *m_pixmap =  QPixmap::fromImage(image);

        //TODO get size from above
        QSize myScaleSize = this->parentWidget()->size();
        //qDebug()<< "DisplayVideo::receiveData myScaleSize"  << myScaleSize.height() << " x "<<  myScaleSize.width();

        //get a bound // a fast bad hack
        if(myScaleSize.width()>1778 || myScaleSize.height()> 680)
        {
            myScaleSize = QSize(1778, 680);
            //qDebug()<< "DisplayVideo::receiveData bound ! "  ;
        }
        *m_pixmap =  m_pixmap->scaled(myScaleSize,Qt::KeepAspectRatio,Qt::FastTransformation);

        m_label->setPixmap(*m_pixmap);


    this->update();
    //*** draw it
//    //the rects
//    QRectF target(0, 0, 1920, 1080);
//    QRectF source(0, 0, 1920, 1080);

//    //clear
//    m_painter->eraseRect(target);

//    //display the qimage

//    m_painter->drawImage(target,(const QImage) *(vidData->getImage()),source);

//    m_window->update();

//    for(int i=0;i<10000000000;i++)
//        i*i*i;
//    QString fileName(UtilFactoryDataFileName::getDatePattern());
//    fileName.append(QString("-grab.%1.png").arg(m_debugCounter++));

//    //save it to file
//    QFile file(fileName);
//    if (!file.open(QIODevice::WriteOnly))
//    {

//        qWarning("DisplayVideo::receiveData error while open  the file");
//    }
//    else
//    {
//        //uchar *buffer = (uchar*) malloc(sizeof(uchar) * sol_x * sol_y * 3); // 3 depends on color image

//        qDebug("DisplayVideo::receiveData will save the picture to file");


//        uchar *buffer = vidData->getPointerToBuff();

//        QImage myImage(buffer,1920,1080,QImage::Format_RGB888);

////        int bufferLength = vidData->getSizeOfBuff();// vidData->getImage()->byteCount();

////        buffer  = vidData->getImage()->bits();

////        if(buffer==0)qWarning("DisplayVideo::receiveData error");


//        myImage.save(&file,"PNG");

////        //write it
////        QTextStream out(&file,);

////        //copy
////        for( int i = 0; i<bufferLength;i++)
////        {
////            out << buffer[i];
////        }

//        //close file
//        file.close();

    }//if visible
    //send the data
    emit sendData(dataFrame);
}

void DisplayVideo::init()
{


     qDebug("DisplayVideo::receiveData will construct the picture Widget");

     m_personalID = rand();
     m_debugCounter = 0;


     m_label = new QLabel(this);
     m_pixmap = new QPixmap();

     m_layout = new QHBoxLayout(this);
     m_layout->addWidget(m_label);

     //add layout

     this->setLayout(m_layout);

//     m_window = new QWidget();
//     m_window->setWindowTitle("Situs Cam");

//     m_painter = new QPainter(m_window);
//     m_painter->setPen(Qt::blue);

//     m_window->show();
     //take all the range witch is given
     this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
