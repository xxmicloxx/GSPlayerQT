#include "api.h"
#include <QTime>
#include <QDebug>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#define QT_USE_FAST_OPERATOR_PLUS

API::API(QObject *parent) :
    QObject(parent)
{
    util = new Util(this);
    boost::uuids::uuid uuidobj = boost::uuids::random_generator()();
    std::stringstream ss;
    ss << uuidobj;
    uuid = ss.str();
    connected = false;
}

std::string API::getSessionID() {
    Value mainMap;
    mainMap["header"]["privacy"] = Value(0);
    mainMap["header"]["client"] = Value("htmlshark");
    mainMap["header"]["clientRevision"] = Value("20120312");
    mainMap["header"]["uuid"] = Value(boost::lexical_cast<std::string>(uuid));
    mainMap["parameters"] = Value(JsonBox::Object());
    mainMap["method"] = Value("initiateSession");
    std::stringstream ss;
    ss << mainMap;
    std::string postData = ss.str();
    std::string result = util->postData("https://grooveshark.com/more.php?initiateSession", postData);
    Value resultRoot;
    resultRoot.loadFromString(result);
    sessionID = resultRoot["result"].getString();
    qDebug() << "Session: " << QString::fromStdString(sessionID);
    return sessionID;
}

void API::init() {
    getSessionID();
    getCommunicationToken();
    getCountry();
}

void API::checkConnect() {
    if (!connected) {
        connected = true;
        init();
    }
}


std::string API::getCommunicationToken() {
    Value mainMap;
    mainMap["header"]["privacy"] = Value(0);
    mainMap["header"]["client"] = Value("htmlshark");
    mainMap["header"]["clientRevision"] = Value("20120312");
    mainMap["header"]["uuid"] = Value(boost::lexical_cast<std::string>(uuid));
    mainMap["header"]["session"] = Value(sessionID);
    std::string data = util->getMd5FromString(sessionID);
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    mainMap["parameters"]["secretKey"] = Value(data);
    mainMap["method"] = Value("getCommunicationToken");
    std::stringstream ss;
    ss << mainMap;
    std::string postData = ss.str();
    std::string result = util->postData("https://grooveshark.com/more.php?getCommunicationToken", postData);
    Value result_root;
    result_root.loadFromString(result);
    token = result_root["result"].getString();
    qDebug() << "CommunicationToken: " << QString::fromStdString(token);
    return token;
}

void API::getCountry() {
    Value mainMap;
    mainMap["header"]["uuid"] = Value(boost::lexical_cast<std::string>(uuid));
    mainMap["header"]["session"] = Value(sessionID);
    mainMap["header"]["client"] = Value("jsqueue");
    mainMap["header"]["clientRevision"] = Value("20120312.02");
    mainMap["header"]["privacy"] = Value(0);
    std::string salt = ":closeButNoCigar:";
    srand(QTime::currentTime().msec());
    std::string randNum = "";
    for (int i = 0; i < 6; i++) {
        int random = rand();
        while (random > 15) {
            random = random - 16;
        }
        std::ostringstream oss;
        oss << std::hex << random;
        randNum.append(oss.str());
    }
    std::string tokenReady = randNum + util->getSha1FromString("getCountry:" + token + salt + randNum);
    qDebug() << "Calculated Token: " << QString::fromStdString(tokenReady);
    mainMap["header"]["token"] = Value(tokenReady);
    mainMap["parameters"] = Value(JsonBox::Object());
    mainMap["method"] = Value("getCountry");
    std::stringstream ss;
    ss << mainMap;
    std::string postData = ss.str();
    std::string result = util->postData("http://grooveshark.com/more.php?getCountry", postData);
    Value result_root;
    result_root.loadFromString(result);
    countryMap = result_root["result"];
}

Value API::getHeaderMap(std::string client, std::string method) {
    Value headerMap;
    headerMap["uuid"] = Value(boost::lexical_cast<std::string>(uuid));
    headerMap["country"] = countryMap;
    headerMap["session"] = Value(sessionID);
    headerMap["client"] = Value(client);
    headerMap["clientRevision"] = Value("20120312");
    headerMap["privacy"] = Value(0);
    std::string salt = "";
    if (client == "htmlshark") {
        salt = ":breakfastBurritos:";
    } else if (client == "jsqueue") {
        salt = ":closeButNoCigar:";
    }
    srand(QTime::currentTime().msec());
    std::string randNum = "";
    for (int i = 0; i < 6; i++) {
        int random = rand();
        while (random > 15) {
            random = random - 16;
        }
        std::ostringstream oss;
        oss << std::hex << random;
        randNum.append(oss.str());
    }
    std::string tokenReady = randNum + util->getSha1FromString(method + ":" + token + salt + randNum);
    qDebug() << "Calculated Token: " << QString::fromStdString(tokenReady);
    headerMap["token"] = Value(tokenReady);
    return headerMap;
}

