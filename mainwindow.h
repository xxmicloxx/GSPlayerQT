#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <API/api.h>
#include <audioplayerbridge.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnSide2_clicked();

private:
    API *api;
    AudioPlayerBridge *bridge;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
