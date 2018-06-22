#include <qfile.h>
#include <qpainter.h>
#include <qstring.h>
#include <qtextstream.h>

#include "hdfDataOp.h"
#include "hdfImage.h"
#include "qutility.h"
#include "utility.h"
#include "projector/geographic.h"

hdfImage::hdfImage( int width, int height )
:
outputProjector( new projectorGeographic() ),
input( 0 ),
outputMin( 0 ),
outputMax( 1 ),
colorMap(),
outImage( width, height, QImage::Format_ARGB32_Premultiplied ),
worldRect( 0, 1, 1, 0 ),
viewRect( 0, 1, 1, 0 ),
imageRect( 0, 0, 1, 1 ),
overSampling( 1 )
{
	colorMap.setDomain( 0, 1 );
	colorMap.Insert( 0, color( 0, 0, 0 ) );
	colorMap.Insert( 1, color( 1, 1, 1 ) );

	static QPainterPath boundariesPath[3];
	std::fill( boundaryProjector, boundaryProjector + 3, ( projector * ) 0 );
}

hdfImage::~hdfImage()
{

}

void hdfImage::setOutputProjector( ptr<projector> theOutputProjector )
{
	if ( theOutputProjector.IsValid() )
	{
		outputProjector = theOutputProjector;
	}
	else
	{
		outputProjector = new projectorGeographic();
	}

	setWorldRect( outputProjector->BoundingRectProjected() );	
}

const ptr<projector>& hdfImage::OutputProjector() const
{
	return outputProjector;
}

void hdfImage::setNumInputs( int theNumInputs )
{
	if ( theNumInputs != NumInputs() )
	{
		input.resize( clamp( theNumInputs, 0, 3 ) );
		fill( input.begin(), input.end(), new hdfDataNode );

		output.resize( input.size() );
		for ( unsigned int channel = 0 ; channel < output.size() ; ++channel )
		{
			output[ channel ].resize( Height(), Width(), hdfValue( 0, 0 ) );
		}
	}
}

int hdfImage::NumInputs() const
{
	return int( input.size() );
}

void hdfImage::setInput( int channel, ptr<hdfDataNode> theInput )
{
	if ( ( channel >= 0 ) && ( channel < NumInputs() ) )
	{
		if ( theInput.IsValid() )
		{
			input[ channel ] = theInput;
		}
		else
		{
			input[ channel ] = new hdfDataNode;
		}
		
		hdfDataNode::PromoteProjectors( input[ channel ] );
	}
}

const ptr<hdfDataNode>& hdfImage::Input( int channel ) const
{
	return input[ channel ];
}

bool hdfImage::CheckInputs() const
{
	for( unsigned int i = 0; i < input.size(); i++ )
	{
		if( ! input[ i ]->Check() )
		{
			return false;
		}
	}
	return true;
}

hdfImage::inputModeType hdfImage::InputMode() const
{
	inputModeType result = none;

	if ( input.size() == 1 )
	{
		result = single;
	}
	else if ( input.size() == 2 )
	{
		result = comparison;
	}
	else if ( input.size() == 3 )
	{
		result = rgb;
	}

	return( result );
}

void hdfImage::setColorScale( const colorScale & theColorMap )
{
	colorMap = theColorMap;
	setOutputRange( colorMap.DomainMin(), colorMap.DomainMax() );
}

const colorScale & hdfImage::ColorScale() const
{
	return colorMap;
}

void hdfImage::setOutputRange( const hdfScalar & min, const hdfScalar & max )
{
	outputMin = min;
	outputMax = max;

	colorMap.setDomain( OutputMin(), OutputMax() );
}

void hdfImage::setOutputMin( const hdfScalar & min )
{
	setOutputRange( min, OutputMax() );
}

const hdfScalar & hdfImage::OutputMin() const
{
	return( outputMin );
}

void hdfImage::setOutputMax( const hdfScalar & max )
{
	setOutputRange( OutputMin(), max );
}

const hdfScalar & hdfImage::OutputMax() const
{
	return( outputMax );
}

