TEMPLATE = app

CONFIG += debug
CONFIG +=release
CONFIG	+= qt warn_on

INCLUDEPATH += src
LANGUAGE	= C++

QT += svg
#soon :)
#QT += network

TARGET = tarta

SOURCES += \
src/tarta.cpp \
src/main.cpp \
src/boarditem.cpp \
src/boardmodel.cpp \
src/pieceitem.cpp \
src/singleplayerview.cpp
           
HEADERS += \
src/tarta.h \
src/boarditem.h \
src/boardmodel.h \
src/pieceitem.h \
src/singleplayerview.h

RESOURCES  = data/tarta.qrc

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
