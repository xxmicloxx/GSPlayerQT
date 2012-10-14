#include "streaminformation.h"
#include <boost/lexical_cast.hpp>
#define QT_USE_FAST_OPERATOR_PLUS

StreamInformation::StreamInformation(QObject *parent) :
    QObject(parent)
{
}

std::string StreamInformation::directUrl() {
    return "http://" + ip + "/stream.php?streamKey=" + streamKey;
}

int StreamInformation::getUSecs() {
    return boost::lexical_cast<int>(uSecs);
}

std::string StreamInformation::getStreamKey() {
    return streamKey;
}

std::string StreamInformation::getIp() {
    return ip;
}

int StreamInformation::getSongId() {
    return songId;
}

void StreamInformation::setUSecs(std::string set) {
    uSecs = set;
}

void StreamInformation::setStreamKey(std::string set) {
    streamKey = set;
}

void StreamInformation::setIp(std::string set) {
    ip = set;
}

void StreamInformation::setSongId(int set) {
    songId = set;
}
