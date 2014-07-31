#include "module.h"

Module::Module()
{
}


Module::TYPE Module::getModuleType()
{
    return BASECLASS;
}

DataStreamTypes::TYPE Module::getDataStreamType()
{
    return DataStreamTypes::UNDEFINED;
}
