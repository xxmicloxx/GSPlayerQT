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

bool PlaylistHandler::addEntry(Song *song, std::string playlistName) {
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
    node = playlist.firstChild();
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