void hdfImage::setWorldRect( const hdfRect & theWorldRect )
{
	if( theWorldRect != worldRect )
	{
		worldRect = theWorldRect;
		setViewRect( worldRect.FitOutside( imageRect.SwapY() ) );
		std::cerr << "SetWorldRect()\n";
		std::cerr << "image: " << ( float( Width() ) / Height() )
			<< " view: " << ( viewRect.WidthAbs() / viewRect.HeightAbs() ) << '\n';
	}
}

const hdfRect & hdfImage::WorldRect() const
{
	return( worldRect );
}

void hdfImage::setViewRect( const hdfRect & theViewRect )
{
	viewRect = worldRect.ContainRect( theViewRect );
}

const hdfRect & hdfImage::ViewRect() const
{
	return( viewRect );
}

const hdfRect & hdfImage::ImageRect() const
{
	return( imageRect );
}

void hdfImage::setWidth( int theWidth )
{
	int theHeight = int( theWidth * viewRect.HeightAbs() / viewRect.WidthAbs() );

	setSize( theWidth, theHeight );
}

int hdfImage::Width() const
{
	return outImage.width();
}

void hdfImage::setHeight( int theHeight )
{
	int theWidth = int( theHeight * viewRect.WidthAbs() / viewRect.HeightAbs() );

	setSize( theWidth, theHeight );
}

int hdfImage::Height() const
{
	return outImage.height();
}

void hdfImage::setSize( int theWidth, int theHeight )
{
    // resize output image
	outImage = QImage( theWidth, theHeight, QImage::Format_ARGB32_Premultiplied );

    // resize image translation rectangle
	imageRect = hdfRect( 0, 0, hdfScalar( theWidth ), hdfScalar( theHeight ) );

    // resize output data matrices
	for ( unsigned int channel = 0 ; channel < output.size() ; ++channel )
	{
		output[ channel ].resize( Height(), Width(), hdfValue( 0, 0 ) );
	}
}

void hdfImage::setOverSampling( int value )
{
	overSampling = max( 1, value );
}

int hdfImage::OverSampling() const
{
	return overSampling;
}

void hdfImage::Clear()
{
	outImage.fill( 0 );
}

bool hdfImage::InputsValid() const
{
	for( unsigned int i = 0; i < input.size(); i++ )
	{
		if( input[ i ] == 0 )
		{
			return false;
		}
	}
	
	return true;
}

std::vector<hdfScalar> hdfImage::SampleOffsets( int numSamples ) const
{
	std::vector<hdfScalar> sampleOffsets( numSamples );
	for( int i = 0; i < numSamples; i++ )
	{
		sampleOffsets[ i ] = ( i + 0.5 ) / numSamples;
	}
	return sampleOffsets;
}

void hdfImage::RenderMask()
{       
	Clear();
	QPainter paint( & outImage );
	RenderMask( paint, ViewRect().qRectF() );
}

void hdfImage::RenderMask( QPainter & paint, const QRectF & region )
{
	if ( InputMode() != none )
	{               
		SetPainterWindow( paint, region );

		paint.setBrush( QBrush( QColor( "gray" ) ) );
		paint.setPen( QPen( paint.brush(), 0 ) );

		std::vector<QPolygonF> polygons(
			qPolygonFList(
				OutputProjector()->Project(
					Input( 0 )->GetMask() ) ) );

		for( unsigned int i = 0; i < polygons.size(); i++ )
		{
			paint.drawPolygon( polygons[ i ] );
		}
	}
}

void hdfImage::Render()
{
	Render( 0, 0, Width(), Height() );
}

void hdfImage::Render( const intRect & region )
{
	Render( region.Left(), region.Top(), region.Right(), region.Bottom() );
}

