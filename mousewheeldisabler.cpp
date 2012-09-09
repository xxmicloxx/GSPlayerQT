#include "mousewheeldisabler.h"

MouseWheelDisabler::MouseWheelDisabler(QObject *parent) :
    QObject(parent)
{
}

bool MouseWheelDisabler::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::Wheel) {
        event->ignore();
        return true;
    }
    return false;
}
