#ifndef UPNPSERVER_H
#define UPNPSERVER_H

#include <QObject>
#include <Platinum.h>

class UpnpServer : public QObject, public PLT_MediaServer
{
    Q_OBJECT
public:
    explicit UpnpServer(QObject *parent,
                        PLT_CtrlPointReference &ctrlPoint,
                        QString name = "Kamele",
                        bool showIp = false,
                        unsigned int port = 0);
    virtual ~UpnpServer();

protected:
    NPT_Result OnBrowse(NPT_Reference<PLT_Action> &action, const PLT_HttpRequestContext &context);

signals:

public slots:

};

#endif // UPNPSERVER_H