void hdfImage::Render( int xMin, int yMin, int xMax, int yMax )
{
	if( !InputsValid() ) return;

	const int numInputs = input.size();
	std::vector<hdfScalar> sampleOffset = SampleOffsets( overSampling );
	hdfScalar oneOverNumSamples = 1.0 / ( overSampling * overSampling );

	for( int y = yMin ; y < yMax ; ++y )
	{
		for( int x = xMin ; x < xMax ; ++x )
		{
			if( outImage.pixel( x, y ) )
			{
				// clear sample
				for( int i = 0; i < numInputs; i++ )
				{
					output[ i ]( y, x ) = hdfValue( 0, 0 );
				}

				// accumulate sample values, weighted by coverage
				for( int ys = 0; ys < overSampling; ys++ )
				{
					for( int xs = 0; xs < overSampling; xs++ )
					{
						// find the lat/lon location of the sample
						hdfCoord imageSample( hdfScalar( x ) + sampleOffset[ xs ],
										 hdfScalar( y ) + sampleOffset[ ys ] );
						hdfCoord worldSample = outputProjector->UnProject(
							viewRect.ConvertGlobal(	imageRect.ConvertLocal( imageSample ) ) );

						for( int i = 0; i < numInputs; i++ )
						{
							hdfValue rawData( input[ i ]->Value( worldSample ) );
							hdfValue weightedData( rawData.x * rawData.y, rawData.y );
							output[ i ]( y, x ) += weightedData;
						}
					}
				}
				
				// divide by number of samples
				for( int i = 0; i < numInputs; i++ )
				{
					hdfValue& val = output[ i ]( y, x );
					if( val.y > 0 )
						val = hdfValue( val.x / val.y, val.y * oneOverNumSamples );
					else
						val = hdfValue( 0, 0 );
				}
			}
			else
			{
				for( int i = 0; i < numInputs; i++ )
				{
					output[ i ]( y, x ) = hdfValue( 0, 0 );
				}
			}
		}
		progressNotify( y, yMin, yMax );
	}
}

void hdfImage::Colorize()
{
	Colorize( 0, 0, Width(), Height() );
}

void hdfImage::Colorize( const intRect & region )
{
	Colorize( region.Left(), region.Top(), region.Right(), region.Bottom() );
}

void hdfImage::Colorize( int xMin, int yMin, int xMax, int yMax )
{
	if( InputMode() == single )
	{       
		for( int y = yMin ; y < yMax ; ++y )
		{
			for( int x = xMin ; x < xMax ; ++x )
			{
				outImage.setPixel( x, y, colorMap( output[ 0 ]( y, x ).x ).qRgb( convert( output[ 0 ]( y, x ).y, 0, 1 ) ) );
			}
		}
	}
	else if( InputMode() == comparison )
	{       
		hdfOpBinarySub diff;

		for( int y = yMin ; y < yMax ; ++y )
		{
			for( int x = xMin ; x < xMax ; ++x )
			{
				hdfValue dataPixel = diff( output[ 0 ]( y, x ), output[ 1 ]( y, x ) );
				outImage.setPixel( x, y, colorMap( dataPixel.x ).qRgb( convert( dataPixel.y, 0, 1 ) ) );
			}
		}
	}
	else if( InputMode() == rgb )
	{
		for( int y = yMin ; y < yMax ; ++y )
		{
			for( int x = xMin ; x < xMax ; ++x )
			{
				int c[ 4 ];
				for( int i = 0; i < 3; i++ )
				{
					c[ i ] = convert( output[ i ]( y, x ).x, OutputMin(), OutputMax() );
				}
				c[ 3 ] = convert( min( output[ 0 ]( y, x ).y, output[ 1 ]( y, x ).y, output[ 2 ]( y, x ).y ), 0, 1 );
				outImage.setPixel( x, y, qRgba( c[ 0 ], c[ 1 ], c[ 2 ], c[ 3 ] ) );
			}
		}
	}
}

