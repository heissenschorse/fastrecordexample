#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //set standard foo file name
    ui->lineEdit->setText("test0815");

    //red dot timer for blinking, a small thing
    m_redDotTimer = new QTimer(this);
    QObject::connect(m_redDotTimer, SIGNAL(timeout()), this, SLOT(blinkRedDot()));
    //m_redDotTimer->start(800);
    m_recordingFlag = false;
    ui->label_redDot->setVisible(false);

    //hide button, not stable at the moment
    ui->pushButton_save10Pics->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::blinkRedDot()
{
    if(ui->label_redDot->isVisible())
    {
        ui->label_redDot->setVisible(false);
    }
    else ui->label_redDot->setVisible(true);
}

DisplayVideo* MainWindow::getRefToDisplay()
{
    return ui->widgetDisplayModul;
}

void MainWindow::on_pushButton_start_clicked()
{
    if(!m_recordingFlag)
    {
        m_recordingFlag = true;
        m_redDotTimer->start(800);//start blinking
        emit signalStartLoggingClicked();

        ui->label_redDot->setVisible(true);//first show
    }


}

void MainWindow::on_pushButton_stop_clicked()
{
    if(m_recordingFlag)
    {
        m_recordingFlag = false;
        m_redDotTimer->stop();//stop blinking
        emit signalStopLoggingClicked();

        ui->label_redDot->setVisible(false);//hide it again
    }


}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    QString fileName = arg1;
    fileName.append(".avi");

    //qDebug()<<"fileName changed :"<< fileName;
    emit signalFileNameChanged(fileName);

}

void MainWindow::on_pushButton_dir_clicked()
{
    QString path = QDir::toNativeSeparators(QApplication::applicationDirPath());
    QDesktopServices::openUrl(QUrl(QString("file:///" + path) ,QUrl::TolerantMode));
}


void MainWindow::on_pushButton_save10Pics_clicked()
{
    emit signalSaveNext10Pics();
}
