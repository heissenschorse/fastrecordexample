#include "loggervideo.h"

LoggerVideo::LoggerVideo()
{
    m_debugCounter = 0;
    m_frameCounter = 0;
    m_packetsWrittenCounter = 0;
    m_packetGotFrameCounterFalse = 0;
    m_packetDelayCounter = 0;
    m_startThreadFlag = false;
    m_state = ModuleLogger::STOPPED;
    m_fileCounter = 0;
    m_lastFrameTime = 0;
    m_picCounterRelativ = 0;
    m_picCounterGlobal =0;
    m_picSaveFlag = false;

    //for input
    if(LOGGER_VIDEO_INPUT_1920_1080_BGR24)
    {
        m_sol_x = 1920;
        m_sol_y = 1080;
    }
    else if (LOGGER_VIDEO_INPUT_1920_1200_BGR24)
    {
        //for test and generateDebugData
        m_sol_x = 1920;
        m_sol_y = 1200;
    }
    else
    {
        //for test and generateDebugData
        m_sol_x = 352;
        m_sol_y = 288;
    }

    //alloc a field for the data name, max 100 chars !
    m_outFileName = (char*) malloc(sizeof(char) * LOGGER_VIDEO_FILENAME_MAX_CHARS);
    m_outFileNameLength = LOGGER_VIDEO_FILENAME_MAX_CHARS;
    for(int i=0;i<LOGGER_VIDEO_FILENAME_MAX_CHARS;i++) //fill whole array with line ends
        m_outFileName[i] = '\n';

    //for debug msg
    m_personalID = qrand() %1000; //todo pretend conflicts

#ifdef LOGGER_USE_FFMPEG

    //register all
    av_register_all();

#endif

}

void LoggerVideo::saveNext10Pictures()
{
    m_picCounterRelativ = 0;
    m_picSaveFlag = true;
}

void LoggerVideo::setVideoOutputFileName(QString name)
{

    if(LOGGER_VIDEO_DEBUG_MODE)qDebug()<<"LoggerVideo::setVideoOutputFileName name is:"<<name;

    //test name length
    if(name.length()>LOGGER_VIDEO_FILENAME_MAX_CHARS)
    {
        qWarning()<<"LoggerVideo::setVideoOutputFileName name is to long, will abort whole process";
        return;
    }
    m_fileNameVideoReadable = name;
}

const char* LoggerVideo::convertVideoFileName(QString fileName)
{
    const char *nameCharPtr=0;


    QByteArray tmp = fileName.toUtf8();
    nameCharPtr = (char*) tmp.constData(); //name.toStdString().c_str();

    if(LOGGER_VIDEO_DEBUG_MODE)
    {
        qDebug()<<" LoggerVideo::convertVideoFileName "<<fileName;
        //for(int i=0;i<25;i++)printf("%c",nameCharPtr[i]);
        //printf("\n");
        for(int i=0;i<fileName.length();i++)
            qDebug()<<i<<":"<<nameCharPtr[i];

    }


    return nameCharPtr;
}