void hdfImage::RenderLine( int y )
{
	if( InputMode() != none )
	{
		std::vector<hdfCoord> location( Width() );

		if( InputMode() == single )
		{       
			if( input[ 0 ] != 0 )
			{
				for( int x = 0 ; x < Width() ; ++x )
				{
					location[ x ] =
						OutputProjector()->UnProject(
							ViewRect().ConvertGlobal(
								ImageRect().ConvertLocal(
									hdfCoord( hdfScalar( x ) + 0.5, hdfScalar( y ) + 0.5 ) ) ) );
				}

				std::vector<hdfValue> values( Input( 0 )->Values( location ) );
				std::copy( values.begin(), values.end(), output[ 0 ].at( y, 0 ) );

				for( int x = 0 ; x < Width() ; ++x )
				{
					outImage.setPixel( x, y, colorMap( output[ 0 ]( y, x ).x ).qRgb( convert( output[ 0 ]( y, x ).y, 0, 1 ) ) );
				}
			}
		}
		else if( InputMode() == comparison )
		{       
			if( ( input[ 0 ] != 0 ) && ( input[ 1 ] != 0 ) )
			{
				for( int x = 0 ; x < Width() ; ++x )
				{
					location[ x ] =
						OutputProjector()->UnProject(
							ViewRect().ConvertGlobal(
								ImageRect().ConvertLocal(
									hdfCoord( hdfScalar( x ) + 0.5, hdfScalar( y ) + 0.5 ) ) ) );
				}

				for( int i = 0; i < 2; i++ )
				{
					std::vector<hdfValue> values( Input( i )->Values( location ) );
					std::copy( values.begin(), values.end(), output[ i ].at( y, 0 ) );
				}

				hdfOpBinarySub compare;
				for( int x = 0 ; x < Width() ; ++x )
				{
					hdfValue dataPixel = compare( output[ 0 ]( y, x ), output[ 1 ]( y, x ) );
					outImage.setPixel( x, y, colorMap( dataPixel.x ).qRgb( convert( dataPixel.y, 0, 1 ) ) );
				}
			}
		}
		else if( InputMode() == rgb )
		{
			if( ( input[ 0 ] != 0 ) && ( input[ 1 ] != 0 ) && ( input[ 2 ] != 0 ) )
			{
				for( int x = 0 ; x < Width() ; ++x )
				{
					location[ x ] =
						OutputProjector()->UnProject(
							ViewRect().ConvertGlobal(
								ImageRect().ConvertLocal(
									hdfCoord( hdfScalar( x ) + 0.5, hdfScalar( y ) + 0.5 ) ) ) );
				}

				for( int i = 0; i < 3; i++ )
				{
					std::vector<hdfValue> values( Input( i )->Values( location ) );
					std::copy( values.begin(), values.end(), output[ i ].at( y, 0 ) );
				}

				for( int x = 0 ; x < Width() ; ++x )
				{
					int c[ 4 ];
					for( int i = 0; i < 3; i++ )
					{
						c[ i ] = convert( output[ i ]( y, x ).x, OutputMin(), OutputMax() );
					}
					c[ 3 ] = convert( min( output[ 0 ]( y, x ).y, output[ 1 ]( y, x ).y, output[ 2 ]( y, x ).y ), 0, 1 );
					outImage.setPixel( x, y, qRgba( c[ 0 ], c[ 1 ], c[ 2 ], c[ 3 ] ) );
				}
			}
		}
	}

	progressNotify( y, 0, Height() );
}

void hdfImage::RenderBoundaries( int mask )
{
	QPainter paint( & outImage );
	RenderBoundaries( mask, paint, viewRect.qRectF() );
}

void hdfImage::RenderBoundaries( int mask, QPainter & paint, const QRectF & region )
{
	SetPainterWindow( paint, region );
	
	for( int i = 0; i < 3; i++ )
	{
		if( mask & ( 1 << i ) )
		{
//			paint.strokePath( BoundariesPath( i ), QPen( QColor( "white" ) ) );
			paint.setPen( Qt::white );
			const hdfLineList& lineList = OutputProjector()->Project( Boundaries( i ) );
			hdfLineList::const_iterator currentLine = lineList.begin();
			while ( currentLine != lineList.end() )
			{
				for ( unsigned int j = 1 ; j < currentLine->size() ; j++ )
				{
					paint.drawLine( (*currentLine)[j-1].qPointF(), (*currentLine)[j].qPointF() );
				}
				++currentLine;
			}

		}
	}
}

