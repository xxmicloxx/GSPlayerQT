#include "downloadsongwindow.h"
#include "ui_downloadsongwindow.h"

DownloadSongWindow::DownloadSongWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DownloadSongWindow)
{
    ui->setupUi(this);
}

DownloadSongWindow::~DownloadSongWindow()
{
    delete ui;
}
