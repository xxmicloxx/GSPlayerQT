#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <API/api.h>
#include <audioplayerbridge.h>
#include "playlisthandler.h"
#include "coverhelper.h"
#include "player.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onChildClosed();

private slots:
    void on_btnSide2_clicked();

    void on_btn2_clicked();

    void on_btn3_clicked();

    void on_btn1_clicked();
    void onBtnPlay_pressed();

private:
    CoverHelper* coverHelper;
    API *api;
    PlaylistHandler *plh;
    Player *player;
    AudioPlayerBridge *bridge;
    Ui::MainWindow *ui;
    //QxtGlobalShortcut* playButtonShortcut;
};

#endif // MAINWINDOW_H
