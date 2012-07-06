#include <QtGui/QApplication>
#include "interactionwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InteractionWindow w;
    w.show();
    
    return a.exec();
}
