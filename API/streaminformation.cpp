#include "streaminformation.h"
#define QT_USE_FAST_OPERATOR_PLUS

StreamInformation::StreamInformation(QObject *parent) :
    QObject(parent)
{
}

std::string StreamInformation::directUrl() {
    return "http://" + ip + "/stream.php?streamKey=" + streamKey;
}

std::string StreamInformation::getUSecs() {
    return uSecs;
}

std::string StreamInformation::getStreamKey() {
    return streamKey;
}

std::string StreamInformation::getIp() {
    return ip;
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


