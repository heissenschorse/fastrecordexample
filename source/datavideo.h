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


#ifndef DATAVIDEO_H
#define DATAVIDEO_H



//qt
#include <QString>
#include <QImage>
//#include <QByteArray>

//own
#include "data.h"

//solution is dynamic !!

//#define DATAVIDEO_SOLUTION_X 1920
//#define DATAVIDEO_SOLUTION_Y 1080  // storz image hub
//#define DATAVIDEO_SOLUTION_Y 1200  // i Mac
#define DATAVIDEO_BAND 3

class DataVideo : public Data
{
public:
    //typedef
    typedef QSharedPointer<DataVideo> SPtr;
    typedef QSharedPointer<const DataVideo> ConstSPtr;

    //! default constructor
    DataVideo(uint heigt, uint width, FrameTimeObject *frameTime);

    virtual ~DataVideo();

//    //! other, load the the image
//    DataVideo(uchar * data, int width, int height, QImage::Format format);

    //! @see Data::getDataType
    Data::TYPE getDataType();

//    //! add a image
//    void addImage(QImage *img);

    //! get pointer to first element
    //! its for adding data
    uchar* getPointerToBuff();

    //! return size of the buffer
    quint64 getSizeOfBuff();

    //! return true if one of the elements is Null, otherwise false
    bool isBuffNull();

    //! get the saved buffer as Qimage
    QImage getAsImage();

    //! copy the buff data to dest
    void copyDataToBuff(void* dest,int byteAmount);

   // //!  get the saved image as const QImage &image

    //! return the image as String
    QString toString();
private:

    //!init stuff
    void init(uint heigt, uint width);

    //! save a picture
    QImage *m_image;

    //! buffer of 1920 * 1080 * 3 byte
    uchar *m_rawDataBuffer;

    uint m_heigt;
    uint m_width;

};

#endif // DATAVIDEO_H
