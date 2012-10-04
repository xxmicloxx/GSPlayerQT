#include "dragdroplist.h"

DragDropList::DragDropList(QWidget *parent) :
    QListWidget(parent)
{
}

void DragDropList::dropEvent(QDropEvent *event) {
    QListWidget::dropEvent(event);
    emit replaceSeperator();
}
