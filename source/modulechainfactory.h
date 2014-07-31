/*

The MIT License (MIT)

Copyright (c) 2014 Thomas Boy, HTWK Leipzig, university of applied sciences,
European Social Fund (ESF) prototed the project: TOPINUS

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/


#ifndef MODULECHAINFACTORY_H
#define MODULECHAINFACTORY_H



//TODO initAllDevices differ subtypes of device, each device must differ sourcemodul !

//qt
#include <QObject>
#include <QMutex>
#include <QTimer>

//own
#include "module.h"
#include "modulechain.h"
#include "modulesource.h"
#include "sourcevideo.h"
#include "moduledisplay.h"
#include "displayvideo.h"
#include "modulelogger.h"
#include "loggervideo.h"
#include "datastreamtypes.h"


//Define
//! is the value how often the complete chain is updated
#define MODULE_UPDATE_FREQUENZY_IN_HZ 45

//! singelton, should be used to fabricate a ToolChain
class ModuleChainFactory
{
public:

    //! the chain types
    enum CHAINTYPE
    {
        //! produce a normal moduleChain, normal is source->display->logger
        TYPE_NORMAL,
        //! a small module, source -> logger
        TYPE_SHORT,
        //! a player module chain player as "source"
        TYPE_PLAYER,
        //! extras ordinary
        TYPE_EXTRA
    };

    //! function to get the instance
    static ModuleChainFactory* instance();

    //! function to destroy the instance
    static void drop();

    //! return a ModuleChainFactory
    //! @warning must be freed at one time
    static ModuleChain* fabricateModuleChain(CHAINTYPE chaintype, DataStreamTypes::TYPE datastreamtype, Device *dev);

private:

    //! the one instance witch can exist
    static ModuleChainFactory *m_instance;

    //! private
    ModuleChainFactory();

    //! hide copy constructor
    ModuleChainFactory(const ModuleChainFactory&);

    //! override the copy constructor
    ModuleChainFactory& operator=(const ModuleChainFactory &); // hide assign op
                         // we leave just the declarations, so the compiler will warn us
                         // if we try to use those two functions by accident
};

#endif // MODULECHAINFACTORY_H
