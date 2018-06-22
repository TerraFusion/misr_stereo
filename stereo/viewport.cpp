#include <iostream>

#include "glutaux.h"
#include "viewport.h"

viewport::viewport( std::string fileName )
{
    const char * fieldName[3] = { "Red Radiance/RDQI", "Green Radiance/RDQI", "Blue Radiance/RDQI" };

    file = hdfFile( fileName );
    if ( file.IsValid() )
    {
        for ( int i = 0 ; i < 3 ; i++ )
        {
            fields[i] = file->Field( fieldName[i] );
            if ( fields[i].IsNull() )
            {
                std::cout << "Error loading field\n\t" << fieldName[i] << '\n';
                exit( 0 );
            }
        }

        // determine maximum width and height
        width = max( fields[0]->XDim(), fields[1]->XDim(), fields[2]->XDim() );
        height = max( fields[0]->YDim(), fields[1]->YDim(), fields[2]->YDim() );

        // build lookup tables
        for ( int i = 0 ; i < 3 ; i++ )
        {
            blockX[i].resize( width );
            for ( int x = 0 ; x < width ; x++ )
                blockX[i][x] = x * fields[i]->XDim() / width;

            blockY[i].resize( height );
            for ( int y = 0 ; y < height ; y++ )
                blockY[i][y] = y * fields[i]->YDim() / height;
        }

        // create channel data buffers
        for ( int i = 0 ; i < 3 ; i++ )
        {
            fillValue[i] = *(unsigned short *)fields[i]->FillValue();
            channels[i].resize( fields[i]->XDim(), fields[i]->YDim() );
        }

        // resize block image
	    blockImage.resize( width * height * 4 );
    }
    else
    {
        std::cout << "Error loading file:\n\t" << fileName << '\n';
        exit( 0 );
    }
}

int viewport::MinBlock() const
{
    return file->StartBlock();
}

int viewport::MaxBlock() const
{
    return file->EndBlock();
}

void viewport::AllocTextures( int minBlock, int maxBlock )
{
    textures.resize( maxBlock + 1 );
    glGenTextures( maxBlock - minBlock + 1, & textures[ minBlock ] );
}

void viewport::DestroyTextures( int minBlock, int maxBlock )
{
    glDeleteTextures( maxBlock - minBlock + 1, & textures[ minBlock ] );
}

std::vector<unsigned char> & viewport::BlockImage( int blockIndex, double maxVal )
{
    for ( int i = 0 ; i < 3 ; i++ )
    {
        fields[i]->ReadBlock( &channels[i](0,0), blockIndex );
    }

    const float rScale = 255 * fields[0]->Scale() / maxVal;
    const float gScale = 255 * fields[1]->Scale() / maxVal;
    const float bScale = 255 * fields[2]->Scale() / maxVal;

    unsigned char * pixel = & blockImage[0];
    
    for ( int y = 0 ; y < height ; y++ )
    {
        for ( int x = 0 ; x < width ; x++, pixel += 4 )
        {
            unsigned short r = channels[0]( blockX[0][x], blockY[0][y] );
            unsigned short g = channels[1]( blockX[1][x], blockY[1][y] );
            unsigned short b = channels[2]( blockX[2][x], blockY[2][y] );
            bool isTransparent = ( r == fillValue[0] ) || ( g == fillValue[1] ) || ( b == fillValue[2] );
            
            pixel[0] = ( unsigned char ) clamp( ( r >> 2 ) * rScale, 0.0f, 255.0f );
            pixel[1] = ( unsigned char ) clamp( ( g >> 2 ) * gScale, 0.0f, 255.0f );
            pixel[2] = ( unsigned char ) clamp( ( b >> 2 ) * bScale, 0.0f, 255.0f );
    	    pixel[3] = isTransparent ? 0 : 255;
        }
    }

    return blockImage;
}

void viewport::CreateTextureFromImage( int blockIndex )
{
    glBindTexture( GL_TEXTURE_2D, textures[ blockIndex ] );
    
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, & blockImage[0] );

    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
}

vec2d viewport::BlockCenter( int blockIndex ) const
{
    hdfRect blockRect = file->BlockRect( blockIndex );
    hdfCoord blockCenter = blockRect.Center();
    return vec2d( blockCenter.x, blockCenter.y );
}

vec2d viewport::BlockSize() const
{
    hdfRect blockRect = file->BlockRect( MinBlock() );
    return vec2d( blockRect.Width(), blockRect.Height() );
}

vec2d viewport::BlockImageSize() const
{
    return vec2d( width, height );
}

double viewport::PixelsPerMeter() const
{
    return width / BlockSize().x();
}

void viewport::DrawBlock( int blockIndex ) const
{
    glBindTexture( GL_TEXTURE_2D, textures[blockIndex] );
    hdfRect blockRect = file->BlockRect( blockIndex );
    glBegin( GL_TRIANGLE_STRIP );
    {
        glTexCoord2d( 0, 0 );
        glVertex2d( blockRect.Left(), blockRect.Top() );
        glTexCoord2d( 1, 0 );
        glVertex2d( blockRect.Right(), blockRect.Top() );
        glTexCoord2d( 0, 1 );
        glVertex2d( blockRect.Left(), blockRect.Bottom() );
        glTexCoord2d( 1, 1 );
        glVertex2d( blockRect.Right(), blockRect.Bottom() );
    }
    glEnd();
}
