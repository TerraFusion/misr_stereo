#ifndef VIEWPORT_H_INCLUDED
#define VIEWPORT_H_INCLUDED

#include <vector>
#include <string>
#include <map>

#include "hdfFile.h"
#include "hdfField.h"
#include "hdfDataSource.h"

#include "vec2.h"

class viewport
{
public:

    viewport( std::string fileName );

    // returns the minimum valid block index
    int MinBlock() const;

    // returns the maximum valid block index
    int MaxBlock() const;

    // allocate opengl texture names
    void AllocTextures( int minBlock, int maxBlock );

    // release opengl texture names
    void DestroyTextures( int minBlock, int maxBlock );

    // returns a pointer to a RGBA image of merged channels
    std::vector<unsigned char> & BlockImage( int blockIndex, double maxVal );

    // transfers image data to an opengl texture
    void CreateTextureFromImage( int blockIndex );

    // returns the center of the block
    vec2d BlockCenter( int blockIndex ) const;

    // returns the size of a block in meters
    vec2d BlockSize() const;

    // returns the size of a block in pixels
    vec2d BlockImageSize() const;

    double PixelsPerMeter() const;

    // draw the block using opengl
    void DrawBlock( int blockIndex ) const;

protected:

    unsigned short fillValue[3];
    matrix<unsigned short> channels[3];
    std::vector<unsigned char> blockImage;
    int width, height;
    
    hdfFile file;
    hdfField fields[3];

    std::vector<int> blockX[3];
    std::vector<int> blockY[3];

    std::vector<GLuint> textures;
};

#endif // VIEWPORT_H_INCLUDED
