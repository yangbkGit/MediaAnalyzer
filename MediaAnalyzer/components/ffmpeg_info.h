#ifndef FFMPEG_INFO_H
#define FFMPEG_INFO_H

#include "components/general.h"

class FFMPEG_Info
{
public:
    static void ffmpeg_Init();
    static void get_FFMEPGInfo();
    static int get_MediaInfo(const char *filePath, char *infoList);

private:
    static int bSingleton;
    FFMPEG_Info();
};

#endif // FFMPEG_INFO_H
