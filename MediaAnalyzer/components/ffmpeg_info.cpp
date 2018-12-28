#include "ffmpeg_info.h"
#include "ffmpeg_struct.h"

#ifndef AV_WB32
#   define AV_WB32(p, val) do {                 \
        uint32_t d = (val);                     \
        ((uint8_t*)(p))[3] = (d);               \
        ((uint8_t*)(p))[2] = (d)>>8;            \
        ((uint8_t*)(p))[1] = (d)>>16;           \
        ((uint8_t*)(p))[0] = (d)>>24;           \
    } while(0)
#endif

#ifndef AV_RB16
#   define AV_RB16(x)                           \
    ((((const uint8_t*)(x))[0] << 8) |          \
      ((const uint8_t*)(x))[1])
#endif

#define DISP_DEBUG
#ifdef DISP_DEBUG
#define INFO_DBUG(fmt, arg...) UMFDBG("[FFMPEG_Info][%s:%d]" fmt , __FUNCTION__, __LINE__, ##arg)
#else
#define INFO_DBUG(fmt, arg...)
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

    qDebug() <<"filePath" <<filePath;
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



int FFMPEG_Info::get_H264_videofile(const char *srcPath, const char *dstPath)
{
    int ret = SUCCESS;
    FILE *pDst = NULL;

    int videoStreamIdx = -1;
    AVFormatContext *pFormatCtx = NULL;
    AVPacket *pkt = NULL;

    MEM_FUNC_CALL(pDst = fopen(srcPath, "wb"));
    MPEG_FUNC_CALL(avformat_open_input(&pFormatCtx, srcPath, NULL, NULL));
    videoStreamIdx = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);

    MPEG_FUNC_CALL(pkt = av_packet_alloc());
    while(av_read_frame(pFormatCtx, pkt)){
        if(pkt->stream_index == videoStreamIdx){
            h264_mp4toannexb(pFormatCtx, pkt, pDst);
        }
        av_packet_unref(pkt);
    }

    fclose(pDst);
    av_packet_free(&pkt);
    avformat_close_input(&pFormatCtx);
    return ret;
}

int FFMPEG_Info::get_YUV_videofile(const char *srcPath, const char *dstPath)
{

}

int FFMPEG_Info::get_AAC_audiofile(const char *srcPath, const char *dstPath)
{

}


int FFMPEG_Info::h264_mp4toannexb(AVFormatContext *pFormatCtx, AVPacket *pkt, FILE *pDst)
{
    AVPacket *out = NULL;
    AVPacket spspps_pkt;
    
    int ret = SUCCESS, i, len;
    uint8_t unit_type;
    int32_t nal_size;
    uint32_t cumul_size = 0;
    const uint8_t *buf;
    const uint8_t *buf_end;
    int buf_size;
    
    MEM_FUNC_CALL(out = av_packet_alloc());
    buf = pkt->data;                    // head
    buf_size = pkt->size;               // body
    buf_end = pkt->data + pkt->size;    // tail
    
    do{
        ret= AVERROR(EINVAL);
        if(buf+4 > buf_end)
            goto __FAIL;

        for(nal_size=0, i=0; i<4; i++)
            nal_size = (nal_size << 8) | buf[i];

        buf += 4;
        unit_type = *buf & 0x1f;

        if(nal_size > buf_end-buf || nal_size < 0)
            goto __FAIL;

        if(unit_type == 5){
            h264_extradata_to_annexb(pFormatCtx->streams[pkt->stream_index]->codec->extradata,
                    pFormatCtx->streams[pkt->stream_index]->codec->extradata_size,
                    &spspps_pkt,
                    AV_INPUT_BUFFER_PADDING_SIZE);
            if((ret = alloc_and_copy(out,
                                     spspps_pkt.data, spspps_pkt.size,
                                     buf, nal_size)) < 0)
                goto __FAIL;
        } else {
            if((ret = alloc_and_copy(out, NULL, 0, buf, nal_size)) < 0)
                goto __FAIL;
        }

        len = fwrite(out->data, 1, out->size, pDst);
        if(len != out->size){
            INFO_DBUG("warning, length of writed data isn't equal pkt.size(%d, %d)\n", len, out->size);
        }
        fflush(pDst);

        buf += nal_size;
        cumul_size += nal_size + 4;
    } while(cumul_size < buf_size);
    
    
__FAIL:
    av_packet_free(&out);
    return ret;
}

