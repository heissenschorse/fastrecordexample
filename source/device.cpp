#include "device.h"


Device::Device()
{
    m_state = STATE_NONE;
    m_type = TYPE_NONE;
}

Device::Device(TYPE type)
{
    m_type =type ;
    //add a  new state
    m_state = STATE_NONE;  //= new DeviceState(DeviceState::NONE);
}

//void Device::addState(DeviceState *state)
//{
//    m_state = state;

//    //add parent
//    state->setDevice(this);
//}

void Device::setName(QString s)
{
    m_name =s;
}


Device::STATE Device::getState()
{
    return m_state;
}

QString Device::getName()
{
    return m_name;
}

//quint64 Device::getDevNum()
//{
//    return m_devNumber;
//}

//quint64 Device::setDevNum(quint64 x)
//{
//    m_devNumber = x;
//}

void Device::changeState(STATE state)
{
    //TODO perhapts emit state change
    m_state = state;
}


Device::TYPE Device::getType()
{
    return m_type;
}


QString Device::getStateHumanReadable()
{
    QString str;

    switch(m_state)
    {
    case STATE_DEVICE_NOT_AVAILABLE:
        str.append("STATE_DEVICE_NOT_AVAILABLE");
        break;
    case STATE_INITIALIZED:
        str.append("STATE_INITIALIZED");
        break;
    case STATE_INITIALIZED_AND_SOURCE_AVAILABLE:
        str.append("STATE_INITIALIZED_AND_SOURCE_AVAILABLE");
        break;
    case STATE_GRABBING:
        str.append("STATE_GRABBING");
        break;
    case STATE_STOPPED:
        str.append("STATE_STOPPED");
        break;
    case STATE_ERROR_OCCURED:
        str.append("STATE_ERROR_OCCURED");
        break;
    case STATE_NONE:
        str.append("STATE_NONE");
        break;
    }

    return str;
}


QString Device::toString()
{
        QString str;
        str.append("Device:");
        str.append(m_name);
        str.append(";type(");
        str.append(m_type);
        str.append(";state(");
        str.append(getStateHumanReadable());
        str.append(")\n");

        return str;

}
