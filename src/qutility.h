#ifndef QUTILITY_H_INCLUDED
#define QUTILITY_H_INCLUDED

class QApplication;
class QPainter;
class QRectF;

#include <qdir.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qfileinfo.h>

//! Initializes the application path.
//! Call on application startup.
void initAppPath( const char * environment_variable );

//! Returns the path to the application, with subPath appended.
QString getAppPath( QString subPath = QString::null );

//! Converts a color from rgba to qt format
unsigned int getQRgba( unsigned int rgba );

// Converts a number to a string with commas at thousands.
QString CommaInt( double input );

//! Converts a number to a string, tries to display a reasonable number of
//! digits. Displays commas with numbers over 10000.
QString ToString( double input, int digits = 3 );

//! Returns a list of ( before + list item + after )
QStringList GlueStringList( const QStringList source, QString before = QString::null, QString after = QString::null );

//! Returns all files matching pattern, located in path or a subdirectory of
//! path, up to levels deep in the hierarchy.
std::list<QFileInfo> getFileInfoList( QStringList path, QString pattern, int levels = 8 );

//! Splits string first with delimiter1 and again with delimiter2.
std::list<QStringList> SplitSplit( QString delimiter1, QString delimiter2, const QString & theString );

//! Joins the list of stringlists with delimiter1 and the stringlists with delimiter2
QString JoinJoin( QString delimiter1, QString delimiter2, const std::list<QStringList> & theListList );

//! Set the coordinate system of paint so that it is covered by window.
void SetPainterWindow( QPainter & paint, const QRectF & window );

#endif // QUTILITY_H_INCLUDED