bool LoggerVideo::initFFmpeg()
{
#ifdef LOGGER_USE_FFMPEG

    //m_codecID = AV_CODEC_ID_MPEG2VIDEO; //
    m_codecID =  AV_CODEC_ID_H264;
    //m_outFileName ="test.avi";
    m_outFileType = "avi";

   //search for codec
    m_encoder = avcodec_find_encoder( m_codecID);
    if (!m_encoder) {
        m_state = ModuleLogger::UNDEFINED;
        qWarning() <<  "LoggerVideo::init error #"<<1 <<" Codec not found";
        return false;
    }

    //alloc context
    m_codecContext = avcodec_alloc_context3(m_encoder);
    if (!m_codecContext) {
        m_state = ModuleLogger::UNDEFINED;
        qWarning() <<  "LoggerVideo::init error #"<<2 <<" Could not allocate video codec context";
        return false;
    }

    //construct output container
    if(avformat_alloc_output_context2(&m_outContainer, NULL, NULL,  m_fileNameVideoReadable.toUtf8().constData()) < 0)
    {
        m_state = ModuleLogger::UNDEFINED;
        qWarning() <<  "LoggerVideo::init  error #"<<3 <<" while alloc output AVFormat context";
        qWarning() <<  "LoggerVideo::init  error #"<<3 <<" solution width("<<m_sol_x<<") high("<<m_sol_y<<") ";
        return false;
    }

    //construct output stream
    m_outStream = avformat_new_stream(m_outContainer, m_encoder);
    if(!m_outStream)
    {
        m_state = ModuleLogger::UNDEFINED;
        qWarning() <<  "LoggerVideo::init error #"<<4<<" while alloc output context";
        return false;
    }

    //get default values for encoder
    if(avcodec_get_context_defaults3( m_outStream->codec, m_encoder)<0)
    {
        m_state = ModuleLogger::UNDEFINED;
        qWarning() <<  "LoggerVideo::init error #"<<5 <<" set default parameter for encoder";
        return false;
     }

    if(LOGGER_VIDEO_DEBUG_MODE)qDebug()<< "LoggerVideo::initFFmpeg() " << " will configure encoder";

    // configure encoder ***

    //default from  video_encode_example
    m_outStream->codec->bit_rate = 400000;
    /* resolution must be a multiple of two */
    m_outStream->codec->width = m_sol_x;
    m_outStream->codec->height = m_sol_y;
    /* frames per second */
    // m_outStream->codec->time_base= (AVRational){1,25};
    m_outStream->codec->time_base.den = 100; // not 1 / FPS  // old version 25
    m_outStream->codec->time_base.num = 1;
    m_outStream->codec->gop_size = 10; /* emit one intra frame every ten frames */
    m_outStream->codec->max_b_frames=1;
    m_outStream->codec->pix_fmt = AV_PIX_FMT_YUV420P; //for debug
    //PIX_FMT_YUV420P


    m_outStream->codec->max_b_frames = 3;//16; // maximum number of B-frames between non-B-frames

    //what does this ?
    if(m_outContainer->oformat->flags & AVFMT_GLOBALHEADER)
        m_outStream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER; // Place global headers in extradata instead of every keyframe.

    //speed up ?
    //m_outStream->codec->flags2 |= CODEC_FLAG2_FAST;

    /*
     * @see http://ffmpeg.org/trac/ffmpeg/wiki/x264EncodingGuide
     * Choose a preset as option
     *******************

A preset is a collection of options that will provide a certain encoding speed to compression ratio. A slower preset will provide better compression (compression is quality per filesize). This means that, for example, if you target a certain file size or constant bit rate, you will achieve better quality with a slower preset. Similarly, for constant quality encoding, you will simply save bitrate by choosing a slower preset.

The general guideline is to use the slowest preset that you have patience for. Current presets in descending order of speed are: ultrafast,superfast, veryfast, faster, fast, medium, slow, slower, veryslow, placebo. The default preset is medium. Ignore placebo as it is not useful (see FAQ). You can see a list of current presets with -preset help, and what settings they apply with x264 --fullhelp.

You can optionally use -tune to change settings based upon the specifics of your input. Current tunings include: film, animation, grain, stillimage, psnr, ssim, fastdecode, zerolatency. For example, if your input is animation then use the animation tuning, or if you want to preserve grain then use the grain tuning. If you are unsure of what to use or your input does not match any of tunings then omit the -tune option. You can see a list of current tunings with -tune help, and what settings they apply with x264 --fullhelp.

Another optional setting is -profile:v which will limit the output to a specific H.264 profile. This can generally be omitted unless the target device only supports a certain profile (see Compatibility). Current profiles include: baseline, main, high, high10, high422, high444. Note that usage of -profile:v is incompatible with lossless encoding.

As a shortcut, you can also list all possible internal presets/tunes for FFmpeg by specifying no preset or tune option at all:

ffmpeg -i input -c:v libx264 -preset -tune dummy.mp4
     *
     **/

    //set encoder preset
    if(m_codecID == AV_CODEC_ID_H264)
    {

        if(LOGGER_VIDEO_INPUT_QUALITY_BEST)
        {
            av_opt_set( m_outStream->codec->priv_data, "preset", "slow", 0);
            av_opt_set( m_outStream->codec->priv_data, "tune", "ssim", 0);
        }
        if(LOGGER_VIDEO_INPUT_QUALITY_LESS_GOOD)
        {
            av_opt_set( m_outStream->codec->priv_data, "preset", "fast", 0);
            av_opt_set( m_outStream->codec->priv_data, "tune", "ssim", 0);
        }
        if(LOGGER_VIDEO_INPUT_QUALITY_WORST)
        {
            av_opt_set( m_outStream->codec->priv_data, "preset", "ultrafast", 0);
            av_opt_set( m_outStream->codec->priv_data, "tune", "ssim", 0);
        }
        if(LOGGER_VIDEO_INPUT_QUALITY_TEST)
        {
            av_opt_set( m_outStream->codec->priv_data, "profile", "high", 0);
            av_opt_set( m_outStream->codec->priv_data, "preset", "slower", 0);
            //tune options: @see http://mewiki.project357.com/wiki/X264_Settings
            //zerolatency : is shit , just grey video
            // animation : looks good, better on moving pictures than no tune flag
            // grain :looks good also while moving, cost on moving moments more cpu performance
            // ssim  : very fast , cpu flag used, good motions ?
            av_opt_set( m_outStream->codec->priv_data, "tune", "ssim", 0);
        }

    }
     if(LOGGER_VIDEO_DEBUG_MODE)qDebug()<< "LoggerVideo::initFFmpeg() " << " will start encoder";


    //** Open encoder
   int ret = avcodec_open2( m_outStream->codec, m_encoder, NULL);
    if( ret < 0)
    {
        m_state = ModuleLogger::UNDEFINED;
        qWarning() <<  "LoggerVideo::init error #"<<6 <<" while open encoder, errorCode " << ret ;
        qWarning() <<  "LoggerVideo::init error #"<<6 <<" width("<<m_sol_x<<") high"<<m_sol_y<<") pix_fmb("<<m_outStream->codec->pix_fmt<<")";

        return false;
    }

    if(LOGGER_VIDEO_DEBUG_MODE)qDebug()<< "LoggerVideo::initFFmpeg() " << " will open avio";

    // Open output container
    if(avio_open(&m_outContainer->pb, m_fileNameVideoReadable.toUtf8().constData(), AVIO_FLAG_WRITE) < 0)
    {

        m_state = ModuleLogger::UNDEFINED;
        qWarning() <<  "LoggerVideo::init error #"<<7 <<" open container file";
        return false;
    }

    if(LOGGER_VIDEO_DEBUG_MODE)qDebug()<< "LoggerVideo::initFFmpeg() " << " will write av header";

    //Write header to ouput container
    ret = avformat_write_header(m_outContainer, NULL);
    if( ret < 0)
    {
        m_state = ModuleLogger::UNDEFINED;
        qWarning() <<  "LoggerVideo::init error #"<<8<<" while write av format header" << ret << " tag";
        return false;

    }

    if(LOGGER_VIDEO_DEBUG_MODE)qDebug()<< "LoggerVideo::initFFmpeg() " << " will init AVframe";
    //init the encodeFrame
    m_encodeFrame = avcodec_alloc_frame();
    if (!m_encodeFrame) {

        m_state = ModuleLogger::UNDEFINED;
        qWarning() <<  "LoggerVideo::init error #9 alloc frame ";
        return false;
    }
    //set format
    m_encodeFrame->format = AV_PIX_FMT_YUV420P;
    m_encodeFrame->width  =  m_outStream->codec->width;
    m_encodeFrame->height =  m_outStream->codec->height;

    /* the image can be allocated by any means and av_image_alloc() is
                                                           * just the most convenient way if av_malloc() is to be used */
    ret = av_image_alloc(m_encodeFrame->data, m_encodeFrame->linesize,  m_outStream->codec->width,  m_outStream->codec->height,
                          m_outStream->codec->pix_fmt, 32);
    if (ret < 0) {
        qWarning() <<  "LoggerVideo::init error  #10 could not allocate raw picture buffer";
        return false;
    }

#else
    qDebug()<<" LoggerVideo::initFFmpeg() does nothing !! use define LOGGER_VIDEO_VERSION ";
    return false;
#endif

   m_state = ModuleLogger::INITIALIZED;

   return true;
}

