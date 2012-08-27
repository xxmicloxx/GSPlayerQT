#ifndef SEARCHMUSICWINDOW_H
#define SEARCHMUSICWINDOW_H

#include <QMainWindow>
#include "API/api.h"
#include "audioplayerbridge.h"

namespace Ui {
class SearchMusicWindow;
}

class SearchMusicWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SearchMusicWindow(QWidget *parent = 0);
    void setAPI(API *api);
    void setAPB(AudioPlayerBridge *apb);
    ~SearchMusicWindow();

private slots:
    void on_btnBack_clicked();

    void on_btnSearchSong_clicked();

private:
    Ui::SearchMusicWindow *ui;
    API *api;
    AudioPlayerBridge *apb;
};

#endif // SEARCHMUSICWINDOW_H
