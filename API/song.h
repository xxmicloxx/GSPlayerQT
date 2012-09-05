#ifndef SONG_H
#define SONG_H

#include <QObject>

class Song : public QObject
{
    Q_OBJECT
public:
    explicit Song(QObject *parent = 0);
    void setSongId(int songId);
    void setAlbumId(int albumId);
    void setArtistId(int artistId);
    void setSongName(std::string songName);
    void setAlbumName(std::string albumName);
    void setArtistName(std::string artistName);
    void setCoverArtFilename(std::string coverArtFilename);
    int getSongId();
    int getAlbumId();
    int getArtistId();
    std::string getSongName();
    std::string getAlbumName();
    std::string getArtistName();
    std::string getCoverArtFilename();
    
private:
    int songId;
    int albumId;
    int artistId;
    std::string songName;
    std::string artistName;
    std::string albumName;
    std::string coverArtFilename;

signals:
    
public slots:
    
};

#endif // SONG_H
