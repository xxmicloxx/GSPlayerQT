#ifndef SEARCHMUSICLISTITEM_H
#define SEARCHMUSICLISTITEM_H

#include <QWidget>
#include "API/song.h"
#include "API/api.h"
#include "audioplayerbridge.h"
#include "API/streaminformation.h"
#include "playlisthandler.h"
#include "messagehandler.h"

namespace Ui {
class SearchMusicListItem;
}

class SearchMusicListItem : public QWidget
{
    Q_OBJECT
    
public:
    explicit SearchMusicListItem(QWidget *parent = 0, Song* song = new Song(), API* api = new API(), AudioPlayerBridge* apb = new AudioPlayerBridge(), PlaylistHandler *plh = new PlaylistHandler(), MessageHandler* messageHandler = 0);
    ~SearchMusicListItem();
    
public slots:
    void createNewPlaylist();
    void gotStreamKey(StreamInformation* info);
    void onPlaylistChange(std::vector<std::string> vector);

private slots:
    void on_btnPlay_clicked();

    void on_cmbAddTo_currentIndexChanged(int index);

private:
    bool working;
    API* api;
    MessageHandler* messageHandler;
    PlaylistHandler *plh;
    AudioPlayerBridge* apb;
    Song* song;
    Ui::SearchMusicListItem *ui;

};

#endif // SEARCHMUSICLISTITEM_H
