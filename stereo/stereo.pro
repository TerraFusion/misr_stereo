
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = obj
}

macx: DEFINES += MACINTOSH
unix:!macx: DEFINES += LINUX

PLATFORM = $$system(uname -s)

MISRDIR = /home/landon/misr_stereo
HDF4INC = /usr/local/include/
HDF4LIB = /usr/local/lib/
SZIPDIR = /usr/local/

DESTDIR = ../bin
TARGET = misr-stereo

DEPENDPATH += $$MISRDIR/src
INCLUDEPATH += $$MISRDIR/src

SOURCES += glutaux.cpp
SOURCES += ../src/hdfDataNode.cpp
SOURCES += hdfDataSource.cpp
SOURCES += hdfField.cpp
SOURCES += hdfFile.cpp
SOURCES += hdfGrid.cpp
SOURCES += help.cpp
SOURCES += main.cpp
SOURCES += stereoviewer.cpp
SOURCES += ../src/stringaux.cpp
SOURCES += viewport.cpp
SOURCES += misr_orbits.cpp
SOURCES += misr_png_helper.cpp

TEMPLATE     = app
CONFIG -= qt
CONFIG += warn_on stl opengl thread release

LIBS        += -L../lib

unix:!macx: LIBS += -lX11 -lXi -lXmu

macx: LIBS += -framework GLUT
else: LIBS += -lglut

INCLUDEPATH += ../hdfeos/include
LIBS        += -lhdfeos

INCLUDEPATH += $$MISRDIR/gctp
LIBS        += -lgctp

INCLUDEPATH += $$HDF4INC
LIBS        += -L$$HDF4LIB -lmfhdf -ldf

INCLUDEPATH += $$SZIPDIR/include
LIBS        += -L$$SZIPDIR/lib -lsz

LIBS += -lpng
LIBS += -lGLU

exists($$MISRDIR/jpeg.$$PLATFORM) {
	INCLUDEPATH += $$MISRDIR/jpeg.$$PLATFORM/include
	LIBS        += -L$$MISRDIR/jpeg.$$PLATFORM/lib
}

LIBS        +=  -ljpeg -lz

LANGUAGE     = C++
