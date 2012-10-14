#include "browserwindow.h"
#include "ui_browserwindow.h"

BrowserWindow::BrowserWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BrowserWindow)
{
    ui->setupUi(this);
}

BrowserWindow::~BrowserWindow()
{
    delete ui;
}

void BrowserWindow::on_pushButton_clicked()
{
    ui->webView->load(QUrl(ui->textEdit->toPlainText()));
}
