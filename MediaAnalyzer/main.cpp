#include "MediaAnalyzer.h"
#include <QApplication>
#include <QDebug>



int CoreSingleton::bSingleton = FAILED;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CoreSingleton::CoreInitial();

    MediaAnalyzer userUI;
    userUI.show();



    return a.exec();
}
