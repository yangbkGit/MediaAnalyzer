#include "MediaAnalyzer.h"
#include "ui_MediaAnalyzer.h"



MediaAnalyzer::MediaAnalyzer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MediaAnalyzer)
{
    ui->setupUi(this);

    QObject::connect(ui->actionLocal, QAction::triggered, this, Select_LocalMedia);

}

MediaAnalyzer::~MediaAnalyzer()
{
    delete ui;
}


void MediaAnalyzer::Select_LocalMedia()
{
    QString filePath = QFileDialog::getOpenFileName(this, QString("Select a media file..."),\
                                                    QString("../SupportedFiles"), tr("Media files(\
                                    *.mp4 *.avi *.ts *.h264 *.yuv *.aac *.mp3)"));
    if(filePath.isEmpty()){
        return;
    }

    //1.Get media file info

}





