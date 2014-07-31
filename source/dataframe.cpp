#include "dataframe.h"

DataFrame::DataFrame(Data::TYPE type)
{
    m_dataTyp = type;
}

void DataFrame::addData(Data *dat)
{
    //test
    if(isTypeSafe(dat))
    {

      //add data to list
      Data::SPtr datSPtr(dat);

      m_dataQueue.append(datSPtr);



    }
    else qWarning()<<"DataFrame::addData error type mismatch !";
}

void DataFrame::addData(Data::SPtr dat)
{
    if(dat->getDataType()==m_dataTyp)
    {
        m_dataQueue.append(dat);
    }
    else  qWarning()<<"DataFrame::addData error type mismatch !";
}

bool DataFrame::isTypeSafe(Data *dat)
{
    //check type
    if(dat->getDataType()==m_dataTyp)
        return true;

    return false;
}


bool DataFrame::isNOTOverSizeOfQueue(int at)
{
    //! test range
    if(at<m_dataQueue.size()&&at>=0)
        return true;

    return false;
}

DataVideo::SPtr DataFrame::getQueueElementAsVideoAt(int at)
{
    if(m_dataTyp==Data::TYPE_VIDEO)
    {

        if(isNOTOverSizeOfQueue(at))
        {

            //get ptr
            Data::SPtr datPtr = m_dataQueue.at(at);

            //cast to video
            DataVideo::SPtr videodataPtr = datPtr->getAsVideo(datPtr);

            //make it to share pointer
            //QSharedPointer<DataSensor> senDataPtr(senData);

            return videodataPtr;

        }
        else qWarning()<<"DataFrame::getQueueElementAsAudioAt error address not existing element";

    }
    qWarning()<<"DataFrame::getQueueElementAsAudioAt error DataFrame is no Data::TYPE_VIDEO! ";

    return DataVideo::SPtr();
}



FrameTimeObject* DataFrame::getStartFrameTime()
{
    if(!m_dataQueue.isEmpty())
    {
        return m_dataQueue.at(0)->getFrameTime();
    }
    else return 0 ;
}

int DataFrame::getSize()
{
    return m_dataQueue.size();
}


bool DataFrame::isEmpty()
{
    if(getSize()==0)return true;
    return false;
}
