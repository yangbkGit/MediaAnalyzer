#include "MediaAnalyzer.h"
#include "ui_MediaAnalyzer.h"

MediaAnalyzer::MediaAnalyzer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MediaAnalyzer)
{
    ui->setupUi(this);
    setWindowTitle(tr("MediaAnalyzer"));

    openAction = new QAction(tr("&Open"), this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, &QAction::triggered, this, &MediaAnalyzer::open);

    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(openAction);

    QToolBar *toolBar = addToolBar(tr("&file"));
    toolBar->addAction(openAction);

    statusBar();
}

MediaAnalyzer::~MediaAnalyzer()
{
    delete ui;
    delete openAction;
}

void MediaAnalyzer::open()
{
    QMessageBox::information(this, tr("Information"), tr("Open"));
}








