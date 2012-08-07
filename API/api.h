#ifndef API_H
#define API_H

#include <QObject>
#include "util.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <QVariantMap>
#include "streaminformation.h"

class API : public QObject
{
    Q_OBJECT
public:
    explicit API(QObject *parent = 0);
    QString getSessionID();
    QString getCommunicationToken();
    StreamInformation* getStreamKeyFromSongIDEx(QString songID);
    void checkConnect();
    
signals:

private:
    void init();
    bool connected;
    Util* util;
    std::string uuid;
    QVariantMap getCountryMap();
    QVariantMap getHeaderMap(QString client, QString method);
    QVariantMap executeGroovesharkMethod(QVariantMap mainMap, QString method);
    QVariantMap executeGroovesharkMethod(QByteArray array, QString method);
    QString sessionID;
    QString token;
    
public slots:
    
};

#endif // API_H
