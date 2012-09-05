#ifndef SEARCHMUSICWINDOW_H
#define SEARCHMUSICWINDOW_H

#include <QMainWindow>
#include "API/api.h"
#include "audioplayerbridge.h"
#include "searchmusicoverlay.h"

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

public slots:
    void gotSongSearchResult(std::vector<Song*> result);
    void fullyBlendedOut();

private slots:
    void on_btnBack_clicked();

    void on_btnSearchSong_clicked();

    void on_btnSearchArtist_clicked();

private:
    void searchSong(std::string text);
    Ui::SearchMusicWindow *ui;
    SearchMusicOverlay *overlay;
    API *api;
    AudioPlayerBridge *apb;
};

#endif // SEARCHMUSICWINDOW_H
