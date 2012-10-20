#include "player.h"
#include <QtConcurrentRun>
#include <QDebug>

Player::Player(QObject *parent, API *api, PlaylistHandler *plh, AudioPlayerBridge *apb) :
    QObject(parent)
{
    currentLength = 0;
    this->api = api;
    this->plh = plh;
    this->apb = apb;
    connect(apb, SIGNAL(songFinished()), this, SLOT(next()));
    connect(apb, SIGNAL(startedPlaying()), this, SLOT(startedPlaying()));
    connect(api, SIGNAL(streamKeyReady(StreamInformation*)), this, SLOT(gotStreamKey(StreamInformation*)));
    connect(plh, SIGNAL(playlistsChanged(std::vector<std::string>)), this, SLOT(refreshPlaylists(std::vector<std::string>)));
    connect(plh, SIGNAL(songsChanged(std::string,std::vector<Song*>)), this, SLOT(refreshSongs(std::string,std::vector<Song*>)));
    posChangeTimer = new QTimer(this);
    posChangeTimer->setInterval(200);
    connect(posChangeTimer, SIGNAL(timeout()), this, SLOT(onPosChangeTimer_tick()));
    posChangeTimer->start();
    currentSong = NULL;
    refreshPlaylists(plh->getPlaylists());
}

void Player::pause() {
    if (isPlaying()) {
        apb->togglePlayPause();
        emit stateChanged();
    }
}

void Player::startedPlaying() {
    emit stateChanged();
}

void Player::onPosChangeTimer_tick() {
    if (currentSong == NULL || apb->getState() == AudioPlayerBridge::STOPPED)
        return;
    emit songPositionChanged();
}

void Player::refreshPlaylists(std::vector<std::string> playlists) {
    if (playlists.size() == 0) {
        this->currentPlaylist = "";
        this->currentPlaylistSongs = std::vector<Song*>();
        stop();
        emit playlistsChanged(playlists);
        emit currentSongChanged();
        return;
    }
    bool foundMyOne = false;
    for (unsigned int i = 0; i < playlists.size(); i++) {
        if (playlists.at(i) == this->currentPlaylist) {
            foundMyOne = true;
        }
    }
    if (!foundMyOne) {
        this->currentPlaylist = playlists.at(0);
        stop();
    }
    emit playlistsChanged(playlists);
    refreshSongs(this->currentPlaylist, plh->getSongs(this->currentPlaylist));
}

void Player::refreshSongs(std::string playlistName, std::vector<Song*> songs) {
    if (playlistName != this->currentPlaylist)
        return;
    bool foundMyOne = false;
    this->currentPlaylistSongs = songs;
    if (currentSong != NULL) {
        for (unsigned int i = 0; i < currentPlaylistSongs.size(); i++) {
            if (((Song*) currentPlaylistSongs.at(i))->getSongId() == this->currentSong->getSongId()) {
                foundMyOne = true;
            }
        }
    }
    if (!foundMyOne) {
        stop();
    }
    emit currentSongChanged();
}

Song* Player::getCurrentSong() {
    if (currentSong == NULL && currentPlaylistSongs.size() != 0) {
        return currentPlaylistSongs.at(0);
    }
    return currentSong;
}

Song* Player::getSongBefore() {
    unsigned int currSong = getCurrentId();
    if (currentSong == NULL)
        return NULL;
    if (currSong == 0)
        return NULL;
    return currentPlaylistSongs.at(currSong - 1);
}

void Player::next() {
    Song* nextSong = getSongAfter();
    stop();
    if (nextSong == NULL)
        return;
    currentSong = nextSong;
    play();
}

void Player::prev() {
    Song* prevSong = getSongBefore();
    stop();
    if (prevSong == NULL) {
        return;
    }
    currentSong = prevSong;
    play();
}

Song* Player::getSongAfter() {
    int currSong = getCurrentId();
    if (currentPlaylistSongs.size() == 0)
        return NULL;
    if (currSong == (int) currentPlaylistSongs.size() - 1) {
        return NULL;
    }
    return currentPlaylistSongs.at(currSong + 1);
}

void Player::setVolume(int vol) {
    apb->setVolume(vol);
}

int Player::getSongPosition() {
    if (isStopped())
        return 0;
    return apb->getPosition();
}

int Player::getVolume() {
    return apb->getVolume();
}

bool Player::isPlaying() {
    if (currentSong != NULL && apb->getState() == AudioPlayerBridge::PLAYING)
        return true;
    return false;
}

bool Player::isPaused() {
    return (currentSong != NULL && apb->getState() == AudioPlayerBridge::PAUSED);
}

bool Player::isStopped() {
    return (apb->getState() == AudioPlayerBridge::STOPPED || apb->getState() == AudioPlayerBridge::STARTING);
}

void Player::setPosition(int pos) {
    if (currentSong == NULL || apb->getState() == AudioPlayerBridge::STOPPED)
        return;
    apb->setPosition(pos);
}

unsigned int Player::getCurrentId() {
    int currentId = 0;
    if (currentSong == NULL)
        return currentId;
    for (unsigned int i = 0; i < currentPlaylistSongs.size(); i++) {
        if (((Song*) currentPlaylistSongs.at(i))->getSongId() == currentSong->getSongId()) {
            currentId = i;
        }
    }
    return currentId;
}

void Player::stop() {
    apb->stop();
    currentSong = NULL;
    currentLength = 0;
    emit songPositionChanged();
    emit currentSongChanged();
    emit stateChanged();
}

bool Player::isStarting() {
    return (apb->getState() == AudioPlayerBridge::STARTING);
}

void Player::play() {
    if (isPaused()) {
        apb->togglePlayPause();
        emit stateChanged();
    } else if (currentSong == NULL && apb->getState() == AudioPlayerBridge::STOPPED && currentPlaylist != "" && currentPlaylistSongs.size() != 0) {
        currentSong = currentPlaylistSongs.at(0);
        emit currentSongChanged();
        api->getStreamKeyFromSongIDEx(currentSong->getSongId());
        apb->setState(AudioPlayerBridge::STARTING);
        emit stateChanged();
    } else if (apb->getState() == AudioPlayerBridge::STOPPED && currentSong != NULL) {
        emit currentSongChanged();
        api->getStreamKeyFromSongIDEx(currentSong->getSongId());
        apb->setState(AudioPlayerBridge::STARTING);
        emit stateChanged();
    }
}

void Player::setPlaylist(std::string playlist) {
    if (currentPlaylist == playlist)
        return;
    currentPlaylist = playlist;
    stop();
    refreshPlaylists(plh->getPlaylists());
}

std::string Player::getPlaylist() {
    return currentPlaylist;
}

void Player::gotStreamKey(StreamInformation *info) {
    currentLength = info->getUSecs() / 1000;
    emit songPositionChanged();
    QtConcurrent::run(apb, &AudioPlayerBridge::openAndPlay, info->directUrl());
    emit stateChanged();
}

int Player::getLength() {
    return currentLength;
}
