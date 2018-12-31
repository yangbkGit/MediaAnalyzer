#ifndef FFMPEG_STRUCT_H
#define FFMPEG_STRUCT_H

#include <QString>

typedef struct MEDIA_INFO{
    QString media_name;
    int video_codec;
    int audio_codec;
} STmedia;

enum SAVE_MODE{
    SAVE_NOACTION = 0,

    SAVE_MP4,
    SAVE_AVI,
    SAVE_TS,

    SAVE_MAX
};


#define STR_LEN 128


























#endif // FFMPEG_STRUCT_H