int FFMPEG_Info::h264_extradata_to_annexb(const uint8_t *codec_extradata, const int codec_extradata_size, AVPacket *out_extradata, int padding)
{
    uint16_t unit_size;
    uint64_t total_size                 = 0;
    uint8_t *out                        = NULL, unit_nb, sps_done = 0,
             sps_seen                   = 0, pps_seen = 0, sps_offset = 0, pps_offset = 0;
    const uint8_t *extradata            = codec_extradata + 4;
    static const uint8_t nalu_header[4] = { 0, 0, 0, 1 };
    int length_size = (*extradata++ & 0x3) + 1; // retrieve length coded size, 用于指示表示编码数据长度所需字节数

    sps_offset = pps_offset = -1;

    /* retrieve sps and pps unit(s) */
    unit_nb = *extradata++ & 0x1f; /* number of sps unit(s) */
    if (!unit_nb) {
        goto pps;
    }else {
        sps_offset = 0;
        sps_seen = 1;
    }

    while (unit_nb--) {
        int err;

        unit_size   = AV_RB16(extradata);
        total_size += unit_size + 4;
        if (total_size > INT_MAX - padding) {
            av_log(NULL, AV_LOG_ERROR,
                   "Too big extradata size, corrupted stream or invalid MP4/AVCC bitstream\n");
            av_free(out);
            return AVERROR(EINVAL);
        }
        if (extradata + 2 + unit_size > codec_extradata + codec_extradata_size) {
            av_log(NULL, AV_LOG_ERROR, "Packet header is not contained in global extradata, "
                   "corrupted stream or invalid MP4/AVCC bitstream\n");
            av_free(out);
            return AVERROR(EINVAL);
        }
        if ((err = av_reallocp(&out, total_size + padding)) < 0)
            return err;
        memcpy(out + total_size - unit_size - 4, nalu_header, 4);
        memcpy(out + total_size - unit_size, extradata + 2, unit_size);
        extradata += 2 + unit_size;
pps:
        if (!unit_nb && !sps_done++) {
            unit_nb = *extradata++; /* number of pps unit(s) */
            if (unit_nb) {
                pps_offset = total_size;
                pps_seen = 1;
            }
        }
    }

    if (out)
        memset(out + total_size, 0, padding);

    if (!sps_seen)
        av_log(NULL, AV_LOG_WARNING,
               "Warning: SPS NALU missing or invalid. "
               "The resulting stream may not play.\n");

    if (!pps_seen)
        av_log(NULL, AV_LOG_WARNING,
               "Warning: PPS NALU missing or invalid. "
               "The resulting stream may not play.\n");

    out_extradata->data      = out;
    out_extradata->size      = total_size;

    return length_size;
}

int FFMPEG_Info::alloc_and_copy(AVPacket *out,
                          const uint8_t *sps_pps, uint32_t sps_pps_size,
                          const uint8_t *in, uint32_t in_size)
{
    uint32_t offset         = out->size;
    uint8_t nal_header_size = offset ? 3 : 4;
    int err;

    err = av_grow_packet(out, sps_pps_size + in_size + nal_header_size);
    if (err < 0)
        return err;

    if (sps_pps)
        memcpy(out->data + offset, sps_pps, sps_pps_size);
    memcpy(out->data + sps_pps_size + nal_header_size + offset, in, in_size);
    if (!offset) {
        AV_WB32(out->data + sps_pps_size, 1);
    } else {
        (out->data + offset + sps_pps_size)[0] = 0;
        (out->data + offset + sps_pps_size)[1] = 0;
        (out->data + offset + sps_pps_size)[2] = 1;
    }

    return 0;
}












