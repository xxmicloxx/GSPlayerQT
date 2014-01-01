#ifndef PLAYLISTHANDLER_H
#define PLAYLISTHANDLER_H
#include <QtXml/qdom.h>
#include <API/song.h>
#include <vector>
#include <QObject>

class PlaylistHandler : public QObject
{
    Q_OBJECT
public:
    explicit PlaylistHandler(QObject *parent = 0);
    void redefinePlaylist(std::string playlistNane, std::vector<Song*> songs);
    bool createPlaylist(std::string name);
    void renamePlaylist(std::string oldName, std::string newName);
    bool checkIfPlaylistExists(std::string playlistName);
    std::vector<std::string> getPlaylists();
    bool addEntry(Song* song, std::string playlistName);
    std::vector<Song*> getSongs(std::string playlistName);
    void removePlaylist(std::string playlistName);
    void removeSong(Song* song, std::string playlistName);
    void save();
    void load();
    std::string getPlaylistId(std::string name);
    std::string getPlaylistById(std::string id);

signals:
    void playlistsChanged(std::vector<std::string> playlists);
    void songsChanged(std::string playlist, std::vector<Song*> songs);

private:
    QDomElement getPlaylistFromName(std::string playlistName);
    QDomDocument doc;
    QDomElement root;
    void checkInit();
    void init();
    bool used;
};

#endif // PLAYLISTHANDLER_H
