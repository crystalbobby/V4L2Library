#-------------------------------------------------
#
# Project created by QtCreator 2017-09-09T20:50:10
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LXCap
TEMPLATE = app

INCLUDEPATH += ../../Library ui

SOURCES += main.cpp\
        ui/lxcap.cpp \
    ../../Library/libv4l2.cpp \
    ui/filterwindow.cpp \
    ui/pinwindow.cpp \
    ui/devctrl.cpp

HEADERS  += ui/lxcap.h \
    ../../Library/libv4l2.h \
    ui/pinwindow.h \
    ui/filterwindow.h

FORMS    += ui/lxcap.ui \
    ui/pinwindow.ui \
    ui/filterwindow.ui

RESOURCES += \
    sound/sound.qrc \
    img/img.qrc
