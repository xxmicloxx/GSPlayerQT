#include "upnpdialog.h"
#include "ui_upnpdialog.h"
#include "Upnp/upnpcontroller.h"
#include <Platinum.h>

UpnpDialog::UpnpDialog(QWidget *parent, UpnpController *controller, AudioPlayerBridge *apb, Player *player) :
    QDialog(parent),
    ui(new Ui::UpnpDialog)
{
    this->controller = controller;
    this->apb = apb;
    this->player = player;
    ui->setupUi(this);
    connect(controller, SIGNAL(rendererAdded(QString,QString)), this, SLOT(onRendererAdded(QString,QString)));
    connect(controller, SIGNAL(rendererRemoved(QString)), this, SLOT(onRendererRemoved(QString)));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectedDeviceChanged(int)));
    ui->comboBox->addItem("Local playback (disable UPnP)", "LOCAL");
    QMap<QString, QString> deviceMap = controller->getDevices();
    QMapIterator<QString, QString> i(deviceMap);
    while (i.hasNext()) {
        i.next();
        ui->comboBox->addItem(i.value(), i.key());
    }
    if (apb->getUseUpnp()) {
        QString currentRenderer = controller->getRenderer();
        if (currentRenderer == "") {
            // THIS SHALL NOT HAPPEN
            apb->setUseUpnp(false);
        } else {
            comboInited = false;
            int findIndex = ui->comboBox->findData(currentRenderer);
            ui->comboBox->setCurrentIndex(findIndex);
        }
    }
}

UpnpDialog::~UpnpDialog()
{
    delete ui;
}

void UpnpDialog::onSelectedDeviceChanged(int index) {
    if (!comboInited) {
        comboInited = true;
        return;
    }
    QString uuid = ui->comboBox->itemData(index).toString();
    player->stopNoReset();
    if (uuid == "LOCAL") {
        // disable upnp
        apb->setUseUpnp(false);
        controller->setRenderer("");
    } else {
        apb->setUseUpnp(true);
        controller->setRenderer(uuid);
    }
}

void UpnpDialog::on_pushButton_clicked()
{

}

void UpnpDialog::onRendererAdded(QString uuid, QString name) {
    ui->comboBox->addItem(name, uuid);
}

void UpnpDialog::onRendererRemoved(QString uuid) {
    int id = ui->comboBox->findData(uuid);
    if (id == ui->comboBox->currentIndex()) {
        // set to local renderer
        ui->comboBox->setCurrentIndex(0);
    }
    ui->comboBox->removeItem(id);
}
