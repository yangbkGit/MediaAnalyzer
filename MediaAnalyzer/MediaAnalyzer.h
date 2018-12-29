#ifndef MEDIAANALYZER_H
#define MEDIAANALYZER_H

#include "components/ffmpeg_info.h"
#include "components/ffmpeg_fetch264.h"
#include "components/ffmepg_fetchyuv.h"
#include "components/ffmpeg_yuvplayer.h"


namespace Ui {
class MediaAnalyzer;
}

class MediaAnalyzer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MediaAnalyzer(QWidget *parent = 0);
    ~MediaAnalyzer();


private:
    Ui::MediaAnalyzer *ui;
    STmedia mediaItem_1;
    STmedia mediaItem_2;

private slots:
    void selectLocalMedia();
    void getVideoButtonPress();
    void getAudioButtonPress();
    void playButtonPress();
    void saveAsButtonPress();
    void addWatermaskButtonPress();
    void cutMediaButtonPress();
    void pushStreamButtonPress();
    void screenshotButtonPress();
    void avMergeButtonPress();
};

#endif // MEDIAANALYZER_H
