#ifndef FFMPEG_SAVE_H
#define FFMPEG_SAVE_H

#include "components/general.h"
#include "components/ffmpeg_struct.h"

class FFMPEG_SAVE
{
public:
    static int get_mediaFile(const char *srcPath, const char *dstPath);

private:
    FFMPEG_SAVE();
};

#endif // FFMPEG_SAVE_H
