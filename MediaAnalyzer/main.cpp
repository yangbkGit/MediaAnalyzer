#include "MediaAnalyzer.h"
#include <QApplication>
#include <QDebug>

#include "components/general.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MediaAnalyzer userUI;
    userUI.show();

    LibDebug::ffmpegInfo();

    return a.exec();
}
