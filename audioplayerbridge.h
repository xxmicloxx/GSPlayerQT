#ifndef AUDIOPLAYERBRIDGE_H
#define AUDIOPLAYERBRIDGE_H

#include <QObject>

class AudioPlayerBridge : public QObject
{
    Q_OBJECT
public:
    explicit AudioPlayerBridge(QObject *parent = 0);
    void openAndPlay(std::string path);
    
signals:
    
public slots:
    
};

#endif // AUDIOPLAYERBRIDGE_H
