#include "artist.h"

Artist::Artist(QObject *parent) :
    QObject(parent)
{
}

void Artist::setArtistID(int artistID) {
    this->artistID = artistID;
}

void Artist::setArtistName(std::string artistName) {
    this->artistName = artistName;
}

int Artist::getArtistID() {
    return artistID;
}

std::string Artist::getArtistName() {
    return artistName;
}
