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

#ifndef MODULECHAIN_H
#define MODULECHAIN_H

//TODO: toString better a String back then stdout
 //TODO startModuleChain, start other modules
//TODO: make a type check , is every modul of the same DataStreamTypes ?

//qt
#include <QList>
#include <QDebug>
#include <QTimer>

// own
#include "module.h"
#include "modulesource.h"
#include "modulelogger.h"
#include "moduledisplay.h"


//! this is NOT a module, its a container for a list of modules
class ModuleChain
{
public:

    //! a pointer list
    typedef QList<ModuleChain*> List;

    //! empty constructor
    ModuleChain();

    //! insert a module at a specific position
    void insertModuleAtPosition(int index, Module *m);

    //! append the Module to the chain
    void appendModule(Module *m);

    //! check the right order
    //! @return true if its normal
    bool validateOrder();

    //! destroy the chain and all moduls
    void freeTheChain();

    //! get the the head of the list, is the source
    ModuleSource* getSourceModule();

    //! add a ModuleDisplay to Source
    bool appendModulToSource(ModuleDisplay *modDispla);

    //! get LoggerModule in the Chain
    //! @return 0 if there is not logger in the chain
    //! @return return the first logger in the list
    ModuleLogger* getModuleLogger();

    //! get Display Modul in the Chain
    //! @return 0 if there is not logger in the chain
    //! @return return the first dipslay in the list
    ModuleDisplay* getModulDisplay();


    //! return the module at position
    Module* getModuleAt(int index);

    //! for debug, give out on debug console, what is in the chain
    void toString();

    //! start all the moduls which are threads themself like modulesources
    void startModuleChain();

    //! stop the source modul to process anything, stop and free devices
    void stoppModulChain();

    //! stop the source modul to process anything,
    void haltModulChain();

    //! store timer reference for right start up
    void addTimerReference(QTimer *timer);


private:

    //! the private module list
    QList<Module*> m_modList;


    //! get index of module
    //! @warning doesnt work
    //! TODO
    int getIndexOfModul(Module *mod);

    //! timer ref for starting pull
    QTimer *m_timer;
};

#endif // MODULECHAIN_H
