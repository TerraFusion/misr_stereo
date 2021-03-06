unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = obj
}

PLATFORM = $$system(uname -s)
#HDF4DIR = /opt/clout/hdf-4-4.2.5-gnu-4.1.2
HDF4INC = /usr/local/include/
SZIPDIR = /usr/local/

DESTDIR = ../lib
TARGET = hdfeos

INCLUDEPATH += include
DEPENDPATH += include
DEPENDPATH += src

macx: DEFINES += MACINTOSH
unix:!macx: DEFINES += LINUX

SOURCES += src/EHapi.c
SOURCES += src/GDapi.c
SOURCES += src/PTapi.c
SOURCES += src/SWapi.c

HEADERS += include/cfortHdf.h
HEADERS += include/ease.h
HEADERS += include/HdfEosDef.h
HEADERS += include/HDFEOSVersion.h

INCLUDEPATH += ../gctp
#INCLUDEPATH += $$HDF4DIR/include
INCLUDEPATH += $$HDF4INC
INCLUDEPATH += $$SZIPDIR/include

TEMPLATE     = lib
CONFIG      += release staticlib warn_off

LANG = C++

DBFILE	     = hdfeos.db
