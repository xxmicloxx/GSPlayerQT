#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QTimer>
#include "API/api.h"
#include "playlisthandler.h"
#include "audioplayerbridge.h"
#include "API/song.h"
#include "API/streaminformation.h"


class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = 0, API* api = 0, PlaylistHandler* plh = 0, AudioPlayerBridge* apb = 0);
    void setPlaylist(std::string playlist);
    std::string getPlaylist();
    Song* getCurrentSong();
    Song* getSongBefore();
    Song* getSongAfter();
    int getSongPosition();
    int getLength();
    void setPosition(int pos);
    bool isPlaying();
    bool isPaused();
    bool isStopped();
    bool isStarting();
    void setVolume(int vol);
    int getVolume();
    void nextNoPlay();
    void prevNoPlay();
    void stopNoReset();
    
signals:
    void currentSongChanged();
    void playlistsChanged(std::vector<std::string> playlists);
    void songPositionChanged();
    void stateChanged();

public slots:
    void refreshPlaylists(std::vector<std::string> playlists);
    void refreshSongs(std::string playlistName, std::vector<Song*> songs);
    void stop();
    void play();
    void next();
    void prev();
    void pause();
    void gotStreamKey(StreamInformation* info);
    void startedPlaying();

private slots:
    void onPosChangeTimer_tick();

private:
    unsigned int getCurrentId();
    API* api;
    PlaylistHandler* plh;
    AudioPlayerBridge* apb;
    std::string currentPlaylist;
    Song* currentSong;
    QTimer* posChangeTimer;
    int currentLength;
    std::vector<Song*> currentPlaylistSongs;
};

#endif // PLAYER_H
