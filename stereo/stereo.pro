
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = obj
}

macx: DEFINES += MACINTOSH
unix:!macx: DEFINES += LINUX

PLATFORM = $$system(uname -s)

MISRDIR = /home/mstereo/a/sbansal6/misr-stereo-2013.11.24
HDF4INC = /usr/include/hdf
HDF4LIB = /usr/lib64/hdf
SZIPDIR = /usr/local/szip2.1

DESTDIR = ../bin
TARGET = misr-stereo

DEPENDPATH += $$MISRDIR/src
INCLUDEPATH += $$MISRDIR/src

SOURCES += glutaux.cpp
SOURCES += hdfDataNode.cpp
SOURCES += hdfDataSource.cpp
SOURCES += hdfField.cpp
SOURCES += hdfFile.cpp
SOURCES += hdfGrid.cpp
SOURCES += help.cpp
SOURCES += main.cpp
SOURCES += stereoviewer.cpp
SOURCES += stringaux.cpp
SOURCES += viewport.cpp
SOURCES += misr_orbits.cpp
SOURCES += misr_png_helper.cpp

TEMPLATE     = app
CONFIG -= qt
CONFIG += warn_on stl opengl thread release

LIBS        += -L../lib

unix:!macx: LIBS += -lX11 -lXi -lXmu -lpng

macx: LIBS += -framework GLUT -lpng
else: LIBS += -lglut -lGLU -lpng

INCLUDEPATH += ../hdfeos/include
LIBS        += -lhdfeos

INCLUDEPATH += $$MISRDIR/gctp
LIBS        += -lgctp

INCLUDEPATH += $$HDF4INC
LIBS        += -L$$HDF4LIB -lmfhdf -ldf

INCLUDEPATH += $$SZIPDIR/include
LIBS        += -L$$SZIPDIR/lib -lsz

exists($$MISRDIR/jpeg.$$PLATFORM) {
	INCLUDEPATH += $$MISRDIR/jpeg.$$PLATFORM/include
	LIBS        += -L$$MISRDIR/jpeg.$$PLATFORM/lib
}

LIBS        +=  -ljpeg -lz

LANGUAGE     = C++
