#include "playlisthandler.h"
#include <QFile>
#include <QTextStream>
#include <boost/lexical_cast.hpp>

PlaylistHandler::PlaylistHandler(QObject *parent) : QObject(parent)
{
    used = false;
}

void PlaylistHandler::save() {
    if (!used)
        return;
    QFile file("Playlists.xml");
    file.open(QIODevice::WriteOnly);
    QTextStream ts(&file);
    ts << doc.toString();
    file.close();
}

void PlaylistHandler::redefinePlaylist(std::string playlistName, std::vector<Song*> songs) {
    checkInit();
    QDomElement playlist = getPlaylistFromName(playlistName);
    QDomNode node = playlist.firstChild();
    std::vector<QDomElement> removeVector;
    while (!node.isNull()) {
        QDomElement element = node.toElement();
        if (!element.isNull() && element.tagName() == "Song") {
            removeVector.push_back(element);
        }
        node = node.nextSibling();
    }
    for (unsigned int i = 0; i < removeVector.size(); i++) {
        playlist.removeChild(removeVector.at(i));
    }
    save();
    for (unsigned int i = 0; i < songs.size(); i++) {
        Song* song = songs.at(i);
        QDomElement songElement = doc.createElement("Song");
        songElement.setAttribute("SongName", QString::fromStdString(song->getSongName()));
        songElement.setAttribute("ArtistName", QString::fromStdString(song->getArtistName()));
        songElement.setAttribute("AlbumName", QString::fromStdString(song->getAlbumName()));
        songElement.setAttribute("SongId", song->getSongId());
        songElement.setAttribute("ArtistId", song->getArtistId());
        songElement.setAttribute("AlbumId", song->getAlbumId());
        songElement.setAttribute("CoverArtFilename", QString::fromStdString(song->getCoverArtFilename()));
        playlist.appendChild(songElement);
    }
    save();
    emit songsChanged(playlistName, getSongs(playlistName));
}

void PlaylistHandler::renamePlaylist(std::string oldName, std::string newName) {
    checkInit();
    QDomNode node = root.firstChild();
    QDomElement renameElement;
    while (!node.isNull()) {
        QDomElement element = node.toElement();
        if (!element.isNull() && element.tagName() == "Playlist" && element.attribute("name").toStdString() == oldName) {
            renameElement = element;
        }
        node = node.nextSibling();
    }
    renameElement.setAttribute("name", QString::fromStdString(newName));
    save();
    emit playlistsChanged(getPlaylists());
}

bool PlaylistHandler::checkIfPlaylistExists(std::string playlistName) {
    checkInit();
    QDomNode node = root.firstChild();
    while (!node.isNull()) {
        QDomElement element = node.toElement();
        if (!element.isNull()) {
            if (element.tagName() == "Playlist") {
                if (element.attribute("name").toStdString() == playlistName) {
                    return true;
                }
            }
        }
        node = node.nextSibling();
    }
    return false;
}

bool PlaylistHandler::createPlaylist(std::string name) {
    checkInit();
    QDomNode node = root.firstChild();
    while (!node.isNull()) {
        QDomElement element = node.toElement();
        if (!element.isNull()) {
            if (element.tagName() == "Playlist") {
                if (element.attribute("name").toStdString() == name) {
                    return false;
                }
            }
        }
        node = node.nextSibling();
    }
    QDomElement thisPlaylist = doc.createElement("Playlist");
    thisPlaylist.setAttribute("name", QString::fromStdString(name));
    root.appendChild(thisPlaylist);
    save();
    emit playlistsChanged(getPlaylists());
    return true;
}

std::vector<std::string> PlaylistHandler::getPlaylists() {
    checkInit();
    std::vector<std::string> vector;
    QDomNode node = root.firstChild();
    while (!node.isNull()) {
        QDomElement element = node.toElement();
        if (!element.isNull() && element.tagName() == "Playlist") {
            vector.push_back(element.attribute("name").toStdString());
        }
        node = node.nextSibling();
    }
    return vector;
}

QDomElement PlaylistHandler::getPlaylistFromName(std::string playlistName) {
    checkInit();
    QDomElement playlist;
    QDomNode node = root.firstChild();
    while (!node.isNull()) {
        QDomElement element = node.toElement();
        if (!element.isNull() && element.tagName() == "Playlist" && element.attribute("name").toStdString() == playlistName) {
            playlist = element;
        }
        node = node.nextSibling();
    }
    return playlist;
}

