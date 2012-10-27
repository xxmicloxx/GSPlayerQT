#-------------------------------------------------
#
# Project created by QtCreator 2012-06-09T09:42:04
#
#-------------------------------------------------

CONFIG += qxt

QT       += core gui network xml

TARGET = GSPlayer
TEMPLATE = app


SOURCES += main.cpp\
    API/util.cpp \
    audioplayerbridge.cpp \
    API/api.cpp \
    API/streaminformation.cpp \
    mainwindow.cpp \
    searchmusicwindow.cpp \
    searchmusiclistitem.cpp \
    API/song.cpp \
    JsonBox/Value.cpp \
    JsonBox/SolidusEscaper.cpp \
    JsonBox/Object.cpp \
    JsonBox/Indenter.cpp \
    JsonBox/IndentCanceller.cpp \
    JsonBox/Escaper.cpp \
    JsonBox/Convert.cpp \
    JsonBox/Array.cpp \
    API/artist.cpp \
    artistlistitem.cpp \
    searchmusicoverlay.cpp \
    downloadsongwindow.cpp \
    playlisthandler.cpp \
    messagehandler.cpp \
    message.cpp \
    mousewheeldisabler.cpp \
    playlistoptimizewindow.cpp \
    playlistoptimizelistitem.cpp \
    searchmusiclastitemseperator.cpp \
    playlistoptimizelastitemseperator.cpp \
    playmusicwindow.cpp \
    songinfodialog.cpp \
    coverhelper.cpp \
    player.cpp \
    dragdroplist.cpp \
    myvolumestyle.cpp

HEADERS  += \
    API/util.h \
    audioplayerbridge.h \
    API/api.h \
    API/streaminformation.h \
    mainwindow.h \
    searchmusicwindow.h \
    searchmusiclistitem.h \
    API/song.h \
    JsonBox.h \
    JsonBox/Value.h \
    JsonBox/SolidusEscaper.h \
    JsonBox/OutputFilter.h \
    JsonBox/Object.h \
    JsonBox/Indenter.h \
    JsonBox/IndentCanceller.h \
    JsonBox/Grammar.h \
    JsonBox/Escaper.h \
    JsonBox/Convert.h \
    JsonBox/Array.h \
    API/artist.h \
    artistlistitem.h \
    searchmusicoverlay.h \
    downloadsongwindow.h \
    playlisthandler.h \
    messagehandler.h \
    message.h \
    mousewheeldisabler.h \
    playlistoptimizewindow.h \
    playlistoptimizelistitem.h \
    searchmusiclastitemseperator.h \
    playlistoptimizelastitemseperator.h \
    playmusicwindow.h \
    songinfodialog.h \
    coverhelper.h \
    player.h \
    dragdroplist.h \
    myvolumestyle.h

FORMS    += \
    mainwindow.ui \
    searchmusicwindow.ui \
    searchmusiclistitem.ui \
    artistlistitem.ui \
    searchmusicoverlay.ui \
    downloadsongwindow.ui \
    message.ui \
    playlistoptimizewindow.ui \
    playlistoptimizelistitem.ui \
    searchmusiclastitemseperator.ui \
    playlistoptimizelastitemseperator.ui \
    playmusicwindow.ui \
    songinfodialog.ui

LIBS += C:/Users/Mic/Desktop/QtDev/Pros/GSPlayerQT/GSPlayer/bass.lib

DEFINES += UNICODE

HEADERS += bass.h

RESOURCES += \
    Design.qrc

RC_FILE = myWinIcon.rc

INCLUDEPATH += C:/Users/Mic/Desktop/QtDev/Libs/boost_1_51_0
