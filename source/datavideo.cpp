#include "datavideo.h"

DataVideo::DataVideo(uint heigt, uint width, FrameTimeObject *frameTime)
{
    //qDebug()<< "DataVideo "<< heigt << "  "<< width;
    init(heigt, width);
    m_frametime = frameTime;
}

DataVideo::~DataVideo()
{
    free(m_rawDataBuffer);
}

//DataVideo::DataVideo(uchar *data, int width, int height, QImage::Format format)
//{
//    init();
//    m_image =  new QImage(data,  width, height, format );

//    //if(m_image.isNull())qWarning("DataVideo constructor load invalid image");
//}

Data::TYPE DataVideo::getDataType()
{
    return m_dataType;
}

QString DataVideo::toString()
{

    QString str;

    str.append(QString("DataVideo:Head:"));
    str.append(m_frametime->toString());
    //data

    return str;
}

bool DataVideo::isBuffNull()
{
    for(quint64 i=0;i<getSizeOfBuff();i++)
    {
        if(m_rawDataBuffer[i] ==0)
        {
            qWarning("DataVideo::isBuffNull() true on element %i",(int)i);
            return true;
        }
    }
    return false;
}

QImage DataVideo::getAsImage()
{
    return QImage(m_rawDataBuffer,m_width,m_heigt,QImage::Format_RGB888);
}

//void DataVideo::addImage(QImage *img)
//{
//    m_image =img;
//}

void DataVideo::init(uint heigt, uint width)
{
    //default stuff
    m_dataType = Data::TYPE_VIDEO;
    m_frametime = FrameTimeObjectProducer::instance()->getFrameTimeObject();

    //the buffer
    m_rawDataBuffer = (uchar*) malloc(sizeof(uchar) *   width * heigt * DATAVIDEO_BAND); // 3 depends on color image

    m_heigt = heigt;
    m_width = width;

    //init //better use calloc ?
    int size = getSizeOfBuff();
    for(int i=0;i<size;i++)m_rawDataBuffer[i] = 0;

}
uchar* DataVideo::getPointerToBuff()
{
    return m_rawDataBuffer;
}

quint64 DataVideo::getSizeOfBuff()
{
    //qDebug()<< "DataVideo::getSizeOfBuff() "<< m_heigt * m_width * DATAVIDEO_BAND;

    return (m_heigt * m_width * DATAVIDEO_BAND);
}

void DataVideo::copyDataToBuff(void *dest, int byteAmount)
{
    if(!isBuffNull())
    {
        qDebug()<<"DataVideo::copyDataToBuff  " << byteAmount;
        memcpy( dest,  m_rawDataBuffer , (int) byteAmount ) ;
    }
}

//********************* Belongs to Data.cpp ****************

DataVideo::SPtr Data::getAsVideo(Data::SPtr ptr)
{
    if(ptr->getDataType()==Data::TYPE_VIDEO)
    {
        return  ptr.dynamicCast<DataVideo>();
    }
    qWarning("Data::getAsVideo type mismatch during cast");

    return DataVideo::SPtr();
}
