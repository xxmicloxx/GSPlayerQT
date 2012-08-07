#include "api.h"
#include <QStringList>
#include "QJson/serializer.h"
#include "QJson/parser.h"
#include <QByteArray>
#include <QTime>
#include <QVariant>
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

QString API::getSessionID() {
    QVariantMap mainMap = QVariantMap();
    QVariantMap headerMap = QVariantMap();
    headerMap.insert("privacy", 0);
    headerMap.insert("client", "htmlshark");
    headerMap.insert("clientRevision", "20120312");
    headerMap.insert("uuid", boost::lexical_cast<std::string>(uuid).c_str());
    headerMap.insert("country", getCountryMap());
    mainMap.insert("header", headerMap);
    QVariantMap parametersMap = QVariantMap();
    mainMap.insert("parameters", parametersMap);
    mainMap.insert("method", "initiateSession");
    QJson::Serializer serializer;
    QByteArray postData = serializer.serialize(mainMap);
    std::string result = util->postData("https://grooveshark.com/more.php?initiateSession", QString(postData).toAscii().data());
    QJson::Parser parser;
    sessionID = parser.parse(QString::fromStdString(result).toAscii()).toMap()["result"].toString();
    qDebug() << "Session: " << sessionID;
    return parser.parse(QString::fromStdString(result).toAscii()).toMap()["result"].toString();
}

void API::init() {
    getSessionID();
    getCommunicationToken();
}

void API::checkConnect() {
    if (!connected) {
        connected = true;
        init();
    }
}


QString API::getCommunicationToken() {
    QVariantMap mainMap = QVariantMap();
    QVariantMap headerMap = QVariantMap();
    headerMap.insert("privacy", 0);
    headerMap.insert("client", "htmlshark");
    headerMap.insert("clientRevision", "20120312");
    headerMap.insert("uuid", QString(boost::lexical_cast<std::string>(uuid).c_str()));
    headerMap.insert("session", sessionID);
    headerMap.insert("country", getCountryMap());
    mainMap.insert("header", headerMap);
    QVariantMap parametersMap = QVariantMap();
    std::string data = util->getMd5FromString(sessionID.toAscii().data());
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    parametersMap.insert("secretKey", QString(data.c_str()));
    mainMap.insert("parameters", parametersMap);
    mainMap.insert("method", "getCommunicationToken");
    QJson::Serializer serializer;
    QByteArray postData = serializer.serialize(mainMap);
    qDebug() << QString(postData);
    QString result = QString::fromStdString(util->postData("https://grooveshark.com/more.php?getCommunicationToken", std::string(QString(postData).toAscii().data())));
    qDebug() << result;
    QJson::Parser parser;
    token = parser.parse(result.toAscii()).toMap()["result"].toString();
    qDebug() << "CommunicationToken: " << token;
    return parser.parse(result.toAscii()).toMap()["result"].toString();
}

QVariantMap API::getCountryMap() {
    QVariantMap countryMap = QVariantMap();
    countryMap.insert("IPR", "9075");
    countryMap.insert("CC2", "0");
    countryMap.insert("ID", "55");
    countryMap.insert("CC1", "18014398509481984");
    countryMap.insert("CC3", "0");
    countryMap.insert("CC4", "0");
    return countryMap;
}

QVariantMap API::getHeaderMap(QString client, QString method) {
    QVariantMap headerMap = QVariantMap();
    headerMap.insert("uuid", boost::lexical_cast<std::string>(uuid).c_str());
    headerMap.insert("country", getCountryMap());
    headerMap.insert("session", sessionID);
    headerMap.insert("client", client);
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
    std::string tokenReady = randNum + util->getSha1FromString(std::string(method.toAscii().data()) + ":" + std::string(token.toAscii().data()) + salt + randNum);
    qDebug() << "Calculated Token: " << QString::fromStdString(tokenReady);
    headerMap.insert("token", tokenReady.c_str());
    return headerMap;
}

StreamInformation* API::getStreamKeyFromSongIDEx(QString songID) {
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
            randNum + util->getSha1FromString("getStreamKeyFromSongIDEx:" + std::string(token.toAscii().data()) + ":closeButNoCigar:" + randNum) +
            "\",\"country\":{\"CC3\":4294967296,\"DMA\":0,\"ID\":161,\"CC2\":0,\"IPR\":0,\"CC1\":0,\"CC4\":0},\"session\":\"" +
            sessionID.toAscii().data() +
            "\"},\"parameters\":{\"mobile\":false,\"country\":{\"CC3\":4294967296,\"DMA\":0,\"ID\":161,\"CC2\":0,\"CC1\":0,\"IPR\":0,\"CC4\":0},\"songID\":" +
            songID.toAscii().data() +
            ",\"type\":64,\"prefetch\":false},\"method\":\"getStreamKeyFromSongIDEx\"}";
    QVariantMap result = executeGroovesharkMethod(QString::fromStdString(postString).toAscii(), "getStreamKeyFromSongIDEx");
    StreamInformation* info = new StreamInformation(this);
    info->setUSecs(result["uSecs"].toString().toAscii().data());
    info->setStreamKey(result["streamKey"].toString().toAscii().data());
    info->setIp(result["ip"].toString().toAscii().data());
    return info;
}

QVariantMap API::executeGroovesharkMethod(QVariantMap mainMap, QString method) {
    QJson::Serializer serializer;
    QByteArray postData = serializer.serialize(mainMap);
    return executeGroovesharkMethod(postData, method);
}

QVariantMap API::executeGroovesharkMethod(QByteArray array, QString method) {
    qDebug() << "Request: " << QString(array);
    std::string result = util->postData("http://grooveshark.com/more.php?" + std::string(method.toAscii().data()), QString(array).toAscii().data());
    qDebug() << "Result: " << result.c_str();
    QJson::Parser parser;
    QVariantMap resultMap = parser.parse(QString::fromStdString(result).toAscii()).toMap();
    if (resultMap["fault"].isNull() == false && resultMap["fault"].toMap()["code"] == 256) {
        getCommunicationToken();
        return executeGroovesharkMethod(array, method);
    }
    return resultMap["result"].toMap();
}
