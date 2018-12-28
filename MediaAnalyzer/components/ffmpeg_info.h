#ifndef FFMPEG_INFO_H
#define FFMPEG_INFO_H

#include "components/general.h"

class FFMPEG_Info
{
public:
    static void ffmpeg_Init();
    static void get_FFMEPGInfo();
    static int get_MediaInfo(const char *filePath, QStringList &infoList);
    static int get_H264_videofile(const char *srcPath, const char *dstPath);
    static int get_YUV_videofile(const char *srcPath, const char *dstPath);
    static int get_AAC_audiofile(const char *srcPath, const char *dstPath);

private:
    static int bSingleton;
    
    FFMPEG_Info();
    static int h264_mp4toannexb(AVFormatContext *pFormatCtx, AVPacket *pkt, FILE *pDst);
    static int h264_extradata_to_annexb(const uint8_t *codec_extradata,
                                 const int codec_extradata_size,
                                 AVPacket *out_extradata, int padding);
    static int alloc_and_copy(AVPacket *out,
                      const uint8_t *sps_pps, uint32_t sps_pps_size,
                      const uint8_t *in, uint32_t in_size);

};

#endif // FFMPEG_INFO_H
