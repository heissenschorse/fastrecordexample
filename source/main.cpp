/*

The MIT License (MIT)

Copyright (c) 2014 Thomas Boy, htwk leipzig university of applied sciences, project: TOPINUS
promoted of European Social Fund (ESF)

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

//qt
#include <QApplication>

//own
#include "mainwindow.h"
#include "modulechainfactory.h"
#include "cammatroxorionhd.h"
#include "metatyperegister.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //call singelton to register needed type for signals and slots
    MetaTypeRegister::registerAllTyps();

    //dummy main window
    MainWindow widgetMain;

    //get ref to display
    DisplayVideo *myDisplayModul = widgetMain.getRefToDisplay();

    //construct the device, witch should be used
    DeviceCamera *myDevCamHD = new CamMatroxOrionHD();
    myDevCamHD->setName(QString("matrox orion hd DVI input"));
    myDevCamHD->setCamSort(DeviceCamera::CAM_SORT_MATROX_ORION_HD_DVI_INPUT);

    //construct modul chain
    //type short has the moduls: source + logger from video
    ModuleChain *myVideoChain = ModuleChainFactory::instance()->fabricateModuleChain(ModuleChainFactory::TYPE_SHORT,DataStreamTypes::VIDEO,myDevCamHD);

    //**** what is also existing in the complete doc2 source base ?
    //    ModuleChain *mySensorChain = ModuleChainFactory::instance()->fabricateModuleChain(ModuleChainFactory::TYPE_NORMAL,DataStreamTypes::SENSOR);
    //    ModuleChain *myAudioChain = ModuleChainFactory::instance()->fabricateModuleChain(ModuleChainFactory::TYPE_NORMAL,DataStreamTypes::AUDIO);

    //append the display from main window
    bool success =myVideoChain->appendModulToSource(myDisplayModul);
        if(!success)qWarning()<<"!! error while append modul display to video chain";


    //make some output for debug
    myVideoChain->toString();

    //get loggerModul of chain to start capture
    ModuleLogger *myLoggerModul = myVideoChain->getModuleLogger();

    //connect with button from form
    success = QObject::connect(&widgetMain, SIGNAL(signalStartLoggingClicked()),myLoggerModul, SLOT(startLogging()));
    if(!success)qWarning("!! error while connect start button");

    success = QObject::connect(&widgetMain, SIGNAL(signalStopLoggingClicked()),myLoggerModul, SLOT(stoppLogging()));
    if(!success)qWarning("!! error while connect stop button");


    //example influence more specific modul
    //cast more down to use set file name
    if(myLoggerModul->getDataStreamType() == DataStreamTypes::VIDEO)
    {

        LoggerVideo *myVideoLogger = (LoggerVideo*) myLoggerModul;

        //connect main widget and logger modul
        success = QObject::connect(&widgetMain, SIGNAL(signalFileNameChanged(QString)),myVideoLogger, SLOT(setVideoOutputFileName(QString)),Qt::DirectConnection);

        if(!success)qWarning("!! error while connect file name line edit field");
        //set the standard filename, needed for the first time
        myVideoLogger->setVideoOutputFileName(QString("test0815.avi"));//why here? @see MainWindow function on_lineEdit_textChanged

        success = QObject::connect(&widgetMain, SIGNAL(signalSaveNext10Pics()),myVideoLogger, SLOT(saveNext10Pictures()),Qt::DirectConnection);
        if(!success)qWarning("!! error while connect signalSaveNext10Pics ");


    }else qWarning("!! error cast ModuleLogger to LoggerVideo modul");

    //start the modul chain
    myVideoChain->startModuleChain();

    //call fflush to get all output on stout
    fflush(stdout);

    widgetMain.show();

    return a.exec();
}
