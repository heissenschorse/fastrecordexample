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


#ifndef MODULEDISPLAY_H
#define MODULEDISPLAY_H

//TODO:  when this class isn't a widget it should be a child of QObject

//QT
#include <QObject>
#include <QWidget>

//own
#include "module.h"
#include "dataframe.h"

//! modul that is the base class of the controller to the GUI class,
class ModuleDisplay : public QWidget, public Module //first QObject then the rest @see http://doc.qt.digia.com/4.5/moc.html#multiple-inheritance-requires-qobject-to-be-first
{

public:

    //! def a list
    typedef QList<ModuleDisplay*> list;


    //! constructor
    ModuleDisplay();

    //! constructor
    ModuleDisplay(QWidget *parent);

    //! init all widget stuff here
    virtual void init();

    //! @see Module::getModuleType();
    Module::TYPE getModuleType();

    //! get the info, datastream type is used @see DataStreamTypes
    virtual DataStreamTypes::TYPE getDataStreamType();

signals:

    //! modul signal to send Data
    void sendData(DataFrame::SPtr d);

public slots:
    //! modul slot to receive Data , each Modul must have this !
    void receiveData(DataFrame::SPtr d);

};

#endif // MODULEDISPLAY_H
