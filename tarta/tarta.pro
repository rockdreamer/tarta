TEMPLATE = app

CONFIG += debug
CONFIG += qt warn_on

INCLUDEPATH += src
LANGUAGE = C++

QT += svg
#soon :)
#QT += network

TARGET = tarta

SOURCES += \
src/about.cpp \
src/tarta.cpp \
src/main.cpp \
src/boarditem.cpp \
src/boardmodel.cpp \
src/pieceitem.cpp \
src/singleplayerview.cpp \
src/leveldata.cpp \
src/loadingitem.cpp \
src/msgitem.cpp \
           
HEADERS += \
src/about.h \
src/tarta.h \
src/boarditem.h \
src/boardmodel.h \
src/pieceitem.h \
src/singleplayerview.h \
src/leveldata.h \
src/loadingitem.h \
src/msgitem.h \

RESOURCES = data/tarta.qrc

unix {
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}

mac {
    ICON = packaging/mac/tarta.icns
}

win32 {
    #	RC_FILE=packaging/win/tarta.rc
}
CONFIG -= release

OBJECTS_DIR = .obj

UI_DIR = .ui

MOC_DIR = .moc

