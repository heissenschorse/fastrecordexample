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

#ifndef DISPLAYVIDEO_H
#define DISPLAYVIDEO_H

#define DISPLAY_VIDEO_DEBUG_MODE 0

//qt
#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QFile>
#include <QLabel>
#include <QTextStream>
#include <QBoxLayout>

//own
#include "moduledisplay.h"
#include "datavideo.h"
#include "dataframe.h"

class DisplayVideo : public ModuleDisplay
{
    Q_OBJECT

public:
    //! constructor
    explicit DisplayVideo(QWidget *parent);

    DisplayVideo();

    //! @see ModuleDisplay
    void init();

    //! get the info, datastream type is used @see DataStreamTypes
    DataStreamTypes::TYPE getDataStreamType();

public slots:

    //! modul slot to receive Data , each Modul must have this !
    void receiveData(DataFrame::SPtr d);

signals:

    //! modul signal to send Data
    void sendData(DataFrame::SPtr d);

private:

    quint64 m_debugCounter;
    QLabel *m_label;
    QPixmap *m_pixmap;
    QHBoxLayout *m_layout;
    quint64 m_personalID;
};

#endif // DISPLAYVIDEO_H
