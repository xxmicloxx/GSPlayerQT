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
#include "player.h"
#include <vector>

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
    void setMainWindow(QMainWindow* mw);
    void setPlayer(Player *player);
    ~SearchMusicWindow();
    void createNewPlaylist();

public slots:
    void gotSongSearchResult(std::vector<Song*> result);
    void fullyBlendedOut();
    void addedMessage(Message* message);
    void deletedMessage(Message* message);
    void onPlaylistChange(std::vector<std::string> vector);

private slots:
    void addTestItem();

    void on_btnBack_clicked();

    void on_btnSearchSong_clicked();

    void on_txtSearchSong_returnPressed();

    void on_btnPopular_clicked();

    void on_cmbAddAll_currentIndexChanged(int index);
    void searchSong(std::string text);
    void getPopularSongs();
    void getSongsByArtist(int artistId);
    void getSongsByAlbum(int albumId);

private:
    void addSeperator();
    QMainWindow* realMainWindow;
    std::vector<SearchMusicListItem*> itemVector;
    MessageHandler *handler;
    bool working;
    bool refreshingPlaylists;
    Ui::SearchMusicWindow *ui;
    SearchMusicOverlay *overlay;
    API *api;
    AudioPlayerBridge *apb;
    PlaylistHandler *plh;
    Player *player;
};

#endif // SEARCHMUSICWINDOW_H
