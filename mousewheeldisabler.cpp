#include "mousewheeldisabler.h"
#include <QMouseEvent>

MouseWheelDisabler::MouseWheelDisabler(QObject *parent) :
    QObject(parent)
{
}

bool MouseWheelDisabler::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::Wheel) {
        event->ignore();
        return true;
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* mouseEvent = (QMouseEvent*) event;
        if (mouseEvent->button() == Qt::MidButton) {
            event->accept();
            emit playlistAddEvent();
            return true;
        }
    }
    return false;
}
