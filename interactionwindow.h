#ifndef INTERACTIONWINDOW_H
#define INTERACTIONWINDOW_H

#include <QMainWindow>
#include "API/api.h"

namespace Ui {
class InteractionWindow;
}

class InteractionWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit InteractionWindow(QWidget *parent = 0);
    ~InteractionWindow();
    void init();
    void checkConnect();
    bool connected;
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::InteractionWindow *ui;
    API* api;
    QString sessionID;
    QString token;
};

#endif // INTERACTIONWINDOW_H
