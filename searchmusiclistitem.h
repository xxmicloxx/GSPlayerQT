#ifndef SEARCHMUSICLISTITEM_H
#define SEARCHMUSICLISTITEM_H

#include <QWidget>
#include "API/song.h"
#include "API/api.h"
#include "audioplayerbridge.h"
#include "API/streaminformation.h"
#include "playlisthandler.h"
#include "messagehandler.h"
#include "player.h"

namespace Ui {
class SearchMusicListItem;
}

class SearchMusicListItem : public QWidget
{
    Q_OBJECT
    
public:
    explicit SearchMusicListItem(QWidget *parent = 0, Song* song = new Song(), API* api = new API(), AudioPlayerBridge* apb = new AudioPlayerBridge(), PlaylistHandler *plh = new PlaylistHandler(), MessageHandler* messageHandler = 0, Player *player = 0);
    ~SearchMusicListItem();
    Song* getSong();
    
public slots:
    void createNewPlaylist();
    void gotStreamKey(StreamInformation* info);
    void onPlaylistChange(std::vector<std::string> vector);

private slots:
    void showContextMenu(const QPoint& pos);
    void on_btnPlay_clicked();

    void on_cmbAddTo_currentIndexChanged(int index);

signals:
    void getAlbumsByArtist(int artistId);
    void getSongsByArtist(int artistId);
    void getSongsByAlbum(int albumId);

private:
    bool working;
    API* api;
    MessageHandler* messageHandler;
    PlaylistHandler *plh;
    AudioPlayerBridge* apb;
    Song* song;
    Ui::SearchMusicListItem *ui;
    Player *player;
    bool playerWasPlaying;

};

#endif // SEARCHMUSICLISTITEM_H
