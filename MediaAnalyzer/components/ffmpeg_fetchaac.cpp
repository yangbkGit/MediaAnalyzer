#include "ffmpeg_fetchaac.h"




#define DISP_DEBUG
#ifdef DISP_DEBUG
#define FETCHAAC_DBUG(fmt, arg...) UMFDBG("[FFMPEG_FetchAAC][%s:%d]" fmt , __FUNCTION__, __LINE__, ##arg)
#else
#define FETCHAAC_DBUG(fmt, arg...)
#endif

int FFMPEG_FetchAAC::get_AAC_audiofile(const char *srcPath, const char *dstPath)
{
    int ret = SUCCESS;

    int audio_index = -1;

    AVFormatContext *pOutFormatCtx = NULL;
    AVOutputFormat  *pOutFormat = NULL;
    AVStream        *pOutStream = NULL;
    AVStream        *pInStream = NULL;
    AVFormatContext *pFormatCtx = NULL;
    AVPacket pkt;

    MEM_FUNC_CALL(srcPath);
    MEM_FUNC_CALL(dstPath);

    // in
    MPEG_FUNC_CALL(avformat_open_input(&pFormatCtx, srcPath, NULL, NULL));
    if((audio_index = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0)) < 0){
        FETCHAAC_DBUG("av_find_best_stream() failed.\n");
        return FAILED;
    }
    pInStream = pFormatCtx->streams[audio_index];
    AVCodecParameters *pInCodecpar = pInStream->codecpar;

    // out
    pOutFormatCtx = avformat_alloc_context();
    MEM_FUNC_CALL(pOutFormat = av_guess_format(NULL, dstPath, NULL));
    pOutFormatCtx->oformat = pOutFormat;
    MEM_FUNC_CALL(pOutStream = avformat_new_stream(pOutFormatCtx, NULL));

    MPEG_FUNC_CALL(avcodec_parameters_copy(pOutStream->codecpar, pInCodecpar));
    pOutStream->codecpar->codec_tag = 0;
    if((ret = avio_open(&pOutFormatCtx->pb, dstPath, AVIO_FLAG_WRITE)) < 0){
        FETCHAAC_DBUG("avio_open() failed.\n");
        goto __FAIL;
    }

    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    if(avformat_write_header(pOutFormatCtx, NULL) < 0){
        FETCHAAC_DBUG("avformat_write_header() failed.\n");
        goto __FAIL;
    }

    while(av_read_frame(pFormatCtx, &pkt) >= 0){
        if(pkt.stream_index == audio_index){
            FETCHAAC_DBUG("audio_index() start.\n");
            FETCHAAC_DBUG("av_rescale_q_rnd() start.\n");
            pkt.pts = av_rescale_q_rnd(pkt.pts, pInStream->time_base, pOutStream->time_base,
                                       (enum AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
            FETCHAAC_DBUG("av_rescale_q_rnd() end.\n");
            pkt.dts = pkt.pts;
            pkt.duration = av_rescale_q(pkt.duration, pInStream->time_base, pOutStream->time_base);
            pkt.pos = -1;
            pkt.stream_index = 0;
            FETCHAAC_DBUG("av_interleaved_write_frame() start.\n");
            av_interleaved_write_frame(pOutFormatCtx, &pkt);
            FETCHAAC_DBUG("av_interleaved_write_frame() end.\n");
            av_packet_unref(&pkt);
        }
    }

    av_write_trailer(pOutFormatCtx);


__FAIL:
    if(pOutFormatCtx){
        avformat_free_context(pOutFormatCtx);
    }

    if(pOutStream){
        avcodec_close(pOutStream->codec);
        //avformat_free_context(pOutStream) ????????
    }

    if(ret >= 0){
        avio_close(pOutFormatCtx->pb);
        ret = SUCCESS;
    }

    avformat_close_input(&pFormatCtx);

    return ret;
}





























FFMPEG_FetchAAC::FFMPEG_FetchAAC()
{

}
