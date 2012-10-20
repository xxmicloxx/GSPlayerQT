#ifndef PLAYMUSICWINDOW_H
#define PLAYMUSICWINDOW_H

#include <QMainWindow>
#include "API/api.h"
#include "playlisthandler.h"
#include "audioplayerbridge.h"
#include "coverhelper.h"
#include "player.h"

namespace Ui {
class PlayMusicWindow;
}

class PlayMusicWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit PlayMusicWindow(QWidget *parent = 0, PlaylistHandler* plh = 0, API* api = 0, CoverHelper* coverHelper = 0, QMainWindow* mainWindow = 0, Player* player = 0);
    ~PlayMusicWindow();

public slots:
    void playlistsChanged(std::vector<std::string> playlists);
    void songsChanged();
    void refreshPlayPause();
    void gotCover(std::string path);
    void onPositionChanged();
    void makePauseButton();
    void makePlayButton();
    void disablePlay();
    void enablePlay();
    void enableAll();
    void disableControls();
    
private slots:
    void on_cmbPlaylists_currentIndexChanged(int index);

    void on_sldPosition_sliderPressed();

    void on_sldPosition_sliderReleased();

    void on_sldPosition_sliderMoved(int position);

    void on_btnPlay_clicked();

    void on_btnStop_clicked();

    void on_btnNext_clicked();

    void on_btnPrev_clicked();

    void on_sldVolume_sliderMoved(int position);

private:
    Ui::PlayMusicWindow *ui;
    API* api;
    Player* player;
    PlaylistHandler *plh;
    AudioPlayerBridge *apb;
    CoverHelper* coverHelper;
    bool playlistsRefreshing;
    QMainWindow* realMainWindow;
    bool posSliderMoving;
};

#endif // PLAYMUSICWINDOW_H
