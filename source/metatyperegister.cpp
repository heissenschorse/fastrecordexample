#include "metatyperegister.h"



void MetaTypeRegister::registerAllTyps()
{
    int id  =0;

    //dataframe shared pointer
    id= qRegisterMetaType<DataFrame::SPtr>("DataFrame::SPtr");
    //test register
    if(!QMetaType::isRegistered(id))qDebug()<< "MetaTypeRegister: error while register" <<id ;

    //module source error
    id= qRegisterMetaType<ModuleSource::MODERROR>("ModuleSource::MODERROR");
    //test register
    if(!QMetaType::isRegistered(id))qDebug()<< "MetaTypeRegister: error while register" <<id ;

}
