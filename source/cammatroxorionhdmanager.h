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



#ifndef CAMMATROXORIONHDMANAGER_H
#define CAMMATROXORIONHDMANAGER_H

// qt
#include <QMutex>
#include <QDebug>

//#include "cammatroxorionhd.h"

//uncomment to support the matrox orion hd card
//#define SUPPORT_ORION_HD
//#endif


// when  it will be used include
#ifdef SUPPORT_ORION_HD
//for matrox ordion hd card
#include "../include/cam_orion_hd/mil.h"
#endif

//! singelton for exchange basic ids needed for handling the CamMatroxOrionHD
class CamMatroxOrionHDManager
{
public:

    //! function to get the instance
    static CamMatroxOrionHDManager* instance();

    //! function to destroy the instance
    static void drop();

    //! call once for every cam of this type
    void registerCamMatroixOrionHD();

    //! register the id
    void registerMilIDApp(long long id);

//    void unRegisterMilIDApp();

    void registerMilIDSystem(long long id);

    //void unRegisterMilIDSystem();

    //! free milSystemAndApp, call once per cam
    void freeAPPandSystem();

    long long getMilIDApp();

    long long getMilIDSystem();

private:

    //! the one instance witch can exist
    static CamMatroxOrionHDManager *m_instance;

    //! private
    CamMatroxOrionHDManager();

    //! hide copy constructor
    CamMatroxOrionHDManager(const CamMatroxOrionHDManager&);

    //! override the copy constructor
    CamMatroxOrionHDManager& operator=(const CamMatroxOrionHDManager &); // hide assign op
                         // we leave just the declarations, so the compiler will warn us
                         // if we try to use those two functions by accident

    //save the ids
    long long m_milAppID;
    long long m_milSystemID;
    quint64 m_camCounter;

};

#endif // CAMMATROXORIONHDMANAGER_H
