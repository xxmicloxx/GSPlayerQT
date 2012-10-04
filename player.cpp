#include "player.h"
#include <QtConcurrentRun>
#include <QDebug>

Player::Player(QObject *parent, API *api, PlaylistHandler *plh, AudioPlayerBridge *apb) :
    QObject(parent)
{
    this->api = api;
    this->plh = plh;
    this->apb = apb;
    connect(apb, SIGNAL(songFinished()), this, SLOT(next()));
    connect(api, SIGNAL(streamKeyReady(StreamInformation*)), this, SLOT(gotStreamKey(StreamInformation*)));
    connect(plh, SIGNAL(playlistsChanged(std::vector<std::string>)), this, SLOT(refreshPlaylists(std::vector<std::string>)));
    connect(plh, SIGNAL(songsChanged(std::string,std::vector<Song*>)), this, SLOT(refreshSongs(std::string,std::vector<Song*>)));
    refreshPlaylists(plh->getPlaylists());
}

void Player::refreshPlaylists(std::vector<std::string> playlists) {
    if (playlists.size() == 0) {
        this->currentPlaylist = "";
        this->currentPlaylistSongs = std::vector<Song*>();
        stop();
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
    return currentSong;
}

Song* Player::getSongBefore() {
    unsigned int currSong = getCurrentId();
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
    unsigned int currSong = getCurrentId();
    if (currSong == currentPlaylistSongs.size() - 1) {
        return NULL;
    }
    return currentPlaylistSongs.at(currSong + 1);
}

unsigned int Player::getCurrentId() {
    unsigned int currentId = 0;
    for (unsigned int i = 0; i < currentPlaylistSongs.size(); i++) {
        if (currentPlaylistSongs.at(i) == currentSong) {
            currentId = i;
        }
    }
    return currentId;
}

void Player::stop() {
    if (currentSong != NULL) {
        apb->stop();
        currentSong = NULL;
    }
}

void Player::play() {
    if (currentSong != NULL && apb->getState() == AudioPlayerBridge::PAUSED) {
        apb->togglePlayPause();
    } else if (currentSong == NULL && apb->getState() == AudioPlayerBridge::STOPPED && currentPlaylist != "" && currentPlaylistSongs.size() != 0) {
        currentSong = currentPlaylistSongs.at(0);
        api->getStreamKeyFromSongIDEx(currentSong->getSongId());
    } else if (apb->getState() == AudioPlayerBridge::STOPPED && currentSong != NULL) {
        api->getStreamKeyFromSongIDEx(currentSong->getSongId());
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
    QtConcurrent::run(apb, &AudioPlayerBridge::openAndPlay, info->directUrl());
}
