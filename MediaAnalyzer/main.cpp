#include "MediaAnalyzer.h"
#include <QApplication>
#include <QDebug>


int FFMPEG_Info::bSingleton = FAILED;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FFMPEG_Info::ffmpeg_Init();

    MediaAnalyzer userUI;
    userUI.show();



    return a.exec();
}
