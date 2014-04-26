#-------------------------------------------------
#
# Project created by QtCreator 2014-04-22T14:27:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TTF2CBitmap
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

unix {
    INCLUDEPATH += /usr/include
    INCLUDEPATH += /usr/local/include
    INCLUDEPATH += /usr/include/GraphicsMagick/
    INCLUDEPATH += /usr/local/include/GraphicsMagick/
    LIBS += -L/usr/lib
    LIBS += -L/usr/local/lib
    LIBS += -lGraphicsMagick++
}

win32 {
    INCLUDEPATH += $$PWD/_deps-win32/mingw/include
    INCLUDEPATH += $$PWD/_deps-win32/mingw/include/GraphicsMagick/
    debug:LIBS += -L$$PWD/_deps-win32/mingw/lib/debug
    release:LIBS += -L$$PWD/_deps-win32/mingw/lib/release
    LIBS += -lGraphicsMagick++-3
}

LIBS += -lboost_filesystem
LIBS += -lboost_system

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.hpp

FORMS    += mainwindow.ui


