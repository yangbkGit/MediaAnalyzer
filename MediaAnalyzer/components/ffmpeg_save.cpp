#include "ffmpeg_save.h"


#define DISP_DEBUG
#ifdef DISP_DEBUG
#define SAVE_DBUG(fmt, arg...) UMFDBG("[FFMPEG_SAVE][%s:%d]" fmt , __FUNCTION__, __LINE__, ##arg)
#else
#define SAVE_DBUG(fmt, arg...)
#endif



int FFMPEG_SAVE::get_mediaFile(const char *srcPath, const char *dstPath)
{
    AVOutputFormat *pOutFormat = NULL;
    AVFormatContext *pFormatCtx = NULL, *pOutFormatCtx = NULL;
    AVPacket pkt;
    int ret = SUCCESS, i;
    int stream_index = 0;
    int *stream_mapping = NULL;
    int stream_mapping_size = 0;

    av_register_all();

    MPEG_FUNC_CALL(ret = avformat_open_input(&pFormatCtx, srcPath, 0, 0));

    if ((ret = avformat_find_stream_info(pFormatCtx, 0)) < 0) {
        SAVE_DBUG("Failed to retrieve input stream information");
        goto end;
    }

    avformat_alloc_output_context2(&pOutFormatCtx, NULL, NULL, dstPath);
    if (!pOutFormatCtx) {
        SAVE_DBUG("Could not create output context\n");
        ret = AVERROR_UNKNOWN;
        goto end;
    }

    stream_mapping_size = pFormatCtx->nb_streams;
    stream_mapping = (int *)av_mallocz_array(stream_mapping_size, sizeof(*stream_mapping));
    if (!stream_mapping) {
        ret = AVERROR(ENOMEM);
        goto end;
    }

    pOutFormat = pOutFormatCtx->oformat;

    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        AVStream *out_stream;
        AVStream *in_stream = pFormatCtx->streams[i];
        AVCodecParameters *in_codecpar = in_stream->codecpar;

        if (in_codecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE) {
            stream_mapping[i] = -1;
            continue;
        }

        stream_mapping[i] = stream_index++;

        out_stream = avformat_new_stream(pOutFormatCtx, NULL);
        if (!out_stream) {
            fprintf(stderr, "Failed allocating output stream\n");
            ret = AVERROR_UNKNOWN;
            goto end;
        }

        ret = avcodec_parameters_copy(out_stream->codecpar, in_codecpar);
        if (ret < 0) {
            fprintf(stderr, "Failed to copy codec parameters\n");
            goto end;
        }
        out_stream->codecpar->codec_tag = 0;
    }
    av_dump_format(pOutFormatCtx, 0, dstPath, 1);

    if (!(pOutFormat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&pOutFormatCtx->pb, dstPath, AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open output file '%s'", dstPath);
            goto end;
        }
    }

    ret = avformat_write_header(pOutFormatCtx, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file\n");
        goto end;
    }

    while (1) {
        AVStream *in_stream, *out_stream;

        ret = av_read_frame(pFormatCtx, &pkt);
        if (ret < 0)
            break;

        in_stream  = pFormatCtx->streams[pkt.stream_index];
        if (pkt.stream_index >= stream_mapping_size ||
            stream_mapping[pkt.stream_index] < 0) {
            av_packet_unref(&pkt);
            continue;
        }

        pkt.stream_index = stream_mapping[pkt.stream_index];
        out_stream = pOutFormatCtx->streams[pkt.stream_index];

        /* copy packet */
        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;

        ret = av_interleaved_write_frame(pOutFormatCtx, &pkt);
        if (ret < 0) {
            fprintf(stderr, "Error muxing packet\n");
            break;
        }
        av_packet_unref(&pkt);
    }

    av_write_trailer(pOutFormatCtx);
end:

    avformat_close_input(&pFormatCtx);

    /* close output */
    if (pOutFormatCtx && !(pOutFormat->flags & AVFMT_NOFILE))
        avio_closep(&pOutFormatCtx->pb);
    avformat_free_context(pOutFormatCtx);

    av_freep(&stream_mapping);

    if (ret < 0 && ret != AVERROR_EOF) {
        //fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
        fprintf(stderr, "Error occurred: %d\n", ret);
        return 1;
    }

    return 0;
}








FFMPEG_SAVE::FFMPEG_SAVE()
{

}
