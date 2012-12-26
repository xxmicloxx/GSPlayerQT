#include "api.h"
#include <QTime>
#include <QDebug>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <QEventLoop>
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
    connect(util, SIGNAL(dataPosted(int,std::string)), this, SLOT(gotSessionID(int,std::string)));
    connect(util, SIGNAL(dataPosted(int,std::string)), this, SLOT(gotToken(int,std::string)));
    connect(util, SIGNAL(dataPosted(int,std::string)), this, SLOT(gotCountry(int,std::string)));
    connect(util, SIGNAL(dataPosted(int,std::string)), this, SLOT(gotGSMethodAnswer(int,std::string)));
    connect(this, SIGNAL(methodExecuted(Value,int)), this, SLOT(gotResultsFromSongSearch(Value,int)));
    connect(this, SIGNAL(methodExecuted(Value,int)), this, SLOT(gotStreamKeyFromSongIDEx(Value,int)));
    connect(this, SIGNAL(methodExecuted(Value,int)), this, SLOT(gotResultsFromArtistSearch(Value,int)));
    connect(this, SIGNAL(methodExecuted(Value,int)), this, SLOT(gotPopularSongs(Value,int)));
    connect(this, SIGNAL(methodExecuted(Value,int)), this, SLOT(gotAllSongsByArtist(Value,int)));
    connect(this, SIGNAL(methodExecuted(Value,int)), this, SLOT(gotAllSongsByAlbum(Value,int)));
}

void API::makeFail() {
    token = "00000000000";
}

void API::getSessionID() {
    Value mainMap;
    mainMap["header"]["privacy"] = Value(0);
    mainMap["header"]["client"] = Value("htmlshark");
    mainMap["header"]["clientRevision"] = Value("20120830");
    mainMap["header"]["uuid"] = Value(boost::lexical_cast<std::string>(uuid));
    mainMap["parameters"] = Value(JsonBox::Object());
    mainMap["method"] = Value("initiateSession");
    std::stringstream ss;
    ss << mainMap;
    std::string postData = ss.str();
    util->postData("http://grooveshark.com/more.php?initiateSession", postData, 0);
}

void API::gotSessionID(int postActionId, std::string resultText) {
    if (postActionId != 0)
        return;
    Value resultRoot;
    resultRoot.loadFromString(resultText);
    sessionID = resultRoot["result"].getString();
    qDebug() << "Session: " << QString::fromStdString(sessionID);
    emit sessionInitialized();
}

void API::init() {
    connect(this, SIGNAL(sessionInitialized()), SLOT(getCommunicationToken()));
    connect(this, SIGNAL(tokenInitialized()), SLOT(getCountry()));
    connect(this, SIGNAL(countryInitialized()), SLOT(inited()));
    getSessionID();
}

void API::inited() {
    disconnect(SIGNAL(sessionInitialized()), this, SLOT(getCommunicationToken()));
    disconnect(SIGNAL(tokenInitialized()), this, SLOT(getCountry()));
    disconnect(SIGNAL(countryInitialized()), this, SLOT(inited()));
    emit firstConnected();
}

void API::checkConnect() {
    if (!connected) {
        connected = true;
        init();
    }
}


void API::getCommunicationToken() {
    Value mainMap;
    mainMap["header"]["privacy"] = Value(0);
    mainMap["header"]["client"] = Value("htmlshark");
    mainMap["header"]["clientRevision"] = Value("20120830");
    mainMap["header"]["uuid"] = Value(boost::lexical_cast<std::string>(uuid));
    mainMap["header"]["session"] = Value(sessionID);
    std::string data = util->getMd5FromString(sessionID);
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    mainMap["parameters"]["secretKey"] = Value(data);
    mainMap["method"] = Value("getCommunicationToken");
    std::stringstream ss;
    ss << mainMap;
    std::string postData = ss.str();
    util->postData("https://grooveshark.com/more.php?getCommunicationToken", postData, 1);
}

