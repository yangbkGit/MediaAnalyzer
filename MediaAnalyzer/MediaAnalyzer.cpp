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

    mediaItem_1.media_name.clear();
    mediaItem_1.video_codec = AV_CODEC_ID_NONE;
    mediaItem_1.audio_codec = AV_CODEC_ID_NONE;
    mediaItem_2.media_name.clear();
    mediaItem_2.video_codec = AV_CODEC_ID_NONE;
    mediaItem_2.audio_codec = AV_CODEC_ID_NONE;

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

    if(sender == ui->actionLocal_1){
        //1.Get media file info
        QByteArray ba = filePath.toLatin1();
        char *srcPath = ba.data();
        QStringList strList;
        QStringList &rStrList = strList;
        FFMPEG_Info::get_MediaInfo(srcPath, rStrList, &mediaItem_1);

        //2.Show on screen
        mediaItem_1.media_name = filePath;
        ui->InfoView_1->clear();
        for(int i=0; i<strList.size(); i++){
            ui->InfoView_1->append(strList.at(i));
        }
        ui->InfoView_1->show();
    }
    else if(sender == ui->actionLocal_2){
        //1.Get media file info
        QByteArray ba = filePath.toLatin1();
        char *srcPath = ba.data();
        QStringList strList;
        QStringList &rStrList = strList;
        FFMPEG_Info::get_MediaInfo(srcPath, rStrList, &mediaItem_2);

        //2.Show on screen
        mediaItem_2.media_name = filePath;
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
    if(mediaItem_1.media_name.isEmpty() || (AV_CODEC_ID_H264 != mediaItem_1.video_codec)){
        return;
    }
    QString dstPath;
    QMessageBox::StandardButton selected = QMessageBox::information(this,
                                                                    tr("select the FileType you want to save"),
                                                                    tr("OK means *.H264, Canel means"),
                                                                    QMessageBox::Ok | QMessageBox::Cancel);
    if(selected == QMessageBox::Ok){
        dstPath = QFileDialog::getSaveFileName(this, "Save as...",
                                               QString("../SupportedFiles"),
                                               tr("Media files(*.h264)"));
    } else if(selected == QMessageBox::Cancel){
        dstPath = QFileDialog::getSaveFileName(this, "Save as...",
                                               QString("../SupportedFiles"),
                                               tr("Media files(*.yuv)"));
    }

    if(dstPath.isEmpty()){
        return;
    }
    char *chrSrc, *chrDst;
    QByteArray baSrc, baDst;
    baSrc = mediaItem_1.media_name.toLatin1();
    chrSrc = baSrc.data();
    baDst = dstPath.toLatin1();
    chrDst = baDst.data();

    if(selected == QMessageBox::Ok){
        FFMPEG_Fetch264::get_H264_videofile(chrSrc, chrDst);
    } else if(selected == QMessageBox::Cancel){
        FFMEPG_FetchYUV::get_YUV_videofile(chrSrc, chrDst);
    }
    MA_DBUG("getVideoButtonPress Finished !\n");
}

void MediaAnalyzer::getAudioButtonPress()
{
    MA_DBUG("\n");

    if(mediaItem_1.media_name.isEmpty() || (AV_CODEC_ID_NONE == mediaItem_1.audio_codec)){
        return;
    }
    QString dstPath;
    dstPath = QFileDialog::getSaveFileName(this, "Save as...",
                                           QString("../SupportedFiles"),
                                           tr("Media files(*.aac)"));
    if(dstPath.isEmpty()){
        return;
    }
    char *chrSrc, *chrDst;
    QByteArray baSrc, baDst;
    baSrc = mediaItem_1.media_name.toLatin1();
    chrSrc = baSrc.data();
    baDst = dstPath.toLatin1();
    chrDst = baDst.data();

    FFMPEG_FetchAAC::get_AAC_audiofile(chrSrc, chrDst);
    MA_DBUG("getAudioButtonPress Finished !\n");
}

void MediaAnalyzer::playButtonPress()
{
    MA_DBUG("\n");
}

void MediaAnalyzer::saveAsButtonPress()
{
    MA_DBUG("\n");
    if(mediaItem_1.media_name.isEmpty()){
        return;
    }
    QString dstPath;
    QMessageBox::StandardButton selected = QMessageBox::information(this,
                                                                    tr("select the FileType you want to save"),
                                                                    tr("OK means *.mp4,\n Canel means *.avi,\n Save means *.ts"),
                                                                    QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::Save);
    if(selected == QMessageBox::Ok){
        dstPath = QFileDialog::getSaveFileName(this, "Save as...",
                                               QString("../SupportedFiles"),
                                               tr("Media files(*.mp4)"));
    } else if(selected == QMessageBox::Cancel){
        dstPath = QFileDialog::getSaveFileName(this, "Save as...",
                                               QString("../SupportedFiles"),
                                               tr("Media files(*.avi)"));
    } else if(selected == QMessageBox::Save){
        dstPath = QFileDialog::getSaveFileName(this, "Save as...",
                                               QString("../SupportedFiles"),
                                               tr("Media files(*.ts)"));
    }

    if(dstPath.isEmpty()){
        return;
    }
    char *chrSrc, *chrDst;
    QByteArray baSrc, baDst;
    baSrc = mediaItem_1.media_name.toLatin1();
    chrSrc = baSrc.data();
    baDst = dstPath.toLatin1();
    chrDst = baDst.data();

    FFMPEG_SAVE::get_mediaFile(chrSrc, chrDst);
    MA_DBUG("saveAsButtonPress Finished !\n");
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





