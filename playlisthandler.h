#ifndef PLAYLISTHANDLER_H
#define PLAYLISTHANDLER_H
#include <QtSql/QSqlDatabase>

class PlaylistHandler
{
public:
    PlaylistHandler();
    void openDB();
    void closeDB();
    void deleteDB();
    bool createPlaylist(std::string name);

private:
    QSqlDatabase db;
};

#endif // PLAYLISTHANDLER_H