void API::gotToken(int postActionId, std::string resultText) {
    if (postActionId != 1)
        return;
    Value result_root;
    result_root.loadFromString(resultText);
    token = result_root["result"].getString();
    qDebug() << "CommunicationToken: " << QString::fromStdString(token);
    emit tokenInitialized();
}

void API::getCountry() {
    Value mainMap;
    mainMap["header"]["uuid"] = Value(boost::lexical_cast<std::string>(uuid));
    mainMap["header"]["session"] = Value(sessionID);
    mainMap["header"]["client"] = Value("jsqueue");
    mainMap["header"]["clientRevision"] = Value("20120830");
    mainMap["header"]["privacy"] = Value(0);
    std::string salt = ":birdStone:";
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
    util->postData("http://grooveshark.com/more.php", postData, 2);
}

void API::gotCountry(int postActionId, std::string resultText) {
    if (postActionId != 2)
        return;
    Value result_root;
    result_root.loadFromString(resultText);
    countryMap = result_root["result"];
    std::stringstream ss;
    ss << countryMap;
    qDebug() << "CountryMap: " << QString::fromStdString(ss.str());
    emit countryInitialized();
}

Value API::getHeaderMap(std::string client, std::string method) {
    Value headerMap;
    headerMap["uuid"] = Value(boost::lexical_cast<std::string>(uuid));
    headerMap["country"] = countryMap;
    headerMap["session"] = Value(sessionID);
    headerMap["client"] = Value(client);
    headerMap["clientRevision"] = Value("20120830");
    headerMap["privacy"] = Value(0);
    std::string salt = "";
    if (client == "htmlshark") {
        salt = ":tastyTacos:";
    } else if (client == "jsqueue") {
        salt = ":birdStone:";
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

void API::getStreamKeyFromSongIDEx(int songID) {
    lastSongId = songID;
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

    std::string postString = "{\"header\":{\"clientRevision\":\"20120830\",\"privacy\":0,\"client\":\"jsqueue\",\"uuid\":\"" +
            boost::lexical_cast<std::string>(uuid) +
            "\",\"token\":\"" +
            randNum + util->getSha1FromString("getStreamKeyFromSongIDEx:" + token + ":birdStone:" + randNum) +
            "\",\"country\":{\"CC3\":4294967296,\"DMA\":0,\"ID\":161,\"CC2\":0,\"IPR\":0,\"CC1\":0,\"CC4\":0},\"session\":\"" +
            sessionID +
            "\"},\"parameters\":{\"mobile\":false,\"country\":{\"CC3\":4294967296,\"DMA\":0,\"ID\":161,\"CC2\":0,\"CC1\":0,\"IPR\":0,\"CC4\":0},\"songID\":\"" +
            QString::number(songID).toStdString() +
            "\",\"type\":64,\"prefetch\":false},\"method\":\"getStreamKeyFromSongIDEx\"}";
    executeGroovesharkMethod(postString, "getStreamKeyFromSongIDEx", 3);
}

void API::gotStreamKeyFromSongIDEx(Value result, int postActionId) {
    if (postActionId != 3)
        return;
    if (result["error"].isNull() == false) {
        getStreamKeyFromSongIDEx(lastSongId);
        return;
    }
    std::stringstream ss;
    ss << result;
    if (result["streamKey"].isNull()) {
        emit songError(lastSongId);
        return;
    }
    StreamInformation* info = new StreamInformation(this);
    info->setUSecs(result["uSecs"].getString());
    info->setStreamKey(result["streamKey"].getString());
    info->setIp(result["ip"].getString());
    info->setSongId(lastSongId);
    emit streamKeyReady(info);
}

void API::executeGroovesharkMethod(Value mainMap, std::string method, int postResultId) {
    std::stringstream ss;
    ss << mainMap;
    std::string array = ss.str();
    executeGroovesharkMethod(array, method, postResultId);
}

void API::executeGroovesharkMethod(std::string array, std::string method, int postResultId) {
    qDebug() << "Request: " << QString::fromStdString(array);
    util->postData("http://grooveshark.com/more.php?" + method, array, postResultId);
}

void API::gotGSMethodAnswer(int postActionId, std::string resultText) {
    if (postActionId < 3)
        return;
    qDebug() << "Result: " << QString::fromStdString(resultText);
    Value resultMap;
    resultMap.loadFromString(resultText);
    if (!resultMap["fault"].isNull() && resultMap["fault"]["code"].getInt() == 256) {
        QEventLoop loop;
        connect(this, SIGNAL(tokenInitialized()), &loop, SLOT(quit()));
        getCommunicationToken();
        loop.exec();
        disconnect(this ,SIGNAL(tokenInitialized()), &loop, SLOT(quit()));
        Value returnVal;
        returnVal["error"] = true;
        emit methodExecuted(returnVal, postActionId);
        return;
    }
    emit methodExecuted(resultMap["result"], postActionId);
}

void API::getResultsFromSongSearch(std::string query) {
    lastSongSearch = query;
    Value mainMap;
    mainMap["header"] = getHeaderMap("htmlshark", "getResultsFromSearch");
    mainMap["method"] = Value("getResultsFromSearch");
    mainMap["parameters"]["guts"] = Value(0);
    mainMap["parameters"]["ppOverride"] = Value("");
    mainMap["parameters"]["query"] = Value(query);
    Array typeArray;
    typeArray.push_back("Songs");
    mainMap["parameters"]["type"] = Value(typeArray);
    executeGroovesharkMethod(mainMap, "getResultsFromSearch", 4);
}

void API::artistGetAllSongsEx(int artistId) {
    lastSongsByArtistId = artistId;
    Value mainMap;
    mainMap["header"] = getHeaderMap("htmlshark", "artistGetAllSongsEx");
    mainMap["method"] = Value("artistGetAllSongsEx");
    mainMap["parameters"]["artistID"] = Value(artistId);
    executeGroovesharkMethod(mainMap, "artistGetAllSongsEx", 7);
}

void API::gotAllSongsByArtist(Value result, int postActionId) {
    if (postActionId != 7)
        return;
    if (!result.isArray() && result["error"].isNull() == false) {
        artistGetAllSongsEx(lastSongsByArtistId);
        return;
    }
    std::vector<Song*> vector;
    int size = result.getArray().size();
    for (int i = 0; i < size; i++) {
        if (size > 1000 && result[i]["IsVerified"].getString() == "0")
            continue;
        Song* song = new Song(this);
        song->setSongName(result[i]["Name"].getString());
        song->setSongId(boost::lexical_cast<int>(result[i]["SongID"].getString()));
        song->setAlbumName(result[i]["AlbumName"].getString());
        song->setAlbumId(boost::lexical_cast<int>(result[i]["AlbumID"].getString()));
        song->setArtistName(result[i]["ArtistName"].getString());
        song->setArtistId(boost::lexical_cast<int>(result[i]["ArtistID"].getString()));
        vector.push_back(song);
    }
    emit songSearchCompleted(vector);
}

void API::albumGetAllSongs(int albumId) {
    lastSongsByAlbumId = albumId;
    Value mainMap;
    mainMap["header"] = getHeaderMap("htmlshark", "albumGetAllSongs");
    mainMap["method"] = Value("albumGetAllSongs");
    mainMap["parameters"]["albumID"] = Value(albumId);
    executeGroovesharkMethod(mainMap, "albumGetAllSongs", 8);
}

void API::gotAllSongsByAlbum(Value result, int postActionId) {
    if (postActionId != 8)
        return;
    if (!result.isArray() && result["error"].isNull() == false) {
        artistGetAllSongsEx(lastSongsByAlbumId);
        return;
    }
    std::vector<Song*> vector;
    int size = result.getArray().size();
    for (int i = 0; i < size; i++) {
        if (size > 1000 && result[i]["IsVerified"].getString() == "0")
            continue;
        Song* song = new Song(this);
        song->setSongName(result[i]["Name"].getString());
        song->setSongId(boost::lexical_cast<int>(result[i]["SongID"].getString()));
        song->setAlbumName(result[i]["AlbumName"].getString());
        song->setAlbumId(boost::lexical_cast<int>(result[i]["AlbumID"].getString()));
        song->setArtistName(result[i]["ArtistName"].getString());
        song->setArtistId(boost::lexical_cast<int>(result[i]["ArtistID"].getString()));
        vector.push_back(song);
    }
    emit songSearchCompleted(vector);
}

void API::gotResultsFromSongSearch(Value result, int postActionId) {
    if (postActionId != 4)
        return;
    if (result["error"].isNull() == false) {
        getResultsFromSongSearch(lastSongSearch);
        return;
    }
    std::vector<Song*> vector;
    int size = result["result"]["Songs"].getArray().size();
    for (int i = 0; i < size; i++) {
        Song* song = new Song(this);
        song->setSongName(result["result"]["Songs"][i]["SongName"].getString());
        song->setAlbumName(result["result"]["Songs"][i]["AlbumName"].getString());
        song->setArtistName(result["result"]["Songs"][i]["ArtistName"].getString());
        song->setCoverArtFilename(result["result"]["Songs"][i]["CoverArtFilename"].getString());
        song->setSongId(boost::lexical_cast<int>(result["result"]["Songs"][i]["SongID"].getString()));
        song->setAlbumId(boost::lexical_cast<int>(result["result"]["Songs"][i]["AlbumID"].getString()));
        song->setArtistId(boost::lexical_cast<int>(result["result"]["Songs"][i]["ArtistID"].getString()));
        vector.push_back(song);
    }
    emit songSearchCompleted(vector);
}

void API::getResultsFromArtistSearch(std::string query) {
    Value mainMap;
    mainMap["header"] = getHeaderMap("htmlshark", "getResultsFromSearch");
    mainMap["method"] = Value("getResultsFromSearch");
    mainMap["parameters"]["guts"] = Value(0);
    mainMap["parameters"]["ppOverride"] = Value("");
    mainMap["parameters"]["query"] = Value(query);
    Array typeArray;
    typeArray.push_back("Artists");
    mainMap["parameters"]["type"] = Value(typeArray);
    executeGroovesharkMethod(mainMap, "getResultsFromSearch", 5);
}

void API::gotResultsFromArtistSearch(Value result, int postActionId) {
    if (postActionId != 5)
        return;
    if (result["error"].isNull() == false) {
        getResultsFromArtistSearch(lastArtistSearch);
        return;
    }
    std::vector<Artist*> vector;
    int size = result["result"]["Artists"].getArray().size();
    for (int i = 0; i < size; i++) {
        Artist* artist = new Artist(this);
        artist->setArtistID(boost::lexical_cast<int>(result["result"]["Artists"][i]["ArtistID"].getString()));
        artist->setArtistName(result["result"]["Artists"][i]["ArtistName"].getString());
        vector.push_back(artist);
    }
    emit artistSearchCompleted(vector);
}

void API::popularGetSongs() {
    Value mainMap;
    mainMap["header"] = getHeaderMap("htmlshark", "popularGetSongs");
    mainMap["method"] = Value("popularGetSongs");
    mainMap["parameters"]["type"] = Value("daily");
    executeGroovesharkMethod(mainMap, "popularGetSongs", 6);
}

void API::gotPopularSongs(Value result, int postActionId) {
    if (postActionId != 6)
        return;
    if (result["error"].isNull() == false) {
        popularGetSongs();
        return;
    }
    std::vector<Song*> vector;
    for (unsigned int i = 0; i < result["Songs"].getArray().size(); i++) {
        Song* song = new Song(this);
        Value currData = result["Songs"][i];
        song->setSongName(currData["Name"].getString());
        song->setSongId(boost::lexical_cast<int>(currData["SongID"].getString()));
        song->setArtistName(currData["ArtistName"].getString());
        song->setArtistId(boost::lexical_cast<int>(currData["ArtistID"].getString()));
        song->setAlbumName(currData["AlbumName"].getString());
        song->setAlbumId(boost::lexical_cast<int>(currData["AlbumID"].getString()));
        song->setCoverArtFilename(currData["CoverArtFilename"].getString());
        vector.push_back(song);
    }
    emit popularSongSearchCompleted(vector);
}
