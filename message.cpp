#include "message.h"
#include "ui_message.h"
#include <QTimer>

Message::Message(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Message)
{
    ui->setupUi(this);
    opacity = new QGraphicsOpacityEffect(this);
    opacity->setOpacity(0.0);
    this->setGraphicsEffect(opacity);
    blendIn = new QPropertyAnimation(opacity, "opacity");
    blendOut = new QPropertyAnimation(opacity, "opacity");
    moveUp = new QPropertyAnimation(this, "geometry");
    moveUp->setDuration(1000);
    blendIn->setDuration(500);
    blendOut->setDuration(500);
    blendIn->setStartValue(0.0);
    blendOut->setStartValue(0.75);
    blendIn->setEndValue(0.75);
    blendOut->setEndValue(0.0);
    this->setAttribute(Qt::WA_TransparentForMouseEvents);
    connect(blendIn, SIGNAL(finished()), this, SLOT(blendedIn()));
}

Message::~Message()
{
    delete ui;
    delete blendIn;
    delete blendOut;
    delete moveUp;
}

void Message::blendedIn() {
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), blendOut, SLOT(start()));
    timer->setSingleShot(true);
    timer->start(1500);
}

void Message::setText(std::string text) {
    ui->lblMessage->setText(QString::fromStdString(text));
}
