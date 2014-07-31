#### BUILD ##################
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


##ffmepg support forward dependy to config
include(../config/ffmpeg.pri)
## framegrabber matrox orion hd, lib deps
include(../config/matroxorionhd.pri)

#####################################################
message(*Build source)

#destination dir
DESTDIR  = ../bin
#name of executable
TARGET = FastRecordExample

TEMPLATE = app

##use what exist
SOURCES += *.cpp

HEADERS  += *.h

FORMS    += *.ui

###################################

RESOURCES += ../include/resources.qrc
