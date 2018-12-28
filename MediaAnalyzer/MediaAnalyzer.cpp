#include "MediaAnalyzer.h"
#include "ui_MediaAnalyzer.h"

#include "components/ffmpeg_struct.h"

MediaAnalyzer::MediaAnalyzer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MediaAnalyzer)
{
    ui->setupUi(this);

    ui->InfoView_1->addAction(ui->actionLocal);
    ui->InfoView_2->addAction(ui->actionLocal);
    QObject::connect(ui->actionLocal, QAction::triggered, this, Select_LocalMedia);

}

MediaAnalyzer::~MediaAnalyzer()
{
    delete ui;
}


void MediaAnalyzer::Select_LocalMedia()
{
    QString filePath = QFileDialog::getOpenFileName(this, QString("Select a media file..."), QString("../SupportedFiles"),
                                                    tr("Media files(*.mp4 *.avi *.ts *.h264 *.yuv *.aac *.mp3)"));
    if(filePath.isEmpty()){
        return;
    }

    //1.Get media file info
    QByteArray ba = filePath.toLatin1();
    char *srcPath = ba.data();
    QStringList strList;
    QStringList &rStrList = strList;
    FFMPEG_Info::get_MediaInfo(srcPath, rStrList);

    //2.Show on screen
    for(int i=0; i<strList.size(); i++){
        ui->InfoView_1->append(strList.at(i));
    }
    ui->InfoView_1->show();
}





