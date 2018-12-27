#include "general.h"
#include <QDebug>

void CoreSingleton::FFMEPGInfo()
{
    qDebug() <<avcodec_configuration();
    unsigned version = avcodec_version();
    qDebug() <<"version:" <<version;
}

void CoreSingleton::CoreInitial()
{
    if(SUCCESS == CoreSingleton::bSingleton)
        return;
    bSingleton = SUCCESS;

    av_register_all();
    qDebug() <<"CoreInitial";
}


int Get_MediaInfo(const char *filePath, char *infoList)
{
    if(!infoList){
        return FAILED;
    }

    int ret = SUCCESS;
    AVFormatContext *pFormatCtx = NULL;

    ret = avformat_open_input(&pFormatCtx, filePath, NULL, NULL);
    if(!ret){
        return FAILED;
    }
    ret = avformat_find_stream_info(pFormatCtx, NULL);
    if(!ret){
        goto __EXIT;
    }
    av_dump_format(pFormatCtx, 0, filePath, 0);





__EXIT:
    avformat_close_input(&pFormatCtx);
    return ret;
}


