#ifdef LOGGER_USE_FFMPEG

bool LoggerVideo::writeRestOfPakets()
{

    if(m_state!=ModuleLogger::UNDEFINED)
    {
        AVPacket pkt;
        av_init_packet(&pkt);
        pkt.data = NULL;    // packet data will be allocated by the encoder
        pkt.size = 0;
        int got_output = 0;
        int i=0;

    /* get the delayed frames */
    //only calles 1 time --> no more like a internal queue
    for (got_output = 1; got_output; i++) {
        //fflush(stdout);
        //if(LOGGER_VIDEO_DEBUG_MODE) qDebug()<<" LoggerVideo::generateDebugData() #8.5 + "<< i << " loop delay frames, m_packetDelayCounter("<< m_packetDelayCounter++<<") ";
         int ret = avcodec_encode_video2( m_outStream->codec, &pkt, NULL, &got_output);
        if (ret < 0) {
            qDebug("LoggerVideo::generateDebugData, delayed:  Error encoding frame #9");
            return false;
        }

        if (got_output) {
            if(LOGGER_VIDEO_DEBUG_MODE)qDebug("Write frame %3d (size=%5i) on delay loop", (int) m_packetsWrittenCounter, pkt.size);
            if(LOGGER_VIDEO_DEBUG_MODE)fflush(stdout);

            if(av_interleaved_write_frame(m_outContainer, &pkt) < 0)
            {
                qDebug("LoggerVideo::generateDebugData # error while write frame #10");
                return false;
            }
            else m_packetsWrittenCounter++;

            //here write to dictionairy


            av_free_packet(&pkt);
        }
    }//delayed frames for end
    }//end state check


    return true;
}

