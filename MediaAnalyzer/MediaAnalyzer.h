#ifndef MEDIAANALYZER_H
#define MEDIAANALYZER_H

#include "components/general.h"
#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QByteArray>
#include <QDir>

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
