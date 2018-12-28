#include "ffmpeg_info.h"
#include "ffmpeg_struct.h"

#define DISP_DEBUG
#ifdef DISP_DEBUG
#define INFO_DBUG(fmt, arg...) UMFDBG("[FFMPEG_Info][%s:%d]" fmt , __FUNCTION__, __LINE__, ##arg)
#endif


FFMPEG_Info::FFMPEG_Info()
{

}

void FFMPEG_Info::ffmpeg_Init()
{
    if(SUCCESS == FFMPEG_Info::bSingleton)
        return;
    bSingleton = SUCCESS;

    av_register_all();
}

void FFMPEG_Info::get_FFMEPGInfo()
{
    qDebug() <<avcodec_configuration();
    unsigned version = avcodec_version();
    qDebug() <<"version:" <<version;
}


int FFMPEG_Info::get_MediaInfo(const char *filePath, QStringList &infoList)
{
    int ret = SUCCESS;
    char strtmp[STR_LEN];
    AVFormatContext *pFormatCtx = NULL;

    MPEG_FUNC_CALL(avformat_open_input(&pFormatCtx, filePath, NULL, NULL));
    MPEG_FUNC_CALL(avformat_find_stream_info(pFormatCtx, NULL));

    sprintf(strtmp, "- %s INFO -\n", filePath);
    infoList <<strtmp;
    sprintf(strtmp, "For media:");
    infoList <<strtmp;
    sprintf(strtmp, "\t input format: %s", pFormatCtx->iformat->name);
    infoList <<strtmp;
    sprintf(strtmp, "\t nb_stream: %d", pFormatCtx->nb_streams);
    infoList <<strtmp;
    int64_t start_time = pFormatCtx->start_time / AV_TIME_BASE;
    sprintf(strtmp, "\t start_time: %ld", start_time);
    infoList <<strtmp;
    int64_t duration = pFormatCtx->duration / AV_TIME_BASE;
    sprintf(strtmp, "\t duration: %ld", duration);
    infoList <<strtmp;

    int video_stream_idx = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if(video_stream_idx >= 0){
        AVStream *pVideoStream = pFormatCtx->streams[video_stream_idx];
        sprintf(strtmp, "For video:");
        infoList <<strtmp;
        sprintf(strtmp, "\t codec_id = %d", pVideoStream->codec->codec_id);
        infoList <<strtmp;
        sprintf(strtmp, "\t codec_name = %s", avcodec_get_name(pVideoStream->codec->codec_id));
        infoList <<strtmp;
        sprintf(strtmp, "\t width = %d", pVideoStream->codec->width);
        infoList <<strtmp;
        sprintf(strtmp, "\t height = %d", pVideoStream->codec->height);
        infoList <<strtmp;
        sprintf(strtmp, "\t pix_fmt = %d", pVideoStream->codec->pix_fmt);
        infoList <<strtmp;
        sprintf(strtmp, "\t avg_frame_rate = %d",
                  pVideoStream->avg_frame_rate.num / pVideoStream->avg_frame_rate.den);
        infoList <<strtmp;
    }

    int audio_stream_idx = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if(audio_stream_idx >= 0){
        AVStream *pAudioStream = pFormatCtx->streams[audio_stream_idx];
        sprintf(strtmp, "For audio:");
        infoList <<strtmp;
        sprintf(strtmp, "\t codec_id = %d", pAudioStream->codec->codec_id);
        infoList <<strtmp;
        sprintf(strtmp, "\t codec_name = %s", avcodec_get_name(pAudioStream->codec->codec_id));
        infoList <<strtmp;
        sprintf(strtmp, "\t channels = %d", pAudioStream->codec->channels);
        infoList <<strtmp;
    }

    avformat_close_input(&pFormatCtx);
    return ret;
}
