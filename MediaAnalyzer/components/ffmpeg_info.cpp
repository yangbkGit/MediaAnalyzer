#include "ffmpeg_info.h"


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


int FFMPEG_Info::get_MediaInfo(const char *filePath, char *infoList)
{
    if(!infoList){
        return FAILED;
    }


    int ret = SUCCESS;
    AVFormatContext *pFormatCtx = NULL;
#if 0   // just study
    MPEG_FUNC_CALL(avformat_open_input(&pFormatCtx, filePath, NULL, NULL));
    MPEG_FUNC_CALL(avformat_find_stream_info(pFormatCtx, NULL));
    av_dump_format(pFormatCtx, 0, filePath, 0);
    fflush(stderr);

    INFO_DBUG("----------------- get media info by hand ----------------\n");
    INFO_DBUG("input format: %s\n", pFormatCtx->iformat->name);
    INFO_DBUG("nb_stream: %d\n", pFormatCtx->nb_streams);
    int64_t start_time = pFormatCtx->start_time / AV_TIME_BASE;
    INFO_DBUG("start_time: %ld\n", start_time);
    int64_t duration = pFormatCtx->duration / AV_TIME_BASE;
    INFO_DBUG("duration: %ld\n", duration);

    int video_stream_idx = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if(video_stream_idx >= 0){
        AVStream *pVideoStream = pFormatCtx->streams[video_stream_idx];
        INFO_DBUG("For video:\n");
        INFO_DBUG("nb_frames = %ld\n", pVideoStream->nb_frames);
        INFO_DBUG("codec_id = %d, codec_name = %s\n",
                  pVideoStream->codec->codec_id, avcodec_get_name(pVideoStream->codec->codec_id));
        INFO_DBUG("width = %d, height = %d\n", pVideoStream->codec->width, pVideoStream->codec->height);
        INFO_DBUG("pix_fmt = %d\n", pVideoStream->codec->pix_fmt);
        INFO_DBUG("bitrate = %ld kb/s \n", pVideoStream->codec->bit_rate / 1000);
        INFO_DBUG("avg_frame_rate = %d\n",
                  pVideoStream->avg_frame_rate.num / pVideoStream->avg_frame_rate.den);
    }

    int audio_stream_idx = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if(audio_stream_idx >= 0){
        AVStream *pAudioStream = pFormatCtx->streams[audio_stream_idx];
        INFO_DBUG("For audio:\n");
        INFO_DBUG("codec_id = %d, codec_name = %s\n",
                  pAudioStream->codec->codec_id, avcodec_get_name(pAudioStream->codec->codec_id));
        INFO_DBUG("sample_rate = %d\n", pAudioStream->codec->sample_rate);
        INFO_DBUG("channels = %d\n", pAudioStream->codec->channels);
        INFO_DBUG("sample_fmt = %d\n", pAudioStream->codec->sample_fmt);
        INFO_DBUG("frame_size = %d\n", pAudioStream->codec->frame_size);
        INFO_DBUG("nb_frames = %ld\n", pAudioStream->nb_frames);
        INFO_DBUG("bitrate = %ld kb/s \n", pAudioStream->codec->bit_rate / 1000);
    }
#endif


    avformat_close_input(&pFormatCtx);
    return ret;
}
