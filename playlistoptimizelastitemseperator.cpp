#include "playlistoptimizelastitemseperator.h"
#include "ui_playlistoptimizelastitemseperator.h"

PlaylistOptimizeLastItemSeperator::PlaylistOptimizeLastItemSeperator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaylistOptimizeLastItemSeperator)
{
    ui->setupUi(this);
    this->setFixedHeight(2);
    this->setAttribute(Qt::WA_PaintOutsidePaintEvent);
}

PlaylistOptimizeLastItemSeperator::~PlaylistOptimizeLastItemSeperator()
{
    delete ui;
}