#endif


DataStreamTypes::TYPE LoggerVideo::getDataStreamType()
{
    return DataStreamTypes::VIDEO;
}


void LoggerVideo::receiveData(DataFrame::SPtr dataFrame)
{

    //qDebug("LoggerVideo::receiveData called");
    if(m_state == ModuleLogger::RECORDING)
    {


//SAVE as h264 with ffmepg
#ifdef LOGGER_USE_FFMPEG

        //***** normal way *****
        //qDebug("LoggerVideo::receiveData will save data with ffmpeg software encoder");

        QTime timer;
        timer.start();

        AVPacket pkt;
        int ret = -1;
        int got_output = 0;

        //init the package
        av_init_packet(&pkt);
        pkt.data = NULL;    // packet data will be allocated by the encoder
        pkt.size = 0;

        //get the data
        DataVideo::SPtr dataVideo;
        if(dataFrame->getSize()>=1)
            dataVideo = dataFrame->getQueueElementAsVideoAt(0);
        else{
            if(dataFrame->getSize()==0)
            {
                 qWarning()<<"LoggerVideo::receiveData found  NO data in DataFrame, will abort";
                 return;
            }
            if(dataFrame->getSize()>1 )
                qWarning()<<"LoggerVideo::receiveData found "<< dataFrame->getSize() << "in DataFrame, some data will be lost";
        }
        //get pointer
        //uchar* ptrDataReceived = dataVideo.data()->getPointerToBuff();

        QImage myImage = dataVideo.data()->getAsImage();


        //try to save data as a pic ****************
        if(m_picSaveFlag)
        {
            if(m_picCounterRelativ<10)
            {
                //save image
                QString fileName = QString("pic%1.bmp").arg(m_picCounterGlobal);
                QFile *filePic= new QFile((const QString) fileName);

                if(filePic==0) qWarning("LoggerVideo::receiveData() error while construct new file object for picture");
                // open data
                bool success =  filePic->open(QIODevice::WriteOnly | QIODevice::Text);
                if(!success)qWarning("LoggerVideo::receiveData() error while  error while pic file");

                //transform
                //QImage saveImage((unsigned char*) pic_dat,w,h,QImage::Format_RGB888);
                //myImage.invertPixels(QImage::InvertRgb);
                //QImage saveImage = QImage::fromData(pic_dat,)

                //save
                success = myImage.save(filePic, "BMP",100); //use realy raw format to have best quality
                if(!success)qWarning("LoggerVideo::receiveData() error while write pic file");
                else
                {
                       //count
                        m_picCounterRelativ++;
                        m_picCounterGlobal++;
                }
                //close the file
                filePic->close();
            }
        }//end save picture



        //**** convert image format

        //invert from bgr to rgb 888 pic
        //myImage.invertPixels(QImage::InvertRgb);
        //qDebug()<< "LoggerSensor::receiveData pkt.size " << (int) dataVideo.data()->getSizeOfBuff();

        //@see http://libav-users.943685.n4.nabble.com/Creating-AVPicture-AVFrame-from-QImage-td943939.html
        //convert the color space
        // Getting QImage dimensions
        int w = m_sol_x;
        int h = m_sol_y;

        // Preparing the buffer to get YUV420P data
        int size = avpicture_get_size(AV_PIX_FMT_YUV420P, w, h);
        uint8_t *pic_dat = (uint8_t *) av_malloc(size);

        // Transforming data from RGB to YUV420P
        RGBtoYUV420P(myImage.bits(), pic_dat, myImage.depth()/8, false, w, h, false);

        // Setting up AVFrame
        //AVFrame *picturePtr = avcodec_alloc_frame();
        //picturePtr->quality = 0;
        m_encodeFrame->quality = 0;

        // Filling AVFrame with YUV420P data
        avpicture_fill((AVPicture *)m_encodeFrame, pic_dat,
                           AV_PIX_FMT_YUV420P, w, h);




        //******************** OLD Things *** for calculate the PTS value of the package**********

        //set the pointer to the package , fast way
        //m_encodeFrame->data =   (uint8_t) ptrDataReceived;

        //pkt.data = ptrDataReceived;
        //pkt.size = (int) dataVideo.data()->getSizeOfBuff();

        //copy whole data
        //dataVideo.data()->copyDataToBuff(m_encodeFrame->data, pkt.size);

        //1.
        //see http://stackoverflow.com/questions/6603979/ffmpegavcodec-encode-video-setting-pts-h264
        //Calculate PTS: (1 / FPS) * sample rate * frame number
        //sample rate 90KHz is for h.264 at 30 fps
        //picture->pts = (1.0 / 30) * 90 * frame_count;

        //  pts = (1 / 25 ) * MODULE_UPDATE_FREQUENZY_IN_HZ  *  m_frameCounter

        //2. way calc like in converter, more precisise
        //calc pts **
        // calc the the , new way
//        double pts = 60  *  m_frameCounter;
//        //qDebug()<< "pts: "<< pts << " pts2: "<< pts / 25;
//        pts = pts / 25; // 25 from encoder
//        m_encodeFrame->pts = pts;


        //get time of now
        FrameTimeObject *frameTimeNow =  dataVideo.data()->getFrameTime();

//        //3. way from last frameTime
//        if(m_startLoggingFrameTime==0)
//        {
//            //use fall back version
//            //2. way calc like in converter, more precisise
//            //calc pts **
//            // calc the the , new way
//            double pts = 60  *  m_frameCounter;
//            //qDebug()<< "pts: "<< pts << " pts2: "<< pts / 25;
//            pts = pts / 25; // 25 from encoder
//            m_encodeFrame->pts = pts;
//        }
//        else
        {
            //new version  compare the ms and calculate the frames

            //new way of count seconds
            quint64 diffSeconds =  m_privateSecondCounter;//old frameTimeNow->getTimeUnix() - m_startLoggingFrameTime->getTimeUnix();
            quint64 msNow =  frameTimeNow->getTimeMs();
            quint64 msStart =  m_startLoggingFrameTime->getTimeMs();
            quint64 diffMS = 0;//frameTimeNow->getTimeMs() - m_startLoggingFrameTime->getTimeMs();

            //make absolut difference
            if(msNow>=msStart)
                diffMS = msNow-msStart;
            else
            {
                diffMS = (1000-msStart)+msNow;
                if(diffMS>=1000)
                {
                    diffSeconds++; //add a second
                    diffMS-=1000; //subtract a seconds
                    //qDebug()<<"!!!!!";
                }


            }
            //make a comparison to last ms, when changed down, a second is gone
            if(diffMS<m_msLast)
            {
                m_privateSecondCounter++;
                diffSeconds++;
                //qDebug()<<"##### "<<m_privateSecondCounter;
            }

            //remember last
            m_msLast = diffMS;
            //chase timeBaseUnit
            //  outStream->codec->time_base.den = 100; //change to ms , was 25 before
            //outStream->codec->time_base.num = 1;
            //@see line 116
            //old
            //double timeBaseUnit = 0.04; //time base unit 1 / FPS = 1 / 25 here  used a higher resolution !
//            quint64 msSinceStart =(quint64) diffSeconds * (quint64) 1000 + (quint64) diffMS;

//            //version change time base unit to ms 10 =  1 / 100 @ see line   outStream->codec->time_base.den
//            quint64 pts = (quint64)  msSinceStart / (quint64 ) 10;


            //newest pretend overflow
            quint64 msSinceStart =(quint64) diffSeconds * (quint64) 100;
            quint64 diffMSBroke = diffMS/(quint64)10;
            quint64 pts = msSinceStart + diffMSBroke;


//            //perform test
//            if(pts>60000)
//            {
//                qWarning()<< "error! LoggerVideo::receiveData pts error !! " << pts << " will use the old way" ;
//                double pts = 60  *  m_frameCounter;
//                //qDebug()<< "pts: "<< pts << " pts2: "<< pts / 25;
//                pts = pts / 25; // 25 from encoder
//                m_encodeFrame->pts = pts;

//            }
//            else
            //test if not montonic , force it
            if(m_ptsLast>=pts)
            {
                //make it monotic by force
                qWarning()<<"LoggerVideo::receiveData"<<m_personalID<<" error: force pts to monotonic:"<<m_ptsLast<<">="<<pts;
                pts = m_ptsLast+10;
            }
            //else
            //write pts to packet
            m_encodeFrame->pts  =pts;

            //qDebug()<<m_personalID<<"| pts: "<<pts << " | old: " << m_ptsLast << "| diff(s|ms): "<< diffSeconds << "|"<<diffMS<<" | msSta:"<<msStart<<"|msNow:"<<msNow <<"\n";
            //save pts
            m_ptsLast = pts;

        }

        //remember the frameTime
        m_lastFrameTime =frameTimeNow;

        //count frame
        m_frameCounter++;

//        if(m_timeline->getList().size()<=m_frameCounter)
//            {qWarning()<<"UtilVideoTimeConverter::convert error #13.5";return false;}

//        //get pts time from frameCounter
//        FrameTimeObject *starTime = m_timeline->getFirst();
//        FrameTimeObject *currentTime = m_timeline->getList().at(m_frameCounter);

//        // http://stackoverflow.com/questions/13595288/understanding-pts-and-dts-in-video-frames

//        //3. way calc diff
//        quint64 diffSeconds = currentTime->getTimeUnix() - starTime->getTimeUnix();
//        quint64 diffMS = currentTime->getTimeMs() - starTime->getTimeMs();

//        double timeBaseUnit = 0.04; //time base unit 1 / FPS = 1 / 25 here
//        quint64 msSinceStart = diffSeconds * 1000 + diffMS;

        //general their should be sample rate at 90 kHz
        //m_encodeFrame->pts = (1 / 25 ) * 90  *  m_frameCounter;


        //old way
        //m_encodeFrame->pts = m_frameCounter++; //for strictly-monotonic PTS
        //m_encodeFrame->display_picture_number = m_frameCounter++;


        //if(LOGGER_VIDEO_DEBUG_MODE)qDebug()<<" LoggerVideo::generateDebugData() #3  before encode, ";

        /* encode the image */
        ret = avcodec_encode_video2( m_outStream->codec, &pkt, m_encodeFrame, &got_output);

        if (ret < 0) {
            qWarning("LoggerVideo::receiveData: error encoding frame, will abort, errorCode %i",ret);

            return;
        }

        //write the dictionary entry
        {

            //ref1
            quint64 ref1 = m_encodeFrame->pts;
            //ref2  framecounter ??  is equal  m_encodeFrame->pts;
            quint64 ref2 = m_frameCounter;

            QTextStream outDic(m_fileDic);

            //build dic string
            QString foo;
            foo.append(QString("%1").arg(m_dicEntryCounter++));
            foo.append(";");
            foo.append(dataFrame->getStartFrameTime()->toString());
            foo.append(";");
            foo.append(QString("%1").arg(ref1).toLatin1());//(quint64)ref;
            foo.append(";");
            foo.append( QString("%1").arg(ref2).toLatin1());//(quint64) framecounter;
            foo.append("\n");
            //qDebug()<< "LoggerVideo::receiveData dic entry: " << foo;
            outDic << foo.toLatin1();

        }
        //qDebug()<<" LoggerVideo::receiveData() #3 + "<< i << " after encode "<< got_output;

        //got frame flag, say this image is ready encoded , soo it could be written
        if (got_output) {
            //if(LOGGER_VIDEO_DEBUG_MODE)
            //qDebug("LoggerVideo::receiveData: write frame %3d (size=%5d)", (int) m_packetsWrittenCounter, pkt.size);

            //set the stream index
            pkt.stream_index = m_outStream->index;

            //if(LOGGER_VIDEO_DEBUG_MODE)qDebug()<< "LoggerVideo::receiveData() #4  will calc the PTS and DTS value ";

            //calc the PTS and DTS value
            if (pkt.pts != AV_NOPTS_VALUE)
            {
                //qDebug()<< "LoggerVideo::receiveData: #1 pts time: "<< pkt.pts ;
                pkt.pts =  av_rescale_q(pkt.pts, m_outStream->codec->time_base, m_outStream->time_base);
                //qDebug()<< "LoggerVideo::receiveData: #2 pts time: "<< pkt.pts ;
            }
            if (pkt.dts != AV_NOPTS_VALUE)
                pkt.dts = av_rescale_q(pkt.dts, m_outStream->codec->time_base, m_outStream->time_base);


            if(LOGGER_VIDEO_DEBUG_MODE) qDebug()<<"LoggerVideo::receiveData() #5 will write frame ";

            ret = av_interleaved_write_frame(m_outContainer, &pkt);
            if( ret< 0)
            {
                if(LOGGER_VIDEO_DEBUG_MODE)qDebug("LoggerVideo::receiveData #6 error while write frame, with ErrorCode %i",ret);
                return;
            }
            else m_packetsWrittenCounter++;


            if(LOGGER_VIDEO_DEBUG_MODE)qDebug()<<"LoggerVideo::receiveData() #7 will write done ";

            av_free(pic_dat);
            av_free_packet(&pkt);

            //
            //if(LOGGER_VIDEO_DEBUG_MODE)
            //qDebug()<<"LoggerVideo("<<m_personalID<<")::receiveData() take "<< timer.elapsed() <<" ms";

        }//end if got a frame
        else {
            if(LOGGER_VIDEO_DEBUG_MODE)qDebug()<<"LoggerVideo::receiveData() #8 gotFrameFlag: false  m_packetGotFrameCounterFalse("<<m_packetGotFrameCounterFalse++  <<"), will write on delay loop  ";
        }


#endif

        //SAVE IN FILE
        //**************
//        DataVideo::SPtr vidData = Data::getAsVideo(d);

//        //qDebug("LoggerVideo::receiveData will save to file");
//        QString fileName(UtilFactoryDataFileName::getDatePattern());
//        fileName.append(QString("-grab.%1.png").arg(m_debugCounter++));

//        //save it to file
//        QFile file(fileName);
//        if (!file.open(QIODevice::WriteOnly))
//        {

//            qWarning(" LoggerVideo::receiveData error while open a file");
//        }
//        else
//        {

//            qDebug("LoggerVideo::receiveData will save the picture to file");

//            vidData->getAsImage().save(&file,"PNG");

//            //close file
//            file.close();
//        }


        //qDebug("LoggerVideo::receiveData  use FFMEPG in  Version %d (prebuild see http://ffmpeg.zeranoe.com/builds/) ",(double) LOGGER_FFMEPEG_VERSION ) ;

        //! @see ffmepg doc/examples/scaling_video.c






    }//if recording state

    //take care about the deletation
//    DataVideo::SPtr vidData = d->getQueueElementAsVideoAt(0); //should be one in the frame
//    DataVideo *ptrDat =  vidData.data();

    //delete ptrDat; //delete in a hard way

    //d.clear();
    //delete d;
}

