#ifndef SEARCHMUSICWINDOW_H
#define SEARCHMUSICWINDOW_H

#include <QMainWindow>
#include "API/api.h"
#include "audioplayerbridge.h"
#include "searchmusicoverlay.h"
#include "searchmusiclistitem.h"
#include "playlisthandler.h"
#include "message.h"
#include <QPoint>
#include "messagehandler.h"

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
    void setPLH(PlaylistHandler *plh);
    ~SearchMusicWindow();

public slots:
    void gotSongSearchResult(std::vector<Song*> result);
    void fullyBlendedOut();
    void addedMessage(Message* message);
    void deletedMessage(Message* message);

private slots:
    void addTestItem();

    void on_btnBack_clicked();

    void on_btnSearchSong_clicked();

    void on_btnSearchArtist_clicked();

    void on_txtSearchSong_returnPressed();

    void on_btnPopular_clicked();

private:
    std::vector<SearchMusicListItem*> itemVector;
    MessageHandler *handler;
    bool working;
    void searchSong(std::string text);
    void getPopularSongs();
    Ui::SearchMusicWindow *ui;
    SearchMusicOverlay *overlay;
    API *api;
    AudioPlayerBridge *apb;
    PlaylistHandler *plh;
};

#endif // SEARCHMUSICWINDOW_H
