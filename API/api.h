#ifndef API_H
#define API_H

#include <QObject>
#include "util.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include "streaminformation.h"
#include <JsonBox.h>
#include "song.h"

using namespace JsonBox;

class API : public QObject
{
    Q_OBJECT
public:
    explicit API(QObject *parent = 0);
    std::string getSessionID();
    std::string getCommunicationToken();
    StreamInformation* getStreamKeyFromSongIDEx(int songID);
    std::vector<Song*> getResultsFromSongSearch(std::string query);
    void checkConnect();
    
signals:

private:
    void init();
    bool connected;
    Util* util;
    std::string uuid;
    Value getCountryMap();
    Value getHeaderMap(std::string client, std::string method);
    Value executeGroovesharkMethod(Value mainMap, std::string method);
    Value executeGroovesharkMethod(std::string array, std::string method);
    std::string sessionID;
    std::string token;
    
public slots:
    
};

#endif // API_H