#ifdef LOGGER_USE_FFMPEG
/**
 * RGBtoYUV420P function is from Gnomemeeting
 */
#define rgbtoyuv(r, g, b, y, u, v) \
  y=(uint8_t)(((int)30*r   +(int)59*g +(int)11*b)/100); \
  u=(uint8_t)(((int)-17*r  -(int)33*g +(int)50*b+12800)/100); \
  v=(uint8_t)(((int)50*r   -(int)42*g -(int)8*b+12800)/100);
void LoggerVideo::RGBtoYUV420P ( const uint8_t *pRGB, uint8_t *pYUV, uint iRGBIncrement, bool bSwapRGB, int iWidth, int iHeight,  bool bFlip )
{
  const unsigned iPlaneSize = iWidth * iHeight;
  const unsigned iHalfWidth = iWidth >> 1;

  // get pointers to the data
  uint8_t *yplane  = pYUV;
  uint8_t *uplane  = pYUV + iPlaneSize;
  uint8_t *vplane  = pYUV + iPlaneSize + (iPlaneSize >> 2);
  const uint8_t *pRGBIndex = pRGB;
  int iRGBIdx[3];
  iRGBIdx[0] = 0;
  iRGBIdx[1] = 1;
  iRGBIdx[2] = 2;
  if ( bSwapRGB )  {
    iRGBIdx[0] = 2;
    iRGBIdx[2] = 0;
  }

  for (int y = 0; y < (int) iHeight; y++) {
    uint8_t *yline  = yplane + (y * iWidth);
    uint8_t *uline  = uplane + ((y >> 1) * iHalfWidth);
    uint8_t *vline  = vplane + ((y >> 1) * iHalfWidth);

    if ( bFlip ) // Flip horizontally
      pRGBIndex = pRGB + ( iWidth * ( iHeight -1 -y ) * iRGBIncrement );

    for ( int x=0; x<iWidth; x+=2 ) {
      rgbtoyuv ( pRGBIndex[iRGBIdx[0]], pRGBIndex[iRGBIdx[1]], pRGBIndex[iRGBIdx[2]], *yline, *uline, *vline );
      pRGBIndex += iRGBIncrement;
      yline++;
      rgbtoyuv ( pRGBIndex[iRGBIdx[0]], pRGBIndex[iRGBIdx[1]], pRGBIndex[iRGBIdx[2]], *yline, *uline, *vline );
      pRGBIndex += iRGBIncrement;
      yline++;
      uline++;
      vline++;
    }
  }
}
#endif



