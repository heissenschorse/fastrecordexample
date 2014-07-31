#include "modulechainfactory.h"

//************ singelton stuff **** START

//must be for declerate the singelton
ModuleChainFactory* ModuleChainFactory::m_instance = 0;


ModuleChainFactory::ModuleChainFactory()
{
}
ModuleChainFactory* ModuleChainFactory::instance()
{

    static QMutex mutex;
    if (!m_instance)
    {
        mutex.lock();

        if (!m_instance)
            m_instance = new ModuleChainFactory();

        mutex.unlock();
    }

    return m_instance;
}

void ModuleChainFactory::drop()
{
    static QMutex mutex;
    mutex.lock();
    delete m_instance;
    m_instance = 0;
    mutex.unlock();
}
//************ singelton stuff **** END


ModuleChain* ModuleChainFactory::fabricateModuleChain(CHAINTYPE chaintype, DataStreamTypes::TYPE datastreamtype, Device *dev)
{
    //chain
    ModuleChain *chain  = new ModuleChain();

    //set update intervall
    QTimer *timer = new QTimer();
    //callulate the timer intervall
    int frequenz = MODULE_UPDATE_FREQUENZY_IN_HZ;
    if(MODULE_UPDATE_FREQUENZY_IN_HZ<0||MODULE_UPDATE_FREQUENZY_IN_HZ>100)
    {
        frequenz = 50;
        qWarning(" ModuleChainFactory::fabricateModuleChain  MODULE_UPDATE_FREQUENZY_IN_HZ in an unnormal range, will set to 50");
    }
    int intervall = 1000 / frequenz;
    timer->setInterval(intervall);

    qDebug("ModuleChainFactory::fabricateModuleChain: will fabricate ModulChain of ModuleChainFactory::CHAINTYPE(%i) and DataStreamTypes::TYPE(%i)",chaintype, datastreamtype);

    //debug
    bool succ = false;

    //declerate the vid chain
    SourceVideo *vidSourc;
    DisplayVideo *vidDisplay;
    LoggerVideo *vidLogger;

    //declare the sensor chain
//    SourceSensor *senSourc;
//    DisplaySensor *senDisplay;
//    LoggerSensor *senLogger;

//    //declare the audio chain
//    SourceAudio *audSource;
//    DisplayAudio *audDisplay;
//    LoggerAudio *audLogger;

    switch(chaintype)
    {
        case TYPE_NORMAL:
                    switch(datastreamtype)
                    {
                        case DataStreamTypes::VIDEO:
                                                //init the objects
                                                vidSourc = new SourceVideo();
                                                vidDisplay = new DisplayVideo();
                                                vidLogger = new LoggerVideo();

                                                //gave the threads an own event queue
                                                vidSourc->moveToThread(vidSourc);

                                                //add the given device to the Source
                                                vidSourc->addDevice(dev);

                                                //call init after specify the cam sort
                                                vidSourc->init();

                                                //connect the signals, make a list
                                                // source -> display
                                                succ = QObject::connect(vidSourc, SIGNAL(sendData(DataFrame::SPtr)),
                                                                        vidDisplay, SLOT(receiveData(DataFrame::SPtr)));
                                                //check display -> logger
                                                if(!succ)qWarning("ModuleChainFactory::fabricateModuleChain error while connect the moduls");

                                                succ = QObject::connect(vidDisplay, SIGNAL(sendData(DataFrame::SPtr)),
                                                                        vidLogger, SLOT(receiveData(DataFrame::SPtr)));
                                                //check
                                                if(!succ)qWarning("ModuleChainFactory::fabricateModuleChain error while connect the moduls");

                                                //connect the timer -> source
                                                succ = QObject::connect(timer, SIGNAL(timeout()),
                                                                        vidSourc, SLOT(sendYourProcessedData()));
                                                //check
                                                if(!succ)qWarning("ModuleChainFactory::fabricateModuleChain error while connect the moduls");



                                                //make the list
                                                chain->appendModule(vidSourc);
                                                chain->appendModule(vidDisplay);
                                                chain->appendModule(vidLogger);

                                                //new way start timer when run is called
                                                chain->addTimerReference(timer);

                                                //old way start timer in time
                                                //start the timer
                                                //timer->start();

                                                break;

//NOT USED HERE*********************************************************
//                        case DataStreamTypes::AUDIO:
//                                                qDebug("ModuleChainFactory::fabricateModuleChain CHAINTYPE::NORMAL DataStreamTypes::AUDIO  constructed");
//                                                //init the objects
//                                                audSource = new SourceAudio();
//                                                audDisplay = new DisplayAudio();
//                                                audLogger = new LoggerAudio();


//                                                //gave the threads an own event queue
//                                                audSource->moveToThread(audSource);

//                                                //add the given device to the Source
//                                                audSource->addDevice(dev);

//                                                //connect the signals, make a list
//                                                // source -> display
//                                                succ = QObject::connect(audSource, SIGNAL(sendData(DataFrame::SPtr)),
//                                                        audDisplay, SLOT(receiveData(DataFrame::SPtr)));
//                                                //check
//                                                if(!succ)qWarning("ModuleChainFactory::fabricateModuleChain error while connect the moduls");

//                                                succ = QObject::connect(audDisplay, SIGNAL(sendData(DataFrame::SPtr)),
//                                                audLogger, SLOT(receiveData(DataFrame::SPtr)));
//                                                //check
//                                                if(!succ)qWarning("ModuleChainFactory::fabricateModuleChain error while connect the moduls");

//                                                //connect the timer -> source
//                                                succ = QObject::connect(timer, SIGNAL(timeout()),
//                                                                        audSource, SLOT(sendYourProcessedData()));
//                                                //check
//                                                if(!succ)qWarning("ModuleChainFactory::fabricateModuleChain error while connect the moduls");

//                                                //
//                                                //make the list
//                                                chain->appendModule(audSource);
//                                                chain->appendModule(audDisplay);
//                                                chain->appendModule(audLogger);

//                                                //new way start timer when run is called
//                                                chain->addTimerReference(timer);

//                                                break;
//                        case DataStreamTypes::SENSOR:
//                                                //init the objects
//                                                senSourc = new SourceSensor();
//                                                senDisplay = new DisplaySensor();
//                                                senLogger = new LoggerSensor();


//                                                //gave the threads an own event queue
//                                                senSourc->moveToThread(senSourc);

//                                                //add the given device to the Source
//                                                senSourc->addDevice(dev);

//                                                //connect the signals, make a list
//                                                // source -> display
//                                                succ = QObject::connect(senSourc, SIGNAL(sendData(DataFrame::SPtr)),
//                                                        senDisplay, SLOT(receiveData(DataFrame::SPtr)));
//                                                //check
//                                                if(!succ)qWarning("ModuleChainFactory::fabricateModuleChain error while connect the moduls");

//                                                succ = QObject::connect(senDisplay, SIGNAL(sendData(DataFrame::SPtr)),
//                                                senLogger, SLOT(receiveData(DataFrame::SPtr)));
//                                                //check
//                                                if(!succ)qWarning("ModuleChainFactory::fabricateModuleChain error while connect the moduls");

//                                                //connect the timer -> source
//                                                succ = QObject::connect(timer, SIGNAL(timeout()),
//                                                                        senSourc, SLOT(sendYourProcessedData()));
//                                                //check
//                                                if(!succ)qWarning("ModuleChainFactory::fabricateModuleChain error while connect the moduls");

//                                                //
//                                                //make the list
//                                                chain->appendModule(senSourc);
//                                                chain->appendModule(senDisplay);
//                                                chain->appendModule(senLogger);

//                                                //new way start timer when run is called
//                                                chain->addTimerReference(timer);

//                                                break;
//****************************************************************************
                        default:  qWarning("ModuleChainFactory::fabricateModuleChain unexpect parameter,DataStreamTypes::TYPE(%i)",datastreamtype);
                                  break;
                    }//end switch datastreamtype

                    break; // break normal

        case TYPE_SHORT:

            switch(datastreamtype)
            {
            //TODO see above   case TYPE_NORMAL to imagine how it works
            case DataStreamTypes::VIDEO:

                                    //init the objects
                                    vidSourc = new SourceVideo();
                                    vidLogger = new LoggerVideo();

                                    //gave the threads an own event queue
                                    vidSourc->moveToThread(vidSourc);

                                    //add the given device to the Source
                                    vidSourc->addDevice(dev);

                                    //call init after specify the cam sort
                                    vidSourc->init();


                                    //connect the signals, make a list
                                    // source -> display
                                    succ = QObject::connect(vidSourc, SIGNAL(sendData(DataFrame::SPtr)),
                                                            vidLogger, SLOT(receiveData(DataFrame::SPtr)));
                                    //check display -> logger
                                    if(!succ)qWarning("ModuleChainFactory::fabricateModuleChain error while connect the moduls");


                                    //connect the timer -> source
                                    succ = QObject::connect(timer, SIGNAL(timeout()),
                                                            vidSourc, SLOT(sendYourProcessedData()));
                                    //check
                                    if(!succ)qWarning("ModuleChainFactory::fabricateModuleChain error while connect the moduls");


                                    //make the list
                                    chain->appendModule(vidSourc);
                                    chain->appendModule(vidLogger);

                                    //new way start timer when run is called
                                    chain->addTimerReference(timer);


                break;
            default:  qWarning("ModuleChainFactory::fabricateModuleChain unexpect parameter, DataStreamTypes::TYPE(%i)",datastreamtype);
                break;
            }//end switch datastreamtype
        break; //SHORT end


        case TYPE_EXTRA:
        default:
            qWarning("ModuleChainFactory::fabricateModuleChain unexpect parameter, CHAINTYPE(%i)",chaintype);
            break;
    }

    return chain;
}

