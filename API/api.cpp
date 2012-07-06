#include "api.h"
#include <QStringList>
#include "QJson/serializer.h"
#include "QJson/parser.h"
#include <QByteArray>
#include <QTime>
#define QT_USE_FAST_OPERATOR_PLUS

API::API(QObject *parent) :
    QObject(parent)
{
    util = new Util(this);
    uuid = QUuid::createUuid();
}

QString API::getSessionID() {
    QString header = util->getSetCookieHeaderWithProxy("http://grooveshark.com/");
    return header.split(';').at(0).split('=').at(1);
}

QString API::getCommunicationToken(QString sessionID) {
    QVariantMap mainMap = QVariantMap();
    QVariantMap headerMap = QVariantMap();
    headerMap.insert("privacy", 0);
    headerMap.insert("client", "htmlshark");
    headerMap.insert("clientRevision", "20120312");
    headerMap.insert("uuid", uuid.toString().replace("{", "").replace("}", "").toUpper());
    headerMap.insert("session", sessionID);
    headerMap.insert("country", getCountryMap());
    mainMap.insert("header", headerMap);
    QVariantMap parametersMap = QVariantMap();
    parametersMap.insert("secretKey", util->getMd5FromString(sessionID).toLower());
    mainMap.insert("parameters", parametersMap);
    mainMap.insert("method", "getCommunicationToken");
    QJson::Serializer serializer;
    QByteArray postData = serializer.serialize(mainMap);
    QString result = util->postData("https://grooveshark.com/more.php?getCommunicationToken", QString(postData));
    QJson::Parser parser;
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

QVariantMap API::getHeaderMap(QString sessionID, QString token, QString client, QString method) {
    QVariantMap headerMap = QVariantMap();
    headerMap.insert("uuid", uuid.toString().replace("{", "").replace("}", "").toUpper());
    headerMap.insert("country", getCountryMap());
    headerMap.insert("session", sessionID);
    headerMap.insert("client", client);
    QString salt = "";
    if (client == "htmlshark") {
        salt = ":breakfastBurritos:";
    } else if (client == "jsqueue") {
        salt = ":closeButNoCigar:";
    }
    srand(QTime::currentTime().msec());
    QString randNum = "";
    for (int i = 0; i < 6; i++) {
        int random = rand();
        while (random > 15) {
            random = random - 16;
        }
        randNum.append(QString::number(random, 16));
    }
    QString tokenReady = randNum + util->getSha1FromString(method + ":" + token + salt + randNum).toLower();
    headerMap.insert("token", tokenReady);
    return headerMap;
}