void LoggerVideo::startLogging()
{

    //start not recording at the moment
    if(m_state!=ModuleLogger::RECORDING)
    {

        qDebug("LoggerVideo::startLogging() will start logging on dev %i",(int) m_personalID);

        //set zero some rakes
        m_ptsLast = 0;
        m_privateSecondCounter = 0;
        m_msLast  =0;

        //mark the frameTime as starting moment
        m_startLoggingFrameTime = FrameTimeObjectProducer::instance()->getFrameTimeObject();
        //doc2variante:
        //QString name = UtilFactoryDataFileName::getVideoFileName(m_personalID);
        //here:
        QString name = m_fileNameVideoReadable;
        if(LOGGER_VIDEO_DEBUG_MODE)qDebug()<<"LoggerVideo::startLogging() will write file  "<<  name;

        if(LOGGER_VIDEO_DEBUG_MODE)qDebug()<<"LoggerVideo::startLogging() count n ? "<< name.contains("\n")<< " length:"<<name.size();

        //count new amount of frames
        m_frameCounter = 0; //set zero if new frames

        //count files
        m_fileCounter++;
        //construct file name in a difficult way
//        int size = name.size();
//        char *filenName = (char*) malloc(sizeof(char) *size +1 );
//        for(int i =0 ;i<size +1 ;i++)filenName[i] = (char) 32; //will with whitespace

//        const char *ptsToString =  name.toUtf8().constData();

//        for(int i=0;i<size;i++)
//            filenName[i] =  ptsToString[i];

//        //copy in a hard way
//        //memcpy((void*) m_outFileName, filenName, 15 );

//        m_outFileName =  filenName; // UtilFactoryDataFileName::getVideoFileName(0,m_fileCounter++);

        //set to zero
        m_dicEntryCounter = 0;


        QTime timer;
        timer.start();

        //init ffmpeg with solution x,y and so

        //try init
        bool succe =  initFFmpeg();
        qDebug()<< "LoggerVideo::startLogging init ffmpeg take "<< timer.elapsed() << " ms";

        if(!succe)
        {
            qWarning()<<" LoggerVideo::startLogging() !! error while initFFFMPEG";


            m_state = ModuleLogger::UNDEFINED;
        }
        else
        {
            //RECORDING will be done

             m_state = ModuleLogger::RECORDING;

             //DICTIONARY

             //dic file
             //doc2 variante
             //m_fileNameDic  = UtilFactoryDataFileName::getVideoDicFileName(m_personalID);
             m_fileNameDic = "videoDic.txt";
             m_fileDic = new QFile((const QString) m_fileNameDic);


             if(m_fileDic==0) qWarning("LoggerVideo::startLogging() error while construct new file object");
             // open data
             bool success =  m_fileDic->open(QIODevice::WriteOnly | QIODevice::Text);
             if(!success)qWarning("LoggerVideo::startLogging() error while open the dictionary file");

             //write header
             writeDicFileHeader();

             //count file numbers written
             m_fileCounter++;

//             m_timer  = new QTimer();
//             m_timer->start();
         }


    }
    else qDebug("LoggerVideo::startLogging() already recording");

}


