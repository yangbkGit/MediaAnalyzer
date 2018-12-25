#ifndef MEDIAANALYZER_H
#define MEDIAANALYZER_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QMessageBox>
#include <QStatusBar>

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
    void open();

    QAction *openAction;
};

#endif // MEDIAANALYZER_H
