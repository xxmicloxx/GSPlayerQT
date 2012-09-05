#ifndef DOWNLOADSONGWINDOW_H
#define DOWNLOADSONGWINDOW_H

#include <QMainWindow>

namespace Ui {
class DownloadSongWindow;
}

class DownloadSongWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit DownloadSongWindow(QWidget *parent = 0);
    ~DownloadSongWindow();
    
private:
    Ui::DownloadSongWindow *ui;
};

#endif // DOWNLOADSONGWINDOW_H
