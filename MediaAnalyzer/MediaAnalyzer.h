#ifndef MEDIAANALYZER_H
#define MEDIAANALYZER_H

#include "components/ffmpeg_info.h"


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


private slots:
    void Select_LocalMedia();
};

#endif // MEDIAANALYZER_H
