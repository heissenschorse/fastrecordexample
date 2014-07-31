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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//qt
#include <QMainWindow>
#include <QDesktopServices>
#include <QDir>
#include <QUrl>
#include <QTimer>

//own
#include "displayvideo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //! return the ref to diplay
    DisplayVideo* getRefToDisplay();


signals:

    void signalStartLoggingClicked();

    void signalStopLoggingClicked();

    void signalFileNameChanged(QString s);

    void signalSaveNext10Pics();

public slots:

    void on_pushButton_start_clicked();

    void on_pushButton_stop_clicked();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_dir_clicked();

    //! red dot method
    void blinkRedDot();


    void on_pushButton_save10Pics_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *m_redDotTimer;
    bool m_recordingFlag;
};

#endif // MAINWINDOW_H
