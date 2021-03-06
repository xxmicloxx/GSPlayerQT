#include <QApplication>
#include "mainwindow.h"
#include <QDebug>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <QTextStream>
#include <QTextCodec>
#include <QFile>

QTextStream stream(fopen("log", "w"));

void myMessageOutput(QtMsgType type, const char *msg)
{
     //in this function, you can write the message to any stream!
    stream << QString(msg);
    stream << QString("\n");
    stream.flush();
}

int main(int argc, char *argv[])
{
    //qInstallMsgHandler(myMessageOutput);
    QApplication a(argc, argv);
//    QFile playlists("Playlists.xml");
//    if (playlists.exists()) {
//        playlists.remove();
//    }
    //QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "UTF-8" ) );
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") );
    //QTextCodec::setCodecForTr( QTextCodec::codecForName("UTF-8") );
    MainWindow w;
    w.show();
    return a.exec();
}
