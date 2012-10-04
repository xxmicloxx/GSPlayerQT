#ifndef PLAYLISTOPTIMIZELISTITEM_H
#define PLAYLISTOPTIMIZELISTITEM_H

#include <QWidget>
#include "playlisthandler.h"
#include "API/song.h"
#include "coverhelper.h"

namespace Ui {
class PlaylistOptimizeListItem;
}

class PlaylistOptimizeListItem : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlaylistOptimizeListItem(QWidget *parent = 0, Song* song = NULL, PlaylistHandler* plh = NULL, std::string playlist = "", CoverHelper* coverHelper = NULL);
    ~PlaylistOptimizeListItem();
    Song* getSong();
    
private slots:
    void on_btnRemove_clicked();

    void on_btnInfo_clicked();

private:
    Ui::PlaylistOptimizeListItem *ui;
    std::string playlist;
    PlaylistHandler* plh;
    CoverHelper* coverHelper;
    Song* song;
};

#endif // PLAYLISTOPTIMIZELISTITEM_H
