#ifndef FFMEPG_FETCHYUV_H
#define FFMEPG_FETCHYUV_H

#include "components/general.h"

class FFMEPG_FetchYUV
{
public:
    static int get_YUV_videofile(const char *srcPath, const char *dstPath);
private:
    FFMEPG_FetchYUV();
    ~FFMEPG_FetchYUV();
};

#endif // FFMEPG_FETCHYUV_H
