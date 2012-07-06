#ifndef API_H
#define API_H

#include <QObject>
#include "util.h"
#include <QUuid>
#include <QVariantMap>

class API : public QObject
{
    Q_OBJECT
public:
    explicit API(QObject *parent = 0);
    QString getSessionID();
    QString getCommunicationToken(QString sessionID);
    
signals:

private:
    Util* util;
    QUuid uuid;
    QVariantMap getCountryMap();
    QVariantMap getHeaderMap(QString sessionID, QString token, QString client, QString method);
    
public slots:
    
};

#endif // API_H
