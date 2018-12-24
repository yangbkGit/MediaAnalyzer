#include "MediaAnalyzer.h"
#include "ui_MediaAnalyzer.h"

MediaAnalyzer::MediaAnalyzer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MediaAnalyzer)
{
    ui->setupUi(this);
}

MediaAnalyzer::~MediaAnalyzer()
{
    delete ui;
}
