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


#ifndef MODULE_H
#define MODULE_H

//TODO pretend to have the same function as QThread because the multiple inheritance
//TODO use shared pointer for adressing the moduls !! need refactoring !!

#include <QObject>

//own
#include "datastreamtypes.h"

//! base class for the moduls
//! couldn't  be a child of QObject, because the of themultiple inheritance
//! every modul is a child of this
class Module //: public QObject
{
public:

    enum TYPE
    {
        //! Source Type, Device Layer abstraction Type, @see ModuleSource
        SOURCE,
        //! Display Type, controller to Widget classes, @see ModuleDisplay @see MVC Pattern
        DISPLAY,
        //! Logger Type, save the data to the file system @see ModuleLogger
        LOGGER,
        //! type of player, when record is stored player could be a kind of source
        PLAYER,
        //! BASE Class Type, should never be used, just the base class use this
        BASECLASS
    };

    //! empty constructor
    Module();

    //! get the info, which module type is used
    virtual Module::TYPE getModuleType();

    //! get the info, datastream type is used @see DataStreamTypes
    virtual DataStreamTypes::TYPE getDataStreamType();
};

#endif // MODULE_H