const QPainterPath & hdfImage::BoundariesPath( int i )
{
	// if the projector has changed since the last time the path was computed
	if( boundaryProjector[ i ] != outputProjector.Ptr() )
	{
		// save the projector for next time
		boundaryProjector[ i ] = outputProjector.Ptr();

		// reproject the coastlines
		const hdfLineList& lineList = OutputProjector()->Project( Boundaries( i ) );

		// clear path
		boundariesPath[ i ] = QPainterPath();

		// create a path from the coastlines
		hdfLineList::const_iterator currentLine = lineList.begin();
		while ( currentLine != lineList.end() )
		{
			boundariesPath[i].moveTo( (*currentLine)[0].qPointF() );
			for ( unsigned int j = 1 ; j < currentLine->size() ; j++ )
			{
				boundariesPath[i].lineTo( (*currentLine)[j].qPointF() );
			}
			++currentLine;
		}
	}

	return boundariesPath[ i ];
}

const hdfLineList& hdfImage::Boundaries( int i )
{
	static hdfLineList boundariesList[3];
	static bool initialized = false;

    // create the list if it does not exist
	if ( !initialized )
	{
		initialized = true;

		// line types to save
		std::map<int,int> listNum;
		listNum[ 1 ] = 0; // coastlines
		listNum[ 3 ] = 1; // political
		listNum[ 4 ] = 2; // state / province
	
		// open coastlines data file
		QFile outlineFile( getAppPath( "resource" ) + "/coastlines.dat" );
		outlineFile.open( QIODevice::ReadOnly );
		QTextStream outlineStream( & outlineFile );

		while ( true )
		{
			int numValues, lineType, leftArea, rightArea;
			double maxLat, minLat, maxLon, minLon;

            // read the line record header
			outlineStream >> numValues >> lineType
				>> leftArea >> rightArea
				>> maxLat >> minLat >> maxLon >> minLon;

            // done if no points in record
			if ( numValues == 0 ) break;

			hdfCoord newPoint;
			hdfLine line;

            // construct the list of points in this record
            //   convert to longitude, latitude in radians from degrees
			for( int i = 0; i < numValues; i += 2 )
			{
				outlineStream >> newPoint.y >> newPoint.x;
				line.push_back( newPoint * ( 3.141592654 / 180.0 ) );
			}
			
            // save the line to the line list
			std::map<int,int>::const_iterator match = listNum.find( lineType );
			if( match != listNum.end() )
			{
				boundariesList[ match->second ].push_back( line );
			}
		}
	}

	return boundariesList[i];
}

matrix<hdfValue>& hdfImage::Output( int channel )
{
	return output[ channel ];
}

const matrix<hdfValue>& hdfImage::Output( int channel ) const
{
	return output[ channel ];
}

matrix<hdfScalar> hdfImage::OutputData( int channel ) const
{
	matrix<hdfScalar> result( Height(), Width() );

	transform( output[ channel ].begin(), output[ channel ].end(), result.begin(), hdfCoord::functionGetX() );

	return result;
}

matrix<hdfScalar> hdfImage::OutputCoverage( int channel ) const
{
	matrix<hdfScalar> result( Height(), Width() );

	transform( output[ channel ].begin(), output[ channel ].end(), result.begin(), hdfCoord::functionGetY() );

	return result;
}

QImage hdfImage::OutputImage() const
{
	return outImage;
}

void hdfImage::SaveImage( QString fileName )
{
	outImage.save( fileName, "PNG" );
}

void hdfImage::progressNotify( int value, int min, int max )
{
	if ( ( value - min ) != 0 )
	{
		if ( ( value - min ) % 100 == 0 )
		{
			printf( "%6d%%\n", 100 * ( value - min ) / ( max - min ) );
		}
	}

	if ( ( value - min ) % 10 == 0 )  printf( "%6d", value - min );

	fflush( stdout );           
}

