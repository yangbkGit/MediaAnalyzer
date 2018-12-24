#ifndef MEDIAANALYZER_H
#define MEDIAANALYZER_H

#include <QMainWindow>

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
};

#endif // MEDIAANALYZER_H
