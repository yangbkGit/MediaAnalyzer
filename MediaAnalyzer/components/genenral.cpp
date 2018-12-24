#include "general.h"
#include <QDebug>

void LibDebug::ffmpegInfo()
{
    qDebug() <<avcodec_configuration();
    unsigned version = avcodec_version();
    qDebug() <<"version:" <<version;
}





