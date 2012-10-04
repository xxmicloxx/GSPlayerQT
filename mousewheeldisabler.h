#ifndef MOUSEWHEELDISABLER_H
#define MOUSEWHEELDISABLER_H

#include <QObject>
#include <QEvent>

class MouseWheelDisabler : public QObject
{
    Q_OBJECT
public:
    explicit MouseWheelDisabler(QObject *parent = 0);
    
signals:
    void playlistAddEvent();

public slots:

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    
};

#endif // MOUSEWHEELDISABLER_H
