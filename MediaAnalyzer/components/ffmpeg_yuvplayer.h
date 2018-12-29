#ifndef FFMPEG_YUVPLAYER_H
#define FFMPEG_YUVPLAYER_H

#include "components/general.h"

class FFMPEG_YUVPlayer
{
public:
    static int player(const char *videoPath,
                      int w_width, int w_height,
                      const int video_width, const int video_height, Uint32 pixFormat);
private:
    static int refresh_video_timer(void *udata);

    FFMPEG_YUVPlayer();
    ~FFMPEG_YUVPlayer();
};

#endif // FFMPEG_YUVPLAYER_H
