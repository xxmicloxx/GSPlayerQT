#ifndef API_H
#define API_H

#include <QObject>
#include "util.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include "streaminformation.h"
#include <JsonBox.h>
#include "song.h"
#include "artist.h"

using namespace JsonBox;

class API : public QObject
{
    Q_OBJECT
public:
    enum ResultType {
        Songs, Artists
    };
    explicit API(QObject *parent = 0);
    void getSessionID();
    std::string getCommunicationToken();
    void getCountry();
    StreamInformation* getStreamKeyFromSongIDEx(int songID);
    std::vector<Song*> getResultsFromSongSearch(std::string query);
    std::vector<Artist*> getResultsFromArtistSearch(std::string query);
    void checkConnect();
    
signals:

private:
    void init();
    bool connected;
    Util* util;
    std::string uuid;
    Value countryMap;
    Value getHeaderMap(std::string client, std::string method);
    Value executeGroovesharkMethod(Value mainMap, std::string method, int postResultId);
    Value executeGroovesharkMethod(std::string array, std::string method, int postResultId);
    std::string sessionID;
    std::string token;
    
public slots:
    void gotSessionID(int postActionId, std::string resultText);
};

#endif // API_H
