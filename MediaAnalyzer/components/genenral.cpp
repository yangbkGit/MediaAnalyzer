#include "general.h"
#include <QDebug>

void LibDebug::ffmpegInfo()
{
    qDebug() <<avcodec_configuration();
    unsigned version = avcodec_version();
    qDebug() <<"version:" <<version;
}

void FFMPEG_Init()
{
    av_register_all();
}


int Get_MediaInfo(const char *filePath, char *infoList)
{
    if(!infoList){
        return FAILED;
    }


    int ret = SUCCESS;
    AVFormatContext *pFormatCtx = NULL;

    FFMPEG_Init();
    ret = avformat_open_input(&pFormatCtx, filePath, NULL, NULL);
    if(!ret){
        return FAILED;
    }









__FAILED:
    avformat_close_input(&pFormatCtx);
}


























