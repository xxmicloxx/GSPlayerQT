#include "upnpcontroller.h"
#include <QDebug>
#include <QtXml>

UpnpController::UpnpController(QObject *parent, PLT_CtrlPointReference ctrlPoint, Player *player, AudioPlayerBridge *apb) :
    QObject(parent), PLT_MediaController(ctrlPoint)
{
    this->player = player;
    this->apb = apb;
    timer = new QTimer(this);
    stoppingInOrderToPlay = false;
    lastPosition = 0;
    songLength = 10000;
    connect(apb, SIGNAL(playOnUpnp(StreamInformation*,Song*)), this, SLOT(playFromAPB(StreamInformation*,Song*)));
    connect(apb, SIGNAL(stopOnUpnp()), this, SLOT(stopFromAPB()));
    connect(apb, SIGNAL(pauseOnUpnp()), this, SLOT(pauseFromAPB()));
    connect(apb, SIGNAL(resumeOnUpnp()), this, SLOT(resumeFromAPB()));
    connect(apb, SIGNAL(volumeUpnpRequest()), this, SLOT(volumeForAPB()));
    connect(apb, SIGNAL(setVolumeOnUpnp(int)), this, SLOT(setVolumeFromAPB(int)));
    connect(apb, SIGNAL(positionUpnpRequest()), this, SLOT(positionForAPB()));
    connect(apb, SIGNAL(setPositionOnUpnp(int)), this, SLOT(setPositionFromAPB(int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(requestData()));
    timer->start(1000);
    PLT_MediaController::SetDelegate(this);
    qDebug() << "Set delegate";
}

UpnpController::~UpnpController() {
}

bool UpnpController::OnMRAdded(PLT_DeviceDataReference &device) {
    NPT_String uuid = device->GetUUID();

    PLT_Service *service;
    if (NPT_SUCCEEDED(device->FindServiceByType("urn:schemas-upnp-org:service:AVTransport:*", service))) {
        qDebug() << "Found renderer: " << device->GetFriendlyName().GetChars();
        renderersMutex.lock();
        rendererMap.insert(QString::fromStdString(uuid.GetChars()), device);
        renderersMutex.unlock();
        emit rendererAdded(QString::fromStdString(uuid.GetChars()), QString::fromStdString(device->GetFriendlyName().GetChars()));
    }

    return true;
}

void UpnpController::OnMRRemoved(PLT_DeviceDataReference &device) {
    NPT_String uuid = device->GetUUID();

    qDebug() << "Removing renderer with name " << device->GetFriendlyName().GetChars();
    if (currentRenderer->GetUUID() == uuid) {
        currentRenderer = NULL;
        player->stopNoReset();
        apb->setUseUpnp(false);
    }
    renderersMutex.lock();
    rendererMap.remove(QString::fromStdString(uuid.GetChars()));
    renderersMutex.unlock();

    emit rendererRemoved(QString::fromStdString(uuid.GetChars()));
}

void UpnpController::OnMRStateVariablesChanged(PLT_Service *service, NPT_List<PLT_StateVariable *> *vars) {
    NPT_String uuid = service->GetDevice()->GetUUID();
    if (currentRenderer.IsNull()) {
        return;
    }
    if (currentRenderer->GetUUID() != uuid) {
        return;
    }
    NPT_List<PLT_StateVariable*>::Iterator var = vars->GetFirstItem();
    while (var) {
        qDebug() << "Received state changes: " << (*var)->GetName().GetChars() << " : " << (*var)->GetValue().GetChars();
        if ((*var)->GetName() == "TransportState") {
            qDebug() << "APB state: " << apb->getState();
            qDebug() << "Stopping in order to play: " << stoppingInOrderToPlay;
            QString state = QString::fromStdString((*var)->GetValue().GetChars());
            // TODO FIX THIS SHIT
            if (state == "STOPPED" && !(apb->getState() == AudioPlayerBridge::STARTING && stoppingInOrderToPlay) && !(apb->getState() == AudioPlayerBridge::PLAYING && (songLength - lastPosition) < 3000)) {
                player->stopNoReset();
                apb->setState(AudioPlayerBridge::STOPPED);
            } else if (state == "STOPPED" && stoppingInOrderToPlay) {
                songLength = nextStreamInformation->getUSecs()/1000;
                QString didl = generateDidl(nextSong, nextStreamInformation);
                SetAVTransportURI(currentRenderer, 0, nextStreamInformation->directUrl().c_str(), didl.toStdString().c_str(), NULL);
                qDebug() << "Setting transport method...";
            } else if (state == "STOPPED" && apb->getState() == AudioPlayerBridge::PLAYING && (songLength - lastPosition) < 3000) {
                // this is the end of a song
                qDebug() << "This is the next song VIA UPNP";
                apb->setState(AudioPlayerBridge::STOPPED);
                apb->emit_songFinished();
            } else if (state == "TRANSITIONING" && apb->getState() != AudioPlayerBridge::STARTING) {
                apb->setState(AudioPlayerBridge::BUFFERING);
            } else if (state == "PLAYING") {
                if (apb->getState() == AudioPlayerBridge::STARTING) {
                    apb->setState(AudioPlayerBridge::PLAYING);
                    apb->emitStartedPlaying();
                } else {
                    player->play();
                    apb->setState(AudioPlayerBridge::PLAYING);
                }
            } else if (state == "PAUSED_PLAYBACK") {
                player->pause();
                apb->setState(AudioPlayerBridge::PAUSED);
            }
        }
        if ((*var)->GetName() == "AVTransportURIMetaData") {
            if (apb->getState() == AudioPlayerBridge::STARTING) {
                // play now
                Play(currentRenderer, 0, "1", NULL);
                qDebug() << "STARTING TO PLAY...";
            }
        }
        ++var;
    }
}

void UpnpController::stopFromAPB() {
    Stop(currentRenderer, 0, NULL);
}

void UpnpController::pauseFromAPB() {
    Pause(currentRenderer, 0, NULL);
}

void UpnpController::resumeFromAPB() {
    Play(currentRenderer, 0, "1", NULL);
}

int UpnpController::volumeForAPB() {
    unsigned int volume;
    GetVolumeState(currentRenderer->GetUUID(), volume);
    return volume;
}

void UpnpController::setVolumeFromAPB(int volume) {
    SetVolume(currentRenderer, 0, "Master", volume, NULL);
}

int UpnpController::positionForAPB() {
    return lastPosition;
}

void UpnpController::setPositionFromAPB(int position) {
    QString realTime = QString::number(position/1000/3600) + ":" + QString::number((position/1000/60)%60) + ":" + QString::number((position/1000) % 60);
    Seek(currentRenderer, 0, "REL_TIME", NPT_String(realTime.toStdString().c_str()), NULL);
}

QString UpnpController::generateDidl(Song *song, StreamInformation *si) {
    QDomDocument doc;

    QDomProcessingInstruction header = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(header);

    QDomElement rootElement = doc.createElement("DIDL-Lite");
    rootElement.setAttribute("xmlns", "urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/");
    rootElement.setAttribute("xmlns:dc", "http://purl.org/dc/elements/1.1/");
    rootElement.setAttribute("xmlns:dlna", "urn:schemas-dlna-org:metadata-1-0/");
    rootElement.setAttribute("xmlns:upnp", "urn:schemas-upnp-org:metadata-1-0/upnp/");
    doc.appendChild(rootElement);

    QDomElement itemElement = doc.createElement("item");
    itemElement.setAttribute("id", "/" + QString::number(song->getSongId()));
    itemElement.setAttribute("parentID", "/");
    itemElement.setAttribute("restricted", 0);
    rootElement.appendChild(itemElement);

    QDomElement dcTitle = doc.createElement("dc:title");
    setXMLVal(doc, dcTitle, QString::fromStdString(song->getSongName()));
    itemElement.appendChild(dcTitle);

    QString artistName = QString::fromStdString(song->getArtistName());

    QDomElement dcCreator = doc.createElement("dc:creator");
    setXMLVal(doc, dcCreator, artistName);
    itemElement.appendChild(dcCreator);

    QDomElement upnpArtist = doc.createElement("upnp:artist");
    setXMLVal(doc, upnpArtist, artistName);
    itemElement.appendChild(upnpArtist);

    QDomElement upnpActor = doc.createElement("upnp:actor");
    setXMLVal(doc, upnpActor, artistName);
    itemElement.appendChild(upnpActor);

    QDomElement upnpAuthor = doc.createElement("upnp:author");
    setXMLVal(doc, upnpAuthor, artistName);
    itemElement.appendChild(upnpAuthor);

    QDomElement upnpAlbum = doc.createElement("upnp:album");
    setXMLVal(doc, upnpAlbum, QString::fromStdString(song->getAlbumName()));
    itemElement.appendChild(upnpAlbum);

    QDomElement upnpGenre = doc.createElement("upnp:genre");
    setXMLVal(doc, upnpGenre, "Unknown");
    itemElement.appendChild(upnpGenre);

    QDomElement res = doc.createElement("res");
    QString minutes = QString::number((si->getUSecs()/1000/1000/60)%60);
    if (minutes.length() < 2) {
        minutes = "0" + minutes;
    }
    QString seconds = QString::number((si->getUSecs()/1000/1000)%60);
    if (seconds.length() < 2) {
        seconds = "0" + seconds;
    }
    QString msecs = QString::number((si->getUSecs()/1000)%1000);
    while (msecs.length() < 3) {
        msecs = "0" + msecs;
    }
    QString durationString = QString::number(si->getUSecs()/1000/1000/60/60) + ":" + minutes + ":"
            + seconds + "." + msecs;
    res.setAttribute("duration", durationString);
    res.setAttribute("protocolInfo", "htp-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_FLAGS=01700000000000000000000000000000");
    setXMLVal(doc, res, QString::fromStdString(si->directUrl()));
    itemElement.appendChild(res);

    QDomElement upnpClass = doc.createElement("upnp:class");
    setXMLVal(doc, upnpClass, "object.item.audioItem.musicTrack");
    itemElement.appendChild(upnpClass);

    return doc.toString();
}

void UpnpController::setXMLVal(QDomDocument doc, QDomElement element, QString string) {
    QDomText t = doc.createTextNode(string);
    element.appendChild(t);
}

void UpnpController::playFromAPB(StreamInformation *si, Song *song) {
    NPT_String state;
    GetTransportState(currentRenderer->GetUUID(), state);
    if (state != "STOPPED") {
        // stop
        stoppingInOrderToPlay = true;
        nextSong = song;
        nextStreamInformation = si;
        Stop(currentRenderer, 0, NULL);
    } else {
        songLength = si->getUSecs()/1000;
        QString didl = generateDidl(song, si);
        SetAVTransportURI(currentRenderer, 0, si->directUrl().c_str(), didl.toStdString().c_str(), NULL);
        qDebug() << "Setting transport method...";
    }
}

void UpnpController::OnGetPositionInfoResult(NPT_Result res, PLT_DeviceDataReference &device, PLT_PositionInfo *info, void *userdata) {
    lastPosition = info->rel_time.ToMillis();
}

void UpnpController::setRenderer(QString uuid) {
    if (!currentRenderer.IsNull()) {
        Stop(currentRenderer, 0, 0);
    }
    currentRenderer = NULL;
    if (uuid != "") {
        renderersMutex.lock();
        PLT_DeviceDataReference device = rendererMap.value(uuid);
        renderersMutex.unlock();
        currentRenderer = device;
    }
    //TODO open etc...
    requestData();
    apb->setState(AudioPlayerBridge::STOPPED);
}

void UpnpController::requestData() {
    if (!currentRenderer.IsNull()) {
        GetPositionInfo(currentRenderer, 0, NULL);
    }
}

QString UpnpController::getRenderer() {
    if (currentRenderer.IsNull()) {
        return "";
    }
    return QString::fromStdString(currentRenderer->GetUUID().GetChars());
}

QString UpnpController::getIconUrl(QString uuid) {
    renderersMutex.lock();
    PLT_DeviceDataReference device = rendererMap.value(uuid);
    renderersMutex.unlock();
    NPT_String iconUrl = device->GetIconUrl();
    if (iconUrl == "") {
        return "";
    }
    return QString::fromStdString(iconUrl.GetChars());
}

QMap<QString, QString> UpnpController::getDevices() {
    QMap<QString, QString> resultMap;
    renderersMutex.lock();
    QMapIterator<QString, PLT_DeviceDataReference> deviceIterator(rendererMap);
    while (deviceIterator.hasNext()) {
        deviceIterator.next();
        QString uuid = deviceIterator.key();
        PLT_DeviceDataReference device = deviceIterator.value();
        resultMap.insert(uuid, QString::fromStdString(device->GetFriendlyName().GetChars()));
    }
    renderersMutex.unlock();
    return resultMap;
}
