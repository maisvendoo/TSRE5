TEMPLATE = app

DESTDIR = ../bin
TARGET = TSRE5

VERSION = 1.0.0

CONFIG -= debug_and_release app_bundle lib_bundle

QT += core gui widgets network opengl

win32 {

    OPENAL_LIB_DIR = $$(OPENAL_BIN)
    OPENAL_INCLUDE_DIR = $$(OPENAL_INCLUDE)

    LIBS += -L$$OPENAL_LIB_DIR -lOpenAL32
    INCLUDEPATH += $$OPENAL_INCLUDE_DIR

    LIBS += -lopengl32 -lglu32
}

unix {

    LIBS += -lopenal
    LIBS += -lGL
}

INCLUDEPATH += ./include

SOURCES += $$files(./src/*.cpp)
HEADERS += $$files(./include/*.h)


