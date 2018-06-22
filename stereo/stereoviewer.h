#ifndef STEREOVIEWER_H_INCLUDED
#define STEREOVIEWER_H_INCLUDED

#include <vector>
#include <string>

#include "vec2.h"
#include "interpolator.h"

#include "misr_orbits.h"

class help;
class viewport;

class stereoViewer
{
public:
    stereoViewer(MISR_Orbits *orbits);

    ~stereoViewer();

    void MousePress( const vec2d & pos, int button );

    void MouseRelease( const vec2d & pos, int button );

    void MouseMotion( const vec2d & delta, int button );

    void AsciiKeyPress( unsigned char key );

    void SpecialKeyPress( int key );

    void Resize( const vec2d & newSize );

    int Update();

    void SetWorldTransform();

    void Draw();

    void MakeBlockTexture( int blockIndex );

    /**
     * @brief The functions calls each viewport to draw its blocks.
     *
     * The functions takes the current view, and calls each viewport
     * to draw it's blocks
     */
    void DrawBlocks(unsigned int view);

    void GoToBlock( int blockIndex );

    void Move( const vec2d & blockDelta );

    void MoveTo( const vec2d & newBlockPosition );

    void UpdateView();

    int NextBlockToLoad();

protected: 

    struct viewport_set
      {
         viewport *v1;
         viewport *v2;

         GLuint   globe_texture;
         GLubyte  *globe_png;

         unsigned int globe_png_w;
         unsigned int globe_png_h;
      };

    int misr_load_globe_texture(const char *path,
                                unsigned int orbit_path,
                                stereoViewer::viewport_set *s); 
    
    int misr_load_globe_spath_texture(const char *path);
    
    

    void switch_current_view(unsigned int view);
    void next_orbit();
    void prev_orbit();

    void draw_globe();



    /****************************/

    void CreateTexture( int blockIndex );

    vec2d ScreenToWorld( const vec2d & pos ) const;
    vec2d WorldToScreen( const vec2d & pos ) const;

    vec2d WorldToBlock( const vec2d & pos ) const;
    vec2d BlockToWorld( const vec2d & pos ) const;

    vec2d ScreenToBlock( const vec2d & pos ) const;
    vec2d BlockToScreen( const vec2d & pos ) const;

    vec2d ScreenToBlockDelta( const vec2d & delta ) const;
    
    vec2d ScreenToWorldDelta( const vec2d & delta ) const;
     
    vec2d WorldToBlockDelta( const vec2d & delta ) const;
	
    void ClearBlockTextures();

    double maxVal;
    int buttonsPressed;
    double zoomRatio, pixelsPerMeter;
    int minBlock, maxBlock;
    int minViewBlock, maxViewBlock;
    vec2d blockPosition;
    vec2d worldPosition;
    vec2d screenPosition;
    vec2d screenSize;
    vec2d blockSize;
    help* helpDisplay;
    
    interpolator<double,vec2d> blockToWorld;
    std::string blockInput;
    std::vector<bool> blockTextureValid;


    /*************************************/
    std::vector<viewport_set *> m_viewports;

    int m_current_view;

    MISR_Orbits *m_orbits;
    bool m_show_globe;


    GLuint  spath_texture[180];
};

#endif // STEREOVIEWER_H_INCLUDED
