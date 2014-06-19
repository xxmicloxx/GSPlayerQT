#-------------------------------------------------
#
# Project created by QtCreator 2012-06-09T09:42:04
#
#-------------------------------------------------

QT       += core gui network xml widgets

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
    myvolumestyle.cpp \
    titleplaydialog.cpp \
    splashscreen.cpp \
    upnpdialog.cpp \
    Upnp/upnpcontroller.cpp \
    Upnp/upnpserver.cpp

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
    myvolumestyle.h \
    titleplaydialog.h \
    splashscreen.h \
    upnpdialog.h \
    Upnp/upnpcontroller.h \
    Upnp/upnpserver.h

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
    songinfodialog.ui \
    titleplaydialog.ui \
    splashscreen.ui \
    upnpdialog.ui

LIBS += C:\Users\ML\Documents\GitHub\GSPlayerQT\bass.lib

LIBS += C:\Users\ML\Documents\GitHub\GSPlayerQT\Platinum.lib
LIBS += C:\Users\ML\Documents\GitHub\GSPlayerQT\Neptune.lib

DEFINES += UNICODE

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000

HEADERS += bass.h

RESOURCES += \
    Design.qrc

RC_FILE = myWinIcon.rc

INCLUDEPATH += C:\Users\ML\boost_1_55_0
INCLUDEPATH += C:\Users\ML\Downloads\PlatinumKit-1-0-5-13_0ab854\PlatinumKit-1-0-5-13_0ab854\Platinum\Source\Platinum
INCLUDEPATH += C:\Users\ML\Downloads\PlatinumKit-1-0-5-13_0ab854\PlatinumKit-1-0-5-13_0ab854\Platinum\Source\Core
INCLUDEPATH += C:\Users\ML\Downloads\PlatinumKit-1-0-5-13_0ab854\PlatinumKit-1-0-5-13_0ab854\Platinum\Source\Devices\MediaConnect
INCLUDEPATH += C:\Users\ML\Downloads\PlatinumKit-1-0-5-13_0ab854\PlatinumKit-1-0-5-13_0ab854\Platinum\Source\Devices\MediaRenderer
INCLUDEPATH += C:\Users\ML\Downloads\PlatinumKit-1-0-5-13_0ab854\PlatinumKit-1-0-5-13_0ab854\Platinum\Source\Devices\MediaServer
INCLUDEPATH += C:\Users\ML\Downloads\PlatinumKit-1-0-5-13_0ab854\PlatinumKit-1-0-5-13_0ab854\Platinum\Source\Extras
INCLUDEPATH += C:\Users\ML\Downloads\PlatinumKit-1-0-5-13_0ab854\PlatinumKit-1-0-5-13_0ab854\Neptune\Source\Core

QMAKE_CXXFLAGS += /DNOMINMAX
