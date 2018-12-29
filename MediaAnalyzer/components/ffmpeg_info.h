#ifndef FFMPEG_INFO_H
#define FFMPEG_INFO_H

#include "components/general.h"
#include "components/ffmpeg_struct.h"

class FFMPEG_Info
{
public:
    static void ffmpeg_Init();
    static void get_FFMEPGInfo();
    static int get_MediaInfo(const char *filePath, QStringList &infoList, STmedia *media_info);

    static int get_AAC_audiofile(const char *srcPath, const char *dstPath);

private:
    static int bSingleton;
    
    FFMPEG_Info();


};

#endif // FFMPEG_INFO_H