void LoggerVideo::writeDicFileHeader()
{
    QTextStream out(m_fileDic);
    out << "#dictionary file video";
    out << "######################################### \n";
    out << "#type : avi \n";
    //out << "#frequenzy(hz) tool chain: " << MODULE_UPDATE_FREQUENZY_IN_HZ << " \n";
//TODO write  sample rate usw.
    out << "######################################### \n";
    out << "#ref1: is the frameCounter (intern) from class FrameTimeObjectProducer  \n",
    out << "#ref2: is the pts values of the image \n";
    out << "#ref3: is the framecoutner\n";
    out << "# nr. entry; unix timestamp ;  milli seconds; ref1 ; ref2; ref3\n";
    out << "####################################################### \n";
}

void LoggerVideo::stoppLogging()
{
    if(m_state!=ModuleLogger::STOPPED)
    {

        //close and free all ffmpeg things
    #ifdef LOGGER_USE_FFMPEG

        if(m_state!= ModuleLogger::UNDEFINED )
        {
            //write the rest
            writeRestOfPakets();

            //free frame
            avcodec_free_frame(&m_encodeFrame);

            qDebug("LoggerVideo::stoppLogging() free ffmpeg stuff");

            av_write_trailer(m_outContainer);
            avio_close(m_outContainer->pb);
            avformat_free_context(m_outContainer);

            //m_streamDescription.clear();

            //Free dic file pointer
            //dic file
            m_fileDic->close();
            free(m_fileDic);

        }
    #endif

        m_state = ModuleLogger::STOPPED;

        //stop the timer
        //m_timer->stop();

    }
    else qDebug("LoggerVideo::stoppLogging() already stopped");
}

