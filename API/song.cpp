#include "song.h"

Song::Song(QObject *parent) :
    QObject(parent)
{
}

void Song::setSongId(int songId) {
    this->songId = songId;
}

void Song::setAlbumId(int albumId) {
    this->albumId = albumId;
}

void Song::setArtistId(int artistId) {
    this->artistId = artistId;
}

void Song::setSongName(std::string songName) {
    this->songName = songName;
}

void Song::setAlbumName(std::string albumName) {
    this->albumName = albumName;
}

void Song::setArtistName(std::string artistName) {
    this->artistName = artistName;
}

void Song::setCoverArtFilename(std::string coverArtFilename) {
    this->coverArtFilename = coverArtFilename;
}

int Song::getSongId() {
    return songId;
}

int Song::getAlbumId() {
    return albumId;
}

int Song::getArtistId() {
    return artistId;
}

std::string Song::getSongName() {
    return songName;
}

std::string Song::getAlbumName() {
    return albumName;
}

std::string Song::getArtistName() {
    return artistName;
}

std::string Song::getCoverArtFilename() {
    if (coverArtFilename == "")
        return "album.png";
    return coverArtFilename;
}
