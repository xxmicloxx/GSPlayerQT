#include "splashscreen.h"
#include "ui_splashscreen.h"
#include <QGraphicsDropShadowEffect>

SplashScreen::SplashScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SplashScreen)
{
    ui->setupUi(this);
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
}

SplashScreen::~SplashScreen()
{
    delete ui;
}
