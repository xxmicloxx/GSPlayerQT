#ifndef SEARCHMUSICLISTITEM_H
#define SEARCHMUSICLISTITEM_H

#include <QWidget>
#include "API/song.h"
#include "API/api.h"
#include "audioplayerbridge.h"

namespace Ui {
class SearchMusicListItem;
}

class SearchMusicListItem : public QWidget
{
    Q_OBJECT
    
public:
    explicit SearchMusicListItem(QWidget *parent = 0, Song* song = new Song(), API* api = new API(), AudioPlayerBridge* apb = new AudioPlayerBridge());
    ~SearchMusicListItem();
    
private slots:
    void on_btnPlay_clicked();

private:
    API* api;
    AudioPlayerBridge* apb;
    Song* song;
    Ui::SearchMusicListItem *ui;
};

#endif // SEARCHMUSICLISTITEM_H
