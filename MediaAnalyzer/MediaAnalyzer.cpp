#include "MediaAnalyzer.h"
#include "ui_MediaAnalyzer.h"

#include "components/ffmpeg_struct.h"

#define DISP_DEBUG
#ifdef DISP_DEBUG
#define MA_DBUG(fmt, arg...) UMFDBG("[MediaAnalyzer][%s:%d]" fmt , __FUNCTION__, __LINE__, ##arg)
#else
#define MA_DBUG(fmt, arg...)
#endif


MediaAnalyzer::MediaAnalyzer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MediaAnalyzer)
{
    ui->setupUi(this);

    QObject::connect(ui->actionLocal_1, &QAction::triggered, this, &MediaAnalyzer::selectLocalMedia);
    QObject::connect(ui->actionLocal_2, &QAction::triggered, this, &MediaAnalyzer::selectLocalMedia);
    QObject::connect(ui->get_video_button, &QPushButton::pressed, this, &MediaAnalyzer::getVideoButtonPress);
    QObject::connect(ui->get_audio_button, &QPushButton::pressed, this, &MediaAnalyzer::getAudioButtonPress);
    QObject::connect(ui->play_button, &QPushButton::pressed, this, &MediaAnalyzer::playButtonPress);
    QObject::connect(ui->save_as_button, &QPushButton::pressed, this, &MediaAnalyzer::saveAsButtonPress);
    QObject::connect(ui->add_watermark_button, &QPushButton::pressed, this, &MediaAnalyzer::addWatermaskButtonPress);
    QObject::connect(ui->cut_media_button, &QPushButton::pressed, this, &MediaAnalyzer::cutMediaButtonPress);
    QObject::connect(ui->push_stream_button, &QPushButton::pressed, this, &MediaAnalyzer::pushStreamButtonPress);
    QObject::connect(ui->avMerge_button, &QPushButton::pressed, this, &MediaAnalyzer::avMergeButtonPress);
    QObject::connect(ui->screenshot_button, &QPushButton::pressed, this, &MediaAnalyzer::screenshotButtonPress);
}

MediaAnalyzer::~MediaAnalyzer()
{
    delete ui;
}


void MediaAnalyzer::selectLocalMedia()
{
    QString filePath = QFileDialog::getOpenFileName(this, QString("Select a media file..."), QString("../SupportedFiles"),
                                                    tr("Media files(*.mp4 *.avi *.ts *.h264 *.yuv *.aac *.mp3)"));
    if(filePath.isEmpty()){
        return;
    }
    QObject *sender = QObject::sender();

    //1.Get media file info
    QByteArray ba = filePath.toLatin1();
    char *srcPath = ba.data();
    QStringList strList;
    QStringList &rStrList = strList;
    FFMPEG_Info::get_MediaInfo(srcPath, rStrList);

    //2.Show on screen
    if(sender == ui->actionLocal_1){
        ui->InfoView_1->clear();
        for(int i=0; i<strList.size(); i++){
            ui->InfoView_1->append(strList.at(i));
        }
        ui->InfoView_1->show();
    }
    else if(sender == ui->actionLocal_2){
        ui->InfoView_2->clear();
        for(int i=0; i<strList.size(); i++){
            ui->InfoView_2->append(strList.at(i));
        }
        ui->InfoView_2->show();
    }
}

/**
 * @brief MediaAnalyzer::getVideoButtonPress
 *      Force: media video codec is H.264
 */
void MediaAnalyzer::getVideoButtonPress()
{
    MA_DBUG("\n");
}

void MediaAnalyzer::getAudioButtonPress()
{
    MA_DBUG("\n");
}

void MediaAnalyzer::playButtonPress()
{
    MA_DBUG("\n");
}

void MediaAnalyzer::saveAsButtonPress()
{
    MA_DBUG("\n");
}

void MediaAnalyzer::addWatermaskButtonPress()
{
    MA_DBUG("\n");
}

void MediaAnalyzer::cutMediaButtonPress()
{
    MA_DBUG("\n");
}

void MediaAnalyzer::pushStreamButtonPress()
{
    MA_DBUG("\n");
}

void MediaAnalyzer::avMergeButtonPress()
{
    MA_DBUG("\n");
}

void MediaAnalyzer::screenshotButtonPress()
{
    MA_DBUG("\n");
}





