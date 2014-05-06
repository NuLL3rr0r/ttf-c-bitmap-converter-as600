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
    INCLUDEPATH += /usr/include/GraphicsMagick
    INCLUDEPATH += /usr/local/include/GraphicsMagick
    LIBS += -L/usr/lib
    LIBS += -L/usr/local/lib
    LIBS += -lGraphicsMagick++
}

win32 {
    debug {
        LIBS += -L$$PWD/_deps-win32/mingw/lib/debug
    }
    release {
        LIBS += -L$$PWD/_deps-win32/mingw/lib/release
    }

    QMAKE_CFLAGS += -isystem $$PWD/_deps-win32/mingw/include
    QMAKE_CXXFLAGS += -isystem $$PWD/_deps-win32/mingw/include

    DEFINES += MAGICKPP_IS_GM
    QMAKE_CFLAGS += -isystem $$PWD/_deps-win32/mingw/include/GraphicsMagick
    QMAKE_CXXFLAGS += -isystem $$PWD/_deps-win32/mingw/include/GraphicsMagick
    LIBS += -lGraphicsMagick++-3

    #DEFINES += MAGICKPP_IS_IM
    #DEFINES += MAGICKCORE_QUANTUM_DEPTH=8
    #DEFINES += MAGICKCORE_HDRI_ENABLE=FALSE
    #QMAKE_CFLAGS += -isystem $$PWD/_deps-win32/mingw/include/ImageMagick-6
    #QMAKE_CXXFLAGS += -isystem $$PWD/_deps-win32/mingw/include/ImageMagick-6
    #LIBS += -lMagick++-6.Q8-3 -lMagickCore-6.Q8-2 -lMagickWand-6.Q8-2
}

LIBS += -lboost_filesystem
LIBS += -lboost_system

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.hpp

FORMS    += mainwindow.ui


