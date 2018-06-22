#ifndef HDFIMAGE_H_INCLUDED
#define HDFIMAGE_H_INCLUDED

#include <qimage.h>
#include <qpainterpath.h>

#include "point.h"
#include "rect.h"
#include "ptr.h"
#include "hdfBase.h"
#include "projector.h"
#include "hdfDataNode.h"
#include "colorScale.h"
#include "matrix.h"

enum borderType
{
	none = 0,
	coastline = 1,
	country = 2,
	state = 4,
	all = 7
};

//! A 2d area used to sample data for display. Given a set of inputs, a rectangle,
//! and a projector, samples the inputs and produces a matrix of data and a image
//! of the data in color. This operates in 2 modes, single channel where the data
//! is mapped to colors using a colorScale, and multiple channel where each data source
//! is mapped to a color channel of the output image.
//! \todo This class does too many things and should be partitioned into
//! multiple classes. The major roadblock for this is drawing the mask without
//! the aid of image rasterization functions.
//! - inputSet: Keeps track of a list of inputs and provides an easy way to query for orbits available for all inputs
//! - region: A rectangular area of interest and a projector
//! - renderer: uses inputSet and region to produce an outputSet
//! - outputSet: stores n matrices of data of the same size, with one extra for coverage
//! - colorizer: maps raw data in a outputSet to visible colors for writing to an image.
//! - vectorRenderer: uses a region to draw properly clipped and transformed lines on an image.
class hdfImage
{

public:

	//! Creates a new data sampler
	//! \param width initial width of the image
	//! \param height initial height of the image
	hdfImage( int width = 128, int height = 128 );

	//! Destructor
	virtual ~hdfImage();

	//! Changes the output map projection.
	//! May call setWorldRect() and setViewRect() if the projection is different.
	virtual void setOutputProjector( ptr<projector> theOutputProjector );

	//! Returns the output map projection.
	const ptr<projector>& OutputProjector() const;

	//! Sets the number of inputs used when dampling data.
	void setNumInputs( int theNumInputs );

	//! Returns the number of inputs used when dampling data.
	int NumInputs() const;

	//! Sets an input to a new hdfDataNode network.
	//! \param channel the input number ( 0 to NumInputs() - 1 )
	//! \param theInput the new input
	void setInput( int channel, ptr<hdfDataNode> theInput );

	//! Returns an input hdfDataNode network.
	//! \param channel the input number ( 0 to NumInputs() - 1 )
	const ptr<hdfDataNode>& Input( int channel ) const;

	//! Descripes how to convert data to an image
	typedef enum
	{
		none,
		single,
		comparison,
		rgb
	} inputModeType;

	//! Returns the data to image conversion mode
	//! This is currently determined only by the number of inputs.
	inputModeType InputMode() const;

	//! Checks all inputs to make sure rendering is possible.
	bool CheckInputs() const;

	//! Changes the color scale used for data to color conversion when
	//! InputMode() is single.
	void setColorScale( const colorScale & theColorScale );

	//! Returns the color scale used for data to color conversion when
	//! InputMode() is single.
	const colorScale & ColorScale() const;

	//! Sets the range of data values mapped to colors when InputMode() is rgb.
	//! \param min the minimum data value to display, will be mapped to black
	//! \param max the maximum data value to display, will be mapped to white
	void setOutputRange( const hdfScalar & min, const hdfScalar & max );

	//! Sets the data value mapped to black when InputMode() is rgb.
	void setOutputMin( const hdfScalar & min );

	//! Returns the data value mapped to black when InputMode() is rgb.
	const hdfScalar & OutputMin() const;

	//! Sets the data value mapped to white when InputMode() is rgb.
	void setOutputMax( const hdfScalar & max );

	//! Returns the data value mapped to white when InputMode() is rgb.
	const hdfScalar & OutputMax() const;

	//! Sets the global bounding rectangle.
	virtual void setWorldRect( const hdfRect & theWorldRect );

	//! Returns the global bounding rectangle.
	const hdfRect & WorldRect() const;

	//! Sets the viewable bounding rectangle.
	//! Controls transformation from image coordinates to world coordinates.
	virtual void setViewRect( const hdfRect & theViewRect );

	//! Returns the viewable bounding rectangle.
	const hdfRect & ViewRect() const;

	//! Returns a reectangle where the upper left corner is ( 0, 0 ) and the
	//! size is ( Width(), Height() ).
	const hdfRect & ImageRect() const;

	//! Sets the width of the output image in pixels.
	void setWidth( int theWidth );

	//! Returns the width of the output image in pixels.
	int Width() const;

	//! Sets the height of the output image in pixels.
	void setHeight( int theHeight );

	//! Returns the height of the output image in pixels.
	int Height() const;

	//! Sets the size of the output image in pixels.
	void setSize( int theWidth, int theHeight );

	//! Sets the number of samples per pixel per axis.
	//! For example, a value of three will divide the pixel into a 3x3 grid
	//! take 9 samples, and average the result for each pixel. This is useful
	//! when resampling data.
	void setOverSampling( int value );

	//! Returns the number of samples per pixel per axis.
	int OverSampling() const;

	//! Clears the output image, does not change Output().
	void Clear();

	//! Returns true if IsValid() is true for all inputs.
	bool InputsValid() const;

