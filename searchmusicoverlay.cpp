#include "searchmusicoverlay.h"
#include "ui_searchmusicoverlay.h"
#include <QPropertyAnimation>

SearchMusicOverlay::SearchMusicOverlay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchMusicOverlay)
{
    ui->setupUi(this);
    effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(0);
    this->setGraphicsEffect(effect);
    blendInAnimation = new QPropertyAnimation(effect, "opacity");
    blendOutAnimation = new QPropertyAnimation(effect, "opacity");
    blendInAnimation->setDuration(250);
    blendOutAnimation->setDuration(250);
    blendInAnimation->setStartValue(0.0);
    blendOutAnimation->setStartValue(0.5);
    blendInAnimation->setEndValue(0.5);
    blendOutAnimation->setEndValue(0.0);
}

SearchMusicOverlay::~SearchMusicOverlay()
{
    delete ui;
}
