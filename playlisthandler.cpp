#include "playlisthandler.h"
#include <QFile>
#include <QtSql/QSqlQuery>

PlaylistHandler::PlaylistHandler()
{
}

void PlaylistHandler::openDB() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("playlists.sqlite");
    db.open();
}

void PlaylistHandler::closeDB() {
    db.close();
}

void PlaylistHandler::deleteDB() {
    closeDB();
    QFile::remove("playlists.sqlite");
}

bool PlaylistHandler::createPlaylist(std::string name) {
    QSqlQuery query;
    bool ret = query.exec(QString("SELECT name FROM sqlite_master WHERE name = '%1' AND type = 'table';").arg(QString::fromStdString(name)));
    if (ret) {
        bool ret2 = query.first();
        if (ret2)
            return false;
        ret = query.exec(QString("CREATE TABLE %1 (id INT PRIMARY KEY NOT NULL AUTO INCREMENT, songName TEXT NOT NULL, artistName TEXT NOT NULL, albumName TEXT NOT NULL, songId INT NOT NULL, artistId INT NOT NULL, albumId INT NOT NULL, coverArtFilename TEXT);").arg(QString::fromStdString(name)));
        return true;
    }
    return false;
}
