TEMPLATE = app

DESTDIR = ../bin
TARGET = TSRE5

VERSION = 1.0.0

CONFIG -= debug_and_release app_bundle lib_bundle

QT += core gui widgets network opengl
QTPLUGIN += bearer

win32 {


}

unix {

    LIBS += -lopenal
    LIBS += -lGL
}

INCLUDEPATH += ../include

SOURCES += $$files(../src/*.cpp)
HEADERS += $$files(../include/*.h)


