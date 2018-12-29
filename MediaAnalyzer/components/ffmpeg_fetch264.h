#ifndef FFMPEG_VIDEO_H
#define FFMPEG_VIDEO_H

#include "components/general.h"
#include "components/ffmpeg_struct.h"

class FFMPEG_Fetch264
{
public:
    static int get_H264_videofile(const char *srcPath, const char *dstPath);
private:
    FFMPEG_Fetch264();
    ~FFMPEG_Fetch264();
    static int h264_mp4toannexb(AVFormatContext *pFormatCtx, AVPacket *pkt, FILE *pDst);
    static int h264_extradata_to_annexb(const uint8_t *codec_extradata,
                                 const int codec_extradata_size,
                                 AVPacket *out_extradata, int padding);
    static int alloc_and_copy(AVPacket *out,
                      const uint8_t *sps_pps, uint32_t sps_pps_size,
                      const uint8_t *in, uint32_t in_size);
};

#endif // FFMPEG_VIDEO_H
