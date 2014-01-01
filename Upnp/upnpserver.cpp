#include "upnpserver.h"

UpnpServer::UpnpServer(QObject *parent, PLT_CtrlPointReference &ctrlPoint, QString name, bool showIp, unsigned int port) :
    QObject(parent), PLT_MediaServer(name.toStdString().c_str(), showIp, NULL, port)
{
}

UpnpServer::~UpnpServer() {
}

NPT_Result UpnpServer::OnBrowse(NPT_Reference<PLT_Action> &action, const PLT_HttpRequestContext &context) {
    return NPT_SUCCESS;
}
