#ifndef FFMPEG_STRUCT_H
#define FFMPEG_STRUCT_H

/* for media info
 *
 */
enum _media_info_item{
    MEDIA_FORMAT = 0,
    MEDIA_STREAM,
    MEDIA_START_TIME,
    MEDIA_DURATION,

    VIDEO_CODEC_ID,
    VIDEO_CODEC_NAME,
    VIDEO_PIX_FMT,
    VIDEO_FRAME_RATE,
    VIDEO_WIDTH,
    VIDEO_HEIGHT,

    AUDIO_CODEC_ID,
    AUDIO_CODEC_NAME,
    AUDIO_CHANNELS,

    MEDIA_MAX
};

#define STR_LEN 128


























#endif // FFMPEG_STRUCT_H
