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

#ifndef LOGGERVIDEO_H
#define LOGGERVIDEO_H


//TODO write test first frame for solution
// to get the parameter for ffmepg init function

//! uncomment to cancel support
#define LOGGER_USE_FFMPEG //use ffmpeg as software encoder

//when it will be used include it the lib
#ifdef LOGGER_USE_FFMPEG
//include ffmpeg
extern "C"
{
#ifndef __STDC_CONSTANT_MACROS
#  define __STDC_CONSTANT_MACROS
#endif
/* The ffmpeg library is completely written in C, so we need to tell the C++ compiler that so it links correctly. */

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/opt.h"
#include "libavutil/common.h"
#include "libavutil/imgutils.h"
#include "libavutil/error.h"

}//end extern c
#endif

//qt
#include <QFile>
#include <QImage>
#include <QProcess>
#include <QStringList>
#include <QTime>

//own
#include "modulelogger.h"
#include "datavideo.h"
#include "dataframe.h"
#include "modulechainfactory.h"


#define LOGGER_VIDEO_VERSION 0.03 //0.03 is special for fast record example

#define LOGGER_VIDEO_DEBUG_MODE 0

#define LOGGER_VIDEO_FILENAME_MAX_CHARS 50
//not used, because solution is get from datavideo
#define LOGGER_VIDEO_INPUT_1920_1080_BGR24 1
#define LOGGER_VIDEO_INPUT_1920_1200_BGR24 0

#define LOGGER_VIDEO_INPUT_QUALITY_BEST 0  //not possible by two video sources
#define LOGGER_VIDEO_INPUT_QUALITY_LESS_GOOD 0 //possible  with 2 video sources
#define LOGGER_VIDEO_INPUT_QUALITY_WORST 0  //working but ugly
#define LOGGER_VIDEO_INPUT_QUALITY_TEST 1   //experimental setting

//! class to store the video files
class LoggerVideo : public ModuleLogger
{
    Q_OBJECT

public:

    LoggerVideo();

    //! get the info, datastream type is used @see DataStreamTypes
    DataStreamTypes::TYPE getDataStreamType(); 

    //! function for ffmpeg init
    bool initFFmpeg();

public slots:

    //! change video file Name this way
    void setVideoOutputFileName(QString name);

    //! will save next 10
    void saveNext10Pictures();

    //! modul slot to receive Data , each Modul must have this !
    void receiveData(DataFrame::SPtr dataFrame);

    //! start writing Data
    //! @warning must be implement from every child !
    void startLogging();

    //! stopp writing Data
    //! @warning must be implement from every child !
    void stoppLogging();



private:
    QFile *m_fileDic;
    QString m_fileNameVideoReadable;
    QString m_fileNameDic;
    quint64 m_dicEntryCounter;
    quint64 m_debugCounter;
    bool m_startThreadFlag;
    quint16 m_fileCounter;
    char *m_outFileName;
    int m_outFileNameLength;
    const char *m_outFileType ;
    quint64 m_frameCounter;
    quint64 m_packetsWrittenCounter;
    int m_sol_x,m_sol_y;
    int m_packetGotFrameCounterFalse;
    int m_packetDelayCounter;

    //pic save option
    quint64 m_picCounterRelativ;//count all pics since slot called save10Pics
    quint64 m_picCounterGlobal;//count all pics since app start
    bool m_picSaveFlag;

    quint64 m_ptsLast; //for check monotonic pts
    quint64 m_privateSecondCounter; //for test own timing things
    quint64 m_msLast;

    //! for debug
    quint64 m_personalID;

    //!last frame time
    FrameTimeObject *m_lastFrameTime;
    //! when start logging the frameTime, called after init ffmpeg
    FrameTimeObject *m_startLoggingFrameTime;

    //! write Dictionary entry
    void writeDicFileHeader();

    //! convert FilenName to char array
    //! @deprecated
    const char* convertVideoFileName(QString fileName);




#ifdef    LOGGER_USE_FFMPEG
    //AVCodec *m_codec;
    AVCodecContext *m_codecContext;
    AVFormatContext *m_outContainer;
    AVCodec *m_encoder ;
    AVStream *m_outStream;
    AVCodecID m_codecID;
    AVFrame *m_encodeFrame;

    FILE *m_file;
    AVFrame *m_frame;

    //! write the delayed frames to the output container
    bool writeRestOfPakets();

    //! convert color space
    //! RGBtoYUV420P function is from Gnomemeeting
    void RGBtoYUV420P ( const uint8_t *pRGB, uint8_t *pYUV, uint iRGBIncrement, bool bSwapRGB, int iWidth, int iHeight,  bool bFlip );

#endif
};

#endif // LOGGERVIDEO_H