StreamInformation* API::getStreamKeyFromSongIDEx(int songID) {
    checkConnect();
    srand(QTime::currentTime().msec());
    std::string randNum = "";
    for (int i = 0; i < 6; i++) {
        int random = rand();
        while (random > 15) {
            random = random - 16;
        }
        std::ostringstream oss;
        oss << std::hex << random;
        randNum.append(oss.str());
    }

    std::string postString = "{\"header\":{\"clientRevision\":\"20120312.02\",\"privacy\":0,\"client\":\"jsqueue\",\"uuid\":\"" +
            boost::lexical_cast<std::string>(uuid) +
            "\",\"token\":\"" +
            randNum + util->getSha1FromString("getStreamKeyFromSongIDEx:" + token + ":closeButNoCigar:" + randNum) +
            "\",\"country\":{\"CC3\":4294967296,\"DMA\":0,\"ID\":161,\"CC2\":0,\"IPR\":0,\"CC1\":0,\"CC4\":0},\"session\":\"" +
            sessionID +
            "\"},\"parameters\":{\"mobile\":false,\"country\":{\"CC3\":4294967296,\"DMA\":0,\"ID\":161,\"CC2\":0,\"CC1\":0,\"IPR\":0,\"CC4\":0},\"songID\":\"" +
            QString::number(songID).toStdString() +
            "\",\"type\":64,\"prefetch\":false},\"method\":\"getStreamKeyFromSongIDEx\"}";
    Value result = executeGroovesharkMethod(postString, "getStreamKeyFromSongIDEx");
    StreamInformation* info = new StreamInformation(this);
    info->setUSecs(result["uSecs"].getString());
    info->setStreamKey(result["streamKey"].getString());
    info->setIp(result["ip"].getString());
    return info;
}

Value API::executeGroovesharkMethod(Value mainMap, std::string method) {
    std::stringstream ss;
    ss << mainMap;
    std::string array = ss.str();
    return executeGroovesharkMethod(array, method);
}

Value API::executeGroovesharkMethod(std::string array, std::string method) {
    qDebug() << "Request: " << QString::fromStdString(array);
    std::string result = util->postData("http://grooveshark.com/more.php?" + method, array);
    qDebug() << "Result: " << QString::fromStdString(result);
    Value resultMap;
    resultMap.loadFromString(result);
    if (!resultMap["fault"].isNull() && resultMap["fault"]["code"].getInt() == 256) {
        getCommunicationToken();
        return executeGroovesharkMethod(array, method);
    }
    return resultMap["result"];
}

std::vector<Song*> API::getResultsFromSongSearch(std::string query) {
    Value mainMap;
    mainMap["header"] = getHeaderMap("htmlshark", "getResultsFromSearch");
    mainMap["method"] = Value("getResultsFromSearch");
    mainMap["parameters"]["guts"] = Value(0);
    mainMap["parameters"]["ppOverride"] = Value("");
    mainMap["parameters"]["query"] = Value(query);
    Array typeArray;
    typeArray.push_back("Songs");
    mainMap["parameters"]["type"] = Value(typeArray);
    Value result = executeGroovesharkMethod(mainMap, "getResultsFromSearch");
    std::vector<Song*> vector;
    int size = result["result"]["Songs"].getArray().size();
    for (int i = 0; i < size; i++) {
        Song* song = new Song(this);
        song->setSongName(result["result"]["Songs"][i]["SongName"].getString());
        song->setAlbumName(result["result"]["Songs"][i]["AlbumName"].getString());
        song->setArtistName(result["result"]["Songs"][i]["ArtistName"].getString());
        song->setSongId(boost::lexical_cast<int>(result["result"]["Songs"][i]["SongID"].getString()));
        song->setAlbumId(boost::lexical_cast<int>(result["result"]["Songs"][i]["AlbumID"].getString()));
        song->setArtistId(boost::lexical_cast<int>(result["result"]["Songs"][i]["ArtistID"].getString()));
        vector.push_back(song);
    }
    return vector;
}

std::vector<Artist*> API::getResultsFromArtistSearch(std::string query) {
    Value mainMap;
    mainMap["header"] = getHeaderMap("htmlshark", "getResultsFromSearch");
    mainMap["method"] = Value("getResultsFromSearch");
    mainMap["parameters"]["guts"] = Value(0);
    mainMap["parameters"]["ppOverride"] = Value("");
    mainMap["parameters"]["query"] = Value(query);
    Array typeArray;
    typeArray.push_back("Artists");
    mainMap["parameters"]["type"] = Value(typeArray);
    Value result = executeGroovesharkMethod(mainMap, "getResultsFromSearch");
    std::vector<Artist*> vector;
    int size = result["result"]["Artists"].getArray().size();
    for (int i = 0; i < size; i++) {
        Artist* artist = new Artist(this);
        artist->setArtistID(boost::lexical_cast<int>(result["result"]["Artists"][i]["ArtistID"].getString()));
        artist->setArtistName(result["result"]["Artists"][i]["ArtistName"].getString());
        vector.push_back(artist);
    }
    return vector;
}