	//! Returns a vector of fractional offsets within a pixel for use when
	//! taking multiple samples per pixel. The offsets decribe the distance
	//! from the top or left edge of the pixel to the sample location.
	std::vector<hdfScalar> SampleOffsets( int numSamples ) const;

	//! Draws the outline guaranteed to contain all data for an input to a QPainter
	//! This is useful for easily igoring all pixels where no data is present.
	void RenderMask( QPainter & paint, const QRectF & region );

	//! Draws the outline guaranteed to contain all data for an input onto the
	//! output image. This is useful for easily igoring all pixels where no data
	//! is present.
	void RenderMask();

	//! Samples the inputs and updates output data for all pixels in the image,
	//! converting the output values to colors and updating the image.
	void Render();

	//! Samples the inputs and updates output data for a rectangle in the image,
	//! converting the output values to colors and updating the image.
	void Render( const intRect & region );

	//! Samples the inputs and updates output data for a rectangle in the image,
	//! converting the output values to colors and updating the image.
	void Render( int xMin, int yMin, int xMax, int yMax );

	//! Converts output data values to colors and updates the image.
	void Colorize();

	//! Converts output data values to colors and updates the image.
	void Colorize( const intRect & region );

	//! Converts output data values to colors and updates the image.
	void Colorize( int xMin, int yMin, int xMax, int yMax );

	//! Samples the inputs and updates output data for a scanline in the image,
	//! converting the output values to colors and updating the image.
	void RenderLine( int y );

	//! Draws coastlines on the output image.
	void RenderBoundaries( int mask );

	//! Draws coastlines on a QPainter.
    void RenderBoundaries( int mask, QPainter & paint, const QRectF & region );
	
	//! Returns a path containing all coastlines in a category.
	//! \bug this does not work properly on some platforms and is not currently used.
	const QPainterPath& BoundariesPath( int i );

	//! Returns a list of coastlines in a category.
	static const hdfLineList& Boundaries( int i );

	//! Returns a matrix containing sampled data and coverage.
	//! This is the data used to produce colors in the image.
	matrix<hdfValue> & Output( int channel = 0 );

	//! Returns a matrix containing sampled data and coverage.
	//! This is the data used to produce colors in the image.
	const matrix<hdfValue> & Output( int channel = 0 ) const;

	//! Returns a matrix containing only sampled data.
	//! \see Output
	matrix<hdfScalar> OutputData( int channel = 0 ) const;

	//! Returns a matrix containing only sampled coverage.
	//! \see Output
	matrix<hdfScalar> OutputCoverage( int channel = 0 ) const;

	//! Returns the output image.
	QImage OutputImage() const;

	//! Writes the output image to a file.
	void SaveImage( QString fileName );

protected:

	//! Called as each line of a rectangle is rendered.
	//! Useful for showing rendering progress.
	//! \param value the line that has just been finished
	//! \param min the first line number of the Render() call in progress
	//! \param max the last line number of the Render() call in progress
	virtual void progressNotify( int value, int min, int max );

	ptr<projector> outputProjector; //!< map projection used in output image

	std::vector< ptr< hdfDataNode > > input; //!< list of data inputs
	std::vector< matrix< hdfValue > > output; //!< list of output data matrices.

	hdfScalar outputMin; //!< minimum value for data to RGB conversion
	hdfScalar outputMax; //!< maximum value for data to RGB conversion

	colorScale colorMap; //!< color map for single channel data to color conversion

	QImage outImage; //!< displays data in color and coastlines

	hdfRect worldRect; //!< global bounding rectangle
	hdfRect viewRect; //!< viewable portion of worldRect
	hdfRect imageRect; //!< image area in pixels
	
	int overSampling; //!< number of samples per pixel per axis
	
	QPainterPath boundariesPath[3]; //!< path containing all coastlines in a category
	projector* boundaryProjector[3]; //!< projection that the cached coastlines are currently stored in
};

//! Builds a file name suitable for output when no user interaction is possible.
QString MakeFileName( QString prefix, QString projection, QString output, QString camera, QString orbit, QString extension );

//! Maps a data value to a color component from 0-255.
int convert( const hdfScalar & c, const hdfScalar & minVal, const hdfScalar & maxVal );

//! Creates an image from a matrix of data and a color map.
QImage CreateImage( const matrix<hdfValue> & input, const colorScale & colorMap );

//! Creates a HSV image from a matrix of data and a color map.
//! \param input the input data
//! \param dataMin data value mapped to 0 on the H channel
//! \param dataMax data value mapped to 1 on the H channel
//! \param coverageMin coverage value mapped to 0 on the V channel
//! \param coverageMax coverage value mapped to 1 on the V channel
QImage CreateImage( const matrix<hdfValue> & input, const hdfScalar & dataMin, const hdfScalar & dataMax, const hdfScalar & coverageMin, const hdfScalar & coverageMax );

//! Creates an image with transparency enabled.
QImage CreateImage( int width, int height, QColor theColor = Qt::transparent );

//! Writes data and coverage to files
//! \param data the input data
//! \param prefix the beginning of the file name. Outputs prefix-data.bin and prefix-coverage.bin.
void SaveBinaryData( const matrix<hdfValue> & data, QString prefix );

#endif // HDFIMAGE_H_INCLUDED
