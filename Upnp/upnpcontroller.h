#ifndef UPNPCONTROLLER_H
#define UPNPCONTROLLER_H

#include <QObject>
#include "Platinum.h"
#include <QMutex>
#include <QMap>
#include "player.h"
#include "audioplayerbridge.h"
#include <QTimer>

class UpnpController : public QObject, public PLT_MediaController, public PLT_MediaControllerDelegate
{
    Q_OBJECT
public:
    explicit UpnpController(QObject *parent, PLT_CtrlPointReference ctrlPoint, Player *player, AudioPlayerBridge *apb);
    virtual ~UpnpController();

    bool OnMRAdded(PLT_DeviceDataReference &device);
    void OnMRRemoved(PLT_DeviceDataReference &device);
    void OnMRStateVariablesChanged(PLT_Service *service, NPT_List<PLT_StateVariable *> *vars);

    QMap<QString, QString> getDevices();
    QString getIconUrl(QString uuid);
    void setRenderer(QString uuid);
    QString getRenderer();
    void OnGetPositionInfoResult(
        NPT_Result               res,
        PLT_DeviceDataReference& device,
        PLT_PositionInfo*        info,
        void*                    userdata);

signals:
    void rendererAdded(QString uuid, QString name);
    void rendererRemoved(QString uuid);

private slots:
    void playFromAPB(StreamInformation *si, Song *song);
    void stopFromAPB();
    void pauseFromAPB();
    void resumeFromAPB();
    int volumeForAPB();
    void setVolumeFromAPB(int volume);
    int positionForAPB();
    void setPositionFromAPB(int position);
    void requestData();

private:
    int lastPosition;
    int songLength;
    QMutex renderersMutex;
    QMap<QString, PLT_DeviceDataReference> rendererMap;
    PLT_DeviceDataReference currentRenderer;
    Player *player;
    AudioPlayerBridge *apb;
    QString generateDidl(Song *song, StreamInformation *si);
    QTimer *timer;
    void setXMLVal(QDomDocument doc, QDomElement element, QString string);
    bool stoppingInOrderToPlay;
};

#endif // UPNPCONTROLLER_H
