#include "searchmusicoverlay.h"
#include "ui_searchmusicoverlay.h"
#include <QPropertyAnimation>
#include <boost/lexical_cast.hpp>

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
    blendOutAnimation->setStartValue(0.75);
    blendInAnimation->setEndValue(0.75);
    blendOutAnimation->setEndValue(0.0);
}

void SearchMusicOverlay::setItemMax(int count) {
    if (ui->progressBar == NULL)
        return;
    ui->progressBar->setMaximum(count);
    updateText(ui->progressBar->maximum() - ui->progressBar->value());
}

void SearchMusicOverlay::updateText(int remaining) {
    ui->lblLeft->setText(QString::fromStdString(boost::lexical_cast<std::string>(remaining) + " verbleibend"));
}

void SearchMusicOverlay::setItemsDone(int count) {
    if (ui->progressBar == NULL)
        return;
    ui->progressBar->setValue(count);
    updateText(ui->progressBar->maximum() - ui->progressBar->value());
}

SearchMusicOverlay::~SearchMusicOverlay()
{
    delete ui;
    delete blendInAnimation;
    delete blendOutAnimation;
}
