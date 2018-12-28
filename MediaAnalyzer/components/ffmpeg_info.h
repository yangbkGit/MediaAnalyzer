#ifndef FFMPEG_INFO_H
#define FFMPEG_INFO_H

#include "components/general.h"

class FFMPEG_Info
{
public:
    static void ffmpeg_Init();
    static void get_FFMEPGInfo();
    static int get_MediaInfo(const char *filePath, QStringList &infoList);
    static int get_H264_videofile(const char *srcPath, const char *dstPath);
    static int get_YUV_videofile(const char *srcPath, const char *dstPath);
    static int get_AAC_audiofile(const char *srcPath, const char *dstPath);

private:
    static int bSingleton;
    
    FFMPEG_Info();
    int analyzePkt2File(AVFormatContext *pFormatCtx, AVPacket *pkt, FILE *pDst);
    
};

#endif // FFMPEG_INFO_H
