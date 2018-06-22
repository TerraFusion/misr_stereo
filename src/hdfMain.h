#ifndef HDF_H_INCLUDED
#define HDF_H_INCLUDED

#include "ptr.h"

#include "hdfDataNode.h"
#include "hdfDataSource.h"
#include "hdfDataOp.h"
#include "hdfDataProjector.h"
#include "hdfStandardDeviation.h"
#include "hdfField.h"
#include "hdfFile.h"
#include "hdfGrid.h"
#include "hdfImage.h"

class QString;

//! Gets a field from a hdfFile
//! \param file the hdf file to read from
//! \param fieldName the name of the field to return
hdfField LoadField( hdfFile file, const QString & fieldName );

//! Gets a field from a specific grid in a hdfFile
//! \param file the hdf file to read from
//! \param gridName the name of the grid to read from
//! \param fieldName the name of the field to return
hdfField LoadField( hdfFile file, const QString & gridName, const QString & fieldName );

//! Converts a camera number to a human readable name.
//! \param cameraNumber MISR camera number (1-9)
const QString& CameraName( int cameraNumber );

//! Extracts the orbit number from a file name.
//! This may fail if the file has been renamed, but there is currently
//! no easy method for retreiving the orbit number.
int OrbitNumber( QString fileName );

#endif // HDF_H_INCLUDED
