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
    bool createPlaylist(std::string name);
    std::vector<std::string> getPlaylists();
    bool addEntry(Song* song, std::string playlistName);
    void save();
    void load();

signals:
    void playlistsChanged(std::vector<std::string> playlists);

private:
    QDomDocument doc;
    QDomElement root;
    void checkInit();
    void init();
    bool used;
};

#endif // PLAYLISTHANDLER_H
