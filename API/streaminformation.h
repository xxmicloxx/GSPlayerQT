#ifndef STREAMINFORMATION_H
#define STREAMINFORMATION_H

#include <QObject>

class StreamInformation : public QObject
{
    Q_OBJECT
public:
    explicit StreamInformation(QObject *parent = 0);
    std::string directUrl();
    std::string getUSecs();
    void setUSecs(std::string set);
    std::string getStreamKey();
    void setStreamKey(std::string set);
    std::string getIp();
    void setIp(std::string set);

private:
    std::string uSecs;
    std::string streamKey;
    std::string ip;

signals:
    
public slots:
    
};

#endif // STREAMINFORMATION_H
