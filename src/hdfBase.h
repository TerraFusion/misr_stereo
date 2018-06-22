#ifndef HDFBASE_H_INCLUDED
#define HDFBASE_H_INCLUDED

#include <list>

#include "point.h"
#include "polygon.h"
#include "rect.h"

//! Base type for operations. Intended to allow easy selection between float and double,
//! but not all code uses this so it has become impractical.
typedef double hdfScalar;

//! A rectangle with integer coordinates.
typedef rect<int> intRect;

//! A rectangle with floating point coordinates.
typedef rect<hdfScalar> hdfRect;

//! A point with integer coordinates.
typedef point<int> intCoord;

//! A point with floating point coordinates.
typedef point<hdfScalar> hdfCoord;

//! A list of hdfCoords
typedef std::list<hdfCoord> hdfCoordList;

//! The output type for data sources. x is the data value, y is the coverage.
//! For example, y is 0 if none of the pixel is covered and 1 if it is completely covered.
//! y is used in some situations to compute the number of samples, such as before taking an average.
typedef point<hdfScalar> hdfValue;

//! A list of hdfValues
typedef std::list<hdfValue> hdfValueList;

//! A line with floating point coordinates
typedef std::vector<hdfCoord> hdfLine;

//! A list of hdfLines
typedef std::list<hdfLine> hdfLineList;

//! A polygon with floating point coordinates
typedef polygon<hdfCoord> hdfPolygon;

//! PI
const double pi = 3.14159265358979;

#endif // HDFBASE_H_INCLUDED
