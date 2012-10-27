#ifndef PLAYLISTOPTIMIZEWINDOW_H
#define PLAYLISTOPTIMIZEWINDOW_H

#include <QMainWindow>
#include "API/api.h"
#include "playlisthandler.h"
#include "coverhelper.h"
#include "player.h"

namespace Ui {
class PlaylistOptimizeWindow;
}

class PlaylistOptimizeWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit PlaylistOptimizeWindow(QWidget *parent = 0, CoverHelper* coverHelper = NULL, Player* player = NULL);
    ~PlaylistOptimizeWindow();
    void setPLH(PlaylistHandler* plh);
    void setAPI(API* api);
    void setMainWindow(QMainWindow *mw);
    
private slots:
    void refreshPlaylists(std::vector<std::string> playlists);
    void on_cmbPlaylist_currentIndexChanged(int index);
    void refreshItems(std::string,std::vector<Song*> songVector);
    void addSeperator();
    void reimplementSeperator();

    void on_btnCreatePlaylist_clicked();

    void on_btnDeletePlaylist_clicked();

    void on_btnRename_clicked();

    void on_btnPlay_clicked();

private:
    QMainWindow *realMainWindow;
    bool playlistsRefreshing;
    Ui::PlaylistOptimizeWindow *ui;
    PlaylistHandler *plh;
    CoverHelper* coverHelper;
    API *api;
    Player *player;
};

#endif // PLAYLISTOPTIMIZEWINDOW_H