QString MakeFileName( QString prefix, QString projection, QString output, QString camera, QString orbit, QString extension )
{
    // prefix
	QString fileName( prefix );

    // projection
	fileName.append( '.' + projection );

    // output
	fileName.append( '.' + output );

    // camera(s)
	if ( camera.length() > 0 ) fileName.append( '.' + camera );

    // orbit number
	fileName.append( '.' + orbit );

    // file extension
	fileName.append( '.' + extension );

	return fileName;
}

int convert( const hdfScalar & c, const hdfScalar & minVal, const hdfScalar & maxVal )
{
	hdfScalar transformed = ( c - minVal ) * hdfScalar( 255 ) / ( maxVal - minVal ) + 0.5;

	return int( clamp( transformed, hdfScalar( 0 ), hdfScalar( 255 ) ) );
}

QImage CreateImage( const matrix<hdfValue> & input, const colorScale & colorMap )
{
	QImage result = CreateImage( input.numCols(), input.numRows() );

	matrix<hdfValue>::const_iterator dataPixel = input.begin();

	for( int y = 0; y < input.numRows(); y++ )
	{
		for( int x = 0; x > input.numCols(); x++, dataPixel++ )
		{
			color outColor = colorMap( dataPixel->x );
			result.setPixel( x, y, QColor::fromRgbF( outColor.r, outColor.g, outColor.b, dataPixel->y ).rgba() );
		}
	}

	return result;
}

QImage CreateImage( const matrix<hdfValue> & input, const hdfScalar & dataMin, const hdfScalar & dataMax, const hdfScalar & coverageMin, const hdfScalar & coverageMax )
{
	QImage result = CreateImage( input.numCols(), input.numRows() );

	matrix<hdfValue>::const_iterator dataPixel = input.begin();

	for( int y = 0; y < input.numRows(); y++ )
	{
		for( int x = 0; x > input.numCols(); x++, dataPixel++ )
		{
			mapper<hdfScalar,double> hueMapper( createInterval( dataMin, dataMax ), createInterval( 4.0 / 6.0, 0.0 ) );
			clamper<double> hueClamper( hueMapper.range );

			mapper<hdfScalar,double> valMapper( coverageMin, coverageMax, 0.2, 0.8 );
			clamper<double> valClamper( valMapper.range );

			double hue = 0.0, sat = 1.0, val = 0.5;

			if ( ( dataPixel->x < dataMin ) || ( dataPixel->y < coverageMin ) )
			{
				// black if less than min data or not enough samples
				val = 0.0;
			}
			else if ( dataPixel->x > dataMax )
			{
				// monochrome if more than max data
				sat = 0.0;
			}
			else
			{
				hue = hueClamper( hueMapper( dataPixel->x ) );
				val = valClamper( valMapper( dataPixel->y ) );
			}

			result.setPixel( x, y, QColor::fromHsvF( hue, sat, val ).rgb() );
		}
	}

	return result;
}

QImage CreateImage( int width, int height, QColor theColor )
{
	QImage result( width, height, QImage::Format_ARGB32_Premultiplied );
	QPainter paint( & result );
	paint.fillRect( QRect( QPoint( 0, 0 ), QSize( width, height ) ), theColor );
	return result;
}

void SaveBinaryData( const matrix<hdfValue> & data, QString prefix )
{
	QString postfix( QString( "%1x%2.bin" ).arg( data.numCols() ).arg( data.numRows() ) );

	QFile dataFile( prefix + "-data-" + postfix );
	QFile coverageFile( prefix + "-coverage-" + postfix );

	dataFile.open( QIODevice::WriteOnly );
	coverageFile.open( QIODevice::WriteOnly );

	QDataStream dataStream( & dataFile );
	QDataStream coverageStream( & coverageFile );

	matrix<hdfValue>::const_iterator current = data.begin();

	while ( current != data.end() )
	{
		dataStream << (*current).x;
		coverageStream << (*current).y;
		++current;
	}

	dataFile.close();
	coverageFile.close();
}
