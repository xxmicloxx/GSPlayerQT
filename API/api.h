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
    void getStreamKeyFromSongIDEx(int songID);
    void getResultsFromSongSearch(std::string query);
    void getResultsFromArtistSearch(std::string query);
    void checkConnect();
    void popularGetSongs();
    void makeFail();
    
signals:
    void sessionInitialized();
    void tokenInitialized();
    void countryInitialized();
    void methodExecuted(Value value, int postResultId);
    void songSearchCompleted(std::vector<Song*>);
    void artistSearchCompleted(std::vector<Artist*>);
    void popularSongSearchCompleted(std::vector<Song*>);
    void streamKeyReady(StreamInformation* info);

private:
    void init();
    bool connected;
    std::string lastSongSearch;
    std::string lastArtistSearch;
    int lastSongId;
    Util* util;
    std::string uuid;
    Value countryMap;
    Value getHeaderMap(std::string client, std::string method);
    void executeGroovesharkMethod(Value mainMap, std::string method, int postResultId);
    void executeGroovesharkMethod(std::string array, std::string method, int postResultId);
    std::string sessionID;
    std::string token;
    
public slots:
    void getSessionID();
    void getCommunicationToken();
    void getCountry();

private slots:
    void gotSessionID(int postActionId, std::string resultText);
    void gotToken(int postActionId, std::string resultText);
    void gotCountry(int postActionId, std::string resultText);
    void inited();
    void gotGSMethodAnswer(int postActionId, std::string resultText);
    void gotResultsFromSongSearch(Value result, int postActionId);
    void gotResultsFromArtistSearch(Value result, int postActionId);
    void gotStreamKeyFromSongIDEx(Value result, int postActionId);
    void gotPopularSongs(Value result, int postActionId);
};

#endif // API_H
