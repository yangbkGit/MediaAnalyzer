#-------------------------------------------------
#
# Project created by QtCreator 2018-12-24T15:59:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MediaAnalyzer
TEMPLATE = app


SOURCES += main.cpp\
	MediaAnalyzer.cpp \
    components/ffmpeg_info.cpp \
    components/ffmpeg_fetch264.cpp \
    components/ffmepg_fetchyuv.cpp \
    components/ffmpeg_yuvplayer.cpp \
    components/ffmpeg_fetchaac.cpp \
    components/ffmpeg_save.cpp



HEADERS += MediaAnalyzer.h \
    components/ffmpeg_info.h \
    components/ffmpeg_struct.h \
    components/ffmpeg_fetch264.h \
    components/ffmepg_fetchyuv.h \
    components/ffmpeg_yuvplayer.h \
    components/ffmpeg_fetchaac.h \
    components/ffmpeg_save.h


FORMS   += MediaAnalyzer.ui






#----------------- Components -----------------
SOURCES += 



HEADERS +=  components/general.h















#----------------- FFmpeg -----------------
INCLUDEPATH += ../libs/FFmpeg/include

LIBS += ../libs/FFmpeg/lib/libavcodec.dll.a\
	../libs/FFmpeg/lib/libavdevice.dll.a\
	../libs/FFmpeg/lib/libavfilter.dll.a\
	../libs/FFmpeg/lib/libavformat.dll.a\
	../libs/FFmpeg/lib/libavutil.dll.a\
	../libs/FFmpeg/lib/libswresample.dll.a\
	../libs/FFmpeg/lib/libswscale.dll.a\
	../libs/FFmpeg/lib/libpostproc.dll.a




#----------------- SDL -----------------
INCLUDEPATH += ../libs/SDL2-2.0.9/include

LIBS += -L../libs/SDL2-2.0.9/lib/x86 -lSDL2
LIBS += -L../libs/SDL2-2.0.9/lib/x86 -lSDL2main
LIBS += -L../libs/SDL2-2.0.9/lib/x86 -lSDL2test


