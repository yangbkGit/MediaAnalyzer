#ifndef FFMPEG_FETCHAAC_H
#define FFMPEG_FETCHAAC_H

#include "components/general.h"
#include "components/ffmpeg_struct.h"

class FFMPEG_FetchAAC
{
public:
    static int get_AAC_audiofile(const char *srcPath, const char *dstPath);

private:
    FFMPEG_FetchAAC();
};

#endif // FFMPEG_FETCHAAC_H
