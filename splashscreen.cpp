#include "splashscreen.h"
#include "ui_splashscreen.h"
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

SplashScreen::SplashScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SplashScreen)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_PaintOutsidePaintEvent);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint); //Set window to fixed size
    this->setWindowFlags(Qt::CustomizeWindowHint); //Set window with no title bar
    this->setWindowFlags(Qt::FramelessWindowHint); //Set a frameless window
    QGraphicsDropShadowEffect *effect1 = new QGraphicsDropShadowEffect(ui->lblGSP);
    effect1->setBlurRadius(5);
    effect1->setOffset(0, 1);
    effect1->setColor(QColor("#BEBEBE"));
    ui->lblGSP->setGraphicsEffect(effect1);
    QGraphicsOpacityEffect *effect2 = new QGraphicsOpacityEffect(this);
    effect2->setOpacity(0);
    this->setGraphicsEffect(effect2);
    QPropertyAnimation *propAn = new QPropertyAnimation(effect2, "opacity", this);
    propAn->setDuration(500);
    propAn->setStartValue(0.0);
    propAn->setEndValue(0.50);
    propAn->start();
}

SplashScreen::~SplashScreen()
{
    delete ui;
}
