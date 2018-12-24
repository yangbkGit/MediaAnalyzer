#ifndef __GENERAL_H__
#define __GENERAL_H__

/** standar C
 *
 */
extern "C"{
#ifdef __cplusplus
    #define __STDC_CONSTANT_MACROS
    #define __STDC_FORMAT_MACROS
    #ifdef _STDINT_H
        #undef _STDINT_H
    #endif
    #include <stdint.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
#endif


#ifndef INT64_C
    #define INT64_C
    #define UINT64_C
    //#define   UINT64_C(value)   __CONCAT(value,ULL)
#endif
}


/** FFMPEG
 *
 */
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavformat/version.h>
#include <libavutil/time.h>
#include <libavutil/mathematics.h>
#include <libswresample/swresample.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libavutil/timestamp.h>

/*** FFmpeg log System
 * av_log_set_level(AV_LOG_DEBUG);
 * av_log(NULL, AV_LOG_INFO, "...%s\n", op);
 **/
#include <libavutil/log.h>
}


/* SDL
 *
 */
extern "C"{
#include <SDL.h>
}

#undef main


class LibDebug
{

public:
    static void ffmpegInfo();
};




#endif
