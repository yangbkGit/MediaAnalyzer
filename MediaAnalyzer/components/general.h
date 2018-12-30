#ifndef __GENERAL_H__
#define __GENERAL_H__

#define CONFIG_SUPPORT_DEBUG_MESSAGE
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
    #include <strings.h>
    #include <unistd.h>
#endif


#ifndef INT64_C
    #define INT64_C
    #define UINT64_C
    //#define   UINT64_C(value)   __CONCAT(value,ULL)
#endif

#define SUCCESS 0
#define FAILED  -1

#ifndef CONFIG_SUPPORT_DEBUG_MESSAGE
    #define UMFDBG(level, fmt, args...)
#else
#define UMFDEBUG
    #ifdef UMFDEBUG
        #define UMFDBG(fmt, args...) do {fprintf(stderr, "[UMF] " fmt, ## args); fflush(stderr);}while(0)
    #else
        #define UMFDBG(level, fmt, args...)
    #endif
#endif

#define MPEG_FUNC_CALL(funcCall) if(SUCCESS!=(funcCall)) \
    {UMFDBG("[RET ERROR][FUNC]:[%s] [LINE]:[%d]\n", __FUNCTION__,__LINE__);}

#define MEM_FUNC_CALL(funcCall) if(NULL==(funcCall)) \
    {UMFDBG("[MEM ERROR][FUNC]:[%s] [LINE]:[%d]\n", __FUNCTION__,__LINE__);}
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



/** for QT
 *
 */
#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QByteArray>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QTextEdit>
#include <QContextMenuEvent>
#include <QMessageBox>

#endif
