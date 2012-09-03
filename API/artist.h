#ifndef ARTIST_H
#define ARTIST_H

#include <QObject>

class Artist : public QObject
{
    Q_OBJECT
public:
    explicit Artist(QObject *parent = 0);
    void setArtistID(int artistID);
    void setArtistName(std::string artistName);
    int getArtistID();
    std::string getArtistName();
    
private:
    int artistID;
    std::string artistName;

signals:
    
public slots:
    
};

#endif // ARTIST_H