std::vector<Song*> PlaylistHandler::getSongs(std::string playlistName) {
    checkInit();
    if (playlistName == "") {
        std::vector<Song*> emptyVector;
        return emptyVector;
    }
    QDomElement playlist = getPlaylistFromName(playlistName);
    std::vector<Song*> songVector;
    QDomNode node = playlist.firstChild();
    while (!node.isNull()) {
        QDomElement element = node.toElement();
        if (!element.isNull() && element.tagName() == "Song") {
            Song* song = new Song(this);
            song->setSongName(element.attribute("SongName").toStdString());
            song->setSongId(element.attribute("SongId").toInt());
            song->setArtistName(element.attribute("ArtistName").toStdString());
            song->setArtistId(element.attribute("ArtistId").toInt());
            song->setAlbumName(element.attribute("AlbumName").toStdString());
            song->setAlbumId(element.attribute("AlbumId").toInt());
            song->setCoverArtFilename(element.attribute("CoverArtFilename").toStdString());
            songVector.push_back(song);
        }
        node = node.nextSibling();
    }
    return songVector;
}

void PlaylistHandler::removePlaylist(std::string playlistName) {
    checkInit();
    QDomNode node = root.firstChild();
    QDomNode removalNode;
    while (!node.isNull()) {
        QDomElement element = node.toElement();
        if (!element.isNull() && element.tagName() == "Playlist" && element.attribute("name").toStdString() == playlistName) {
            removalNode = node;
        }
        node = node.nextSibling();
    }
    root.removeChild(removalNode);
    save();
    emit playlistsChanged(getPlaylists());
}

void PlaylistHandler::removeSong(Song *song, std::string playlistName) {
    checkInit();
    QDomElement playlist = getPlaylistFromName(playlistName);
    QDomNode node = playlist.firstChild();
    QDomNode removalNode;
    while (!node.isNull()) {
        QDomElement element = node.toElement();
        if (!element.isNull() && element.tagName() == "Song" && element.attribute("SongId").toStdString() == boost::lexical_cast<std::string>(song->getSongId())) {
            removalNode = node;
        }
        node = node.nextSibling();
    }
    playlist.removeChild(removalNode);
    save();
    emit songsChanged(playlistName, getSongs(playlistName));
}

bool PlaylistHandler::addEntry(Song *song, std::string playlistName) {
    checkInit();
    QDomElement playlist = getPlaylistFromName(playlistName);
    QDomNode node = playlist.firstChild();
    while (!node.isNull()) {
        QDomElement element = node.toElement();
        if (!element.isNull() && element.tagName() == "Song" && element.attribute("SongId").toStdString() == boost::lexical_cast<std::string>(song->getSongId()))
            return false;
        node = node.nextSibling();
    }
    QDomElement songElement = doc.createElement("Song");
    songElement.setAttribute("SongName", QString::fromStdString(song->getSongName()));
    songElement.setAttribute("ArtistName", QString::fromStdString(song->getArtistName()));
    songElement.setAttribute("AlbumName", QString::fromStdString(song->getAlbumName()));
    songElement.setAttribute("SongId", song->getSongId());
    songElement.setAttribute("ArtistId", song->getArtistId());
    songElement.setAttribute("AlbumId", song->getAlbumId());
    songElement.setAttribute("CoverArtFilename", QString::fromStdString(song->getCoverArtFilename()));
    playlist.appendChild(songElement);
    save();
    emit songsChanged(playlistName, getSongs(playlistName));
    return true;
}

void PlaylistHandler::checkInit() {
    if (used)
        return;
    used = true;
    doc = QDomDocument("GSPlayerPL");
    if (!QFile::exists("Playlists.xml")) {
        init();
    } else {
        load();
    }
}

void PlaylistHandler::init() {
    root = doc.createElement("Playlists");
    doc.appendChild(root);
    save();
}

void PlaylistHandler::load() {
    QFile file("Playlists.xml");
    file.open(QIODevice::ReadOnly);
    doc.setContent(file.readAll());
    file.close();
    root = doc.documentElement();
}
