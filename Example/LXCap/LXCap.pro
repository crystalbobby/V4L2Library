QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LXCap
TEMPLATE = app
DESTDIR += bin
UI_DIR += ui
RCC_DIR += qrc
MOC_DIR += moc
OBJECTS_DIR += obj
CONFIG+=qt warn_off release

INCLUDEPATH += ../../Library ui

SOURCES += src/main.cpp\
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
