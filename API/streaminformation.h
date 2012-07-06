#ifndef STREAMINFORMATION_H
#define STREAMINFORMATION_H

#include <QObject>

class StreamInformation : public QObject
{
    Q_OBJECT
public:
    explicit StreamInformation(QObject *parent = 0);
    QString directUrl();
    QString uSecs();
    void setUSecs(QString set);
    QString streamKey();
    void setStreamKey(QString set);
    QString ip();
    void setIp(QString set);

private:
    QString uSecs;
    QString streamKey;
    QString ip;

signals:
    
public slots:
    
};

#endif // STREAMINFORMATION_H
