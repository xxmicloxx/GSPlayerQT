#ifndef UPNPDIALOG_H
#define UPNPDIALOG_H

#include <QDialog>
#include <Platinum.h>
#include "Upnp/upnpcontroller.h"
#include "audioplayerbridge.h"
#include "player.h"

namespace Ui {
class UpnpDialog;
}

class UpnpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpnpDialog(QWidget *parent = 0, UpnpController *controller = 0, AudioPlayerBridge *apb = 0, Player *player = 0);
    ~UpnpDialog();

private slots:
    void on_pushButton_clicked();
    void onRendererAdded(QString uuid, QString name);
    void onRendererRemoved(QString uuid);
    void onSelectedDeviceChanged(int index);

private:
    Ui::UpnpDialog *ui;
    UpnpController *controller;
    AudioPlayerBridge *apb;
    Player *player;
    bool comboInited = false;
};

#endif // UPNPDIALOG_H
