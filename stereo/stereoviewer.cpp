#include <iostream>

#include <glutaux.h>
#include "help.h"
#include "stereoviewer.h"
#include "utility.h"
#include "viewport.h"
#include <GL/glu.h>


#include "config.h"


#include "misr_png_helper.h"

using namespace std;

stereoViewer::stereoViewer(MISR_Orbits *orbits) :
   m_viewports(),
   m_current_view(-1),
   m_orbits(orbits),
   m_show_globe(false)
{
   if (!orbits)
     return;

   misr_load_globe_spath_texture(PACKAGE_DATA_DIR "/spaths/");


   printf("\n");

   for (unsigned int i = 0; i < orbits->get_orbits_num(); i++)
     {
        stereoViewer::viewport_set *s = NULL;

        s = new stereoViewer::viewport_set;
        if (!s)
          continue;

        // make sure that s->v1 and s->v2 are NULL 
        s->v1 = NULL;
        s->v2 = NULL;
        s->globe_png = NULL;
        s->globe_png_w = 0;
        s->globe_png_h = 0;

        const misr_orbit *o = orbits->get_orbit(i);

        if (!o)
          { 
             delete s;
             continue;
          }

        if (o->o_left_cam_hdf)
          {
             std::string __str = std::string(orbits->get_data_path()) + 
                                 "/" +
                                 std::string(o->o_left_cam_hdf);

             s->v1 = new viewport(__str);
             if (!s->v1) 
               {
                  delete s; 
                  continue; 
               } 
          }

        if (o->o_right_cam_hdf)
          { 
             std::string __str = std::string(orbits->get_data_path()) + 
                                 "/" +
                                 std::string(o->o_right_cam_hdf);
             s->v2 = new viewport(__str);
             if (!s->v2)
               {
                  delete s->v1;
                  delete s;
                  continue;
               }
          }

        if (s->v1 == NULL && s->v2 == NULL)
          {
             delete s;
             continue;
          }


        // load and create the OGL texture.
        int err = misr_load_globe_texture(PACKAGE_DATA_DIR "/paths/", o->o_left_cam_path, s);
        if (err < 0)
          printf("\nFailed to load globe texture...\n");

        m_viewports.push_back(s);
     }

   screenPosition = vec2d( 64, 64 ); 
   screenSize = vec2d( 128, 128 ); 

   switch_current_view(0);

    //original maxVal=450
    maxVal = 900;
    buttonsPressed = 0;
}

stereoViewer::~stereoViewer()
{
   for (unsigned int i = 0; i < m_viewports.size(); i++)
     {
        stereoViewer::viewport_set *s = m_viewports[i];
        if (!s)
          continue;

        if (s->v1) 
          delete s->v1;

        if (s->v2)
          delete s->v2; 
        
        glDeleteTextures(1, &(s->globe_texture));

        if (s->globe_png != NULL) 
          free(s->globe_png);

        delete s;
     }
   m_viewports.clear();


   glDeleteTextures(180, this->spath_texture);


   if (m_orbits)
     delete m_orbits;
}

int
stereoViewer::misr_load_globe_spath_texture(const char *path)
{
   char buf[255] = "";
   
   int width;
   int height;
   bool alpha;

   if (!path)
     return -1;

   printf("Loading globe path images...");
   glGenTextures(180, this->spath_texture);
   for (unsigned int i = 0; i < 180; i++)
     { 
        snprintf(buf, 255, "%s/spath%s%s%d.png", path,
                                           i < 100 ? "0" : "",
                                           i < 10 ? "0" : "",
                                           i); 
        

        GLubyte *__png = misr_load_png(buf, width, height, alpha);
        if (!__png){
			printf("Error loading %s\n",buf);
			glBindTexture(GL_TEXTURE_2D, this->spath_texture[i-1]);          
		continue;
		} 
        
        else glBindTexture(GL_TEXTURE_2D, this->spath_texture[i]);


        glTexImage2D(GL_TEXTURE_2D, 
                   0,
                   GL_RGBA,
                   width,
                   height,
                   0,
                   GL_RGBA,
                   GL_UNSIGNED_BYTE,
                   __png); 
        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        free(__png);


     }

   return 0;
}
int
stereoViewer::misr_load_globe_texture(const char *path,
                                      unsigned int orbit_path,
                                      stereoViewer::viewport_set *s)
{
   char buf[255] = "";
   
   int width;
   int height;
   bool alpha;

   if (!path || !s)
     return -1;

   snprintf(buf, 255, "%s/Path%s%s%d.png", path,
                                           orbit_path < 100 ? "0" : "",
                                           orbit_path < 10 ? "0" : "",
                                           orbit_path);

   printf("Loading globe image : %s\n", buf);


   s->globe_png = misr_load_png(buf, width, height, alpha);

   if (s->globe_png == NULL)
     return -1;

   s->globe_png_w = width;
   s->globe_png_h = height;

   glGenTextures(1, &s->globe_texture);

   glBindTexture(GL_TEXTURE_2D, s->globe_texture);


   glTexImage2D(GL_TEXTURE_2D, 
                0,
                GL_RGBA,
                width,
                height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                s->globe_png);

   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   return 0;
}

void
stereoViewer::switch_current_view(unsigned int view)
{
   stereoViewer::viewport_set *s = NULL;

   if (view >= m_viewports.size())
      return; 
     
   s = m_viewports[view];
   
   if (!s)
     return; 

   if ((int)view != this->m_current_view)
     { 
        
        if (s->v1)
          {
             minBlock = s->v1->MinBlock();
             maxBlock = s->v1->MaxBlock();
          } 
        
        if (s->v2)
          {
             minBlock = max(minBlock, s->v2->MinBlock());
             maxBlock = min(maxBlock, s->v2->MaxBlock());
          } 

        if (this->m_current_view >= 0)
          {
             // prevent texture deallocation during a startup.
             stereoViewer::viewport_set *s2 = this->m_viewports[this->m_current_view];

             // Wiping out some memory use
             s2->v1->DestroyTextures(minBlock, maxBlock);
             s2->v2->DestroyTextures(minBlock, maxBlock);
          } 
        
        s->v1->AllocTextures(minBlock, maxBlock);
        s->v2->AllocTextures(minBlock, maxBlock); 
        
        blockToWorld.clear(); 
        
        for (int block = minBlock; block <= maxBlock; block++)
          { 
             blockToWorld.insert(make_pair(double(block), s->v1->BlockCenter(block)));
          } 
        
        blockTextureValid.resize( maxBlock + 1 );
        ClearBlockTextures(); 
     }
   

   // jump to initial state of the block wether we are on the same orbit or no.
   pixelsPerMeter = s->v1->PixelsPerMeter(); 

   // move to block closer to pole
   blockPosition = vec2d(blockToWorld.KeyMax()-20, 0); 
   worldPosition = BlockToWorld( blockPosition ); 
   
   blockSize = s->v1->BlockSize(); 
   zoomRatio = screenSize.y() / (1.1 * s->v1->BlockImageSize().y());

   this->m_current_view = view;
}


void 
stereoViewer::MousePress( const vec2d &, int button )
{ 
   buttonsPressed++; 
   
   if (button == 3)
     SpecialKeyPress( GLUT_KEY_UP ); 
   if (button == 4)
     SpecialKeyPress( GLUT_KEY_DOWN );
}

void 
stereoViewer::MouseRelease( const vec2d &, int )
{
    if ( --buttonsPressed < 0 ) buttonsPressed = 0;
}

void 
stereoViewer::MouseMotion(const vec2d & delta, int button)
{
    if ( button == 0 )
    {
        Move(ScreenToBlockDelta( -delta));
    }
    else if (button == 1)
    {
        zoomRatio *= pow( 100.25 / 100.0, delta.x() );
        UpdateView();
    }
}

bool
IsNumeric( unsigned char key )
{
    return ( key >= '0' && key <= '9' );
}

int 
Numeric( const std::string numberString )
{
    int result = 0;
    int power = 1;
    std::string::const_reverse_iterator pos = numberString.rbegin();
    while ( pos != numberString.rend() )
    {
        result += power * ( *pos - '0' );
        power *= 10;
        pos++;
    }
    return result;
}

void 
stereoViewer::AsciiKeyPress( unsigned char key )
{
    if ( key == '\r' )
    {
        if ( ! blockInput.empty() )
        {
            GoToBlock( Numeric( blockInput ) );
            blockInput.clear();
        }
    }
    else if ( key == 8 )
    {
        if ( ! blockInput.empty() )
        {
            blockInput.resize( blockInput.size() - 1 );
            glutPostRedisplay();
        }
    }
    else if ( IsNumeric( key ) )
    {
        if ( blockInput.size() < 3 )
        {
            blockInput.push_back( key );
            glutPostRedisplay();
        }
    }
    else if ( key == 'h' || key == 'H' ) 
      { 
         // help menu removed
      }
    else if ( key == 's' || key == 'S' )
    {
       stereoViewer::viewport_set *s = NULL;

       s = this->m_viewports[this->m_current_view];
       if (s)
         {
            viewport *temp_v = s->v1;
            s->v1 = s->v2;
            s->v2 = temp_v;

            glutPostRedisplay();
         }
    }
    else if (key == 'g' || key == 'G')
      {
         this->m_show_globe = !this->m_show_globe;
         glutPostRedisplay();
      }
    else if ( key == 'q' || key == 'Q' )
    {
        delete this;
        exit( 0 );
    }
}

void 
stereoViewer::SpecialKeyPress( int key )
{
    if ( key == GLUT_KEY_UP ) 
      { 
         if (maxVal > 50 ) 
           maxVal -= 50;
         
         ClearBlockTextures(); 
         glutPostRedisplay();
      } 
    else if ( key == GLUT_KEY_DOWN )
      {
         if( maxVal < 1500 ) 
           maxVal += 50;
         
         ClearBlockTextures();
         glutPostRedisplay();
      }
printf("maxVal %f\n",maxVal);
}

void 
stereoViewer::Resize( const vec2d & newSize )
{
    zoomRatio *= newSize.y() / screenSize.y();

    screenSize.x() = newSize.x() / 2; // we have just 2 views by definition. //viewports.size();
    screenSize.y() = newSize.y();
    screenPosition = screenSize / 2;

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, screenSize.x(), screenSize.y(), 0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    UpdateView();

    glutPostRedisplay();
}

int 
stereoViewer::Update()
{
   int next;

   
   if ( buttonsPressed == 0 )
     { 
        next = NextBlockToLoad(); 
        
        if ( next >= minBlock && next <= maxBlock ) 
          {
             MakeBlockTexture( next );
             
             if (next >= minViewBlock && next <= maxViewBlock ) 
               { 
                  glutPostRedisplay(); 
               } 
          } 
        else if (next < 0) 
          { 
          } 
     }

   return 0;
}

void
stereoViewer::next_orbit()
{ 
   if (this->m_current_view + 1 >= (int)this->m_viewports.size())
     {
        this->switch_current_view(0);
     }
   else
     {
        this->switch_current_view(this->m_current_view + 1);
     }
}

void
stereoViewer::prev_orbit()
{ 
   if (this->m_current_view - 1 <= 0)
     {
        this->switch_current_view(this->m_viewports.size() - 1);
     }
   else
     {
        this->switch_current_view(this->m_current_view - 1);
     }
}

void 
stereoViewer::SetWorldTransform()
{
    // move to center of screen
    glTranslated( 0.5 * screenSize.x(), 0.5 * screenSize.y(), 0.0 );

    // zoom in or out
    glScaled( zoomRatio * pixelsPerMeter, -zoomRatio * pixelsPerMeter, 1.0 );

    // move to view position
    vec2d worldPosition = BlockToWorld( blockPosition );
    glTranslated( -worldPosition.x(), -worldPosition.y(), 0.0 );
}

void 
stereoViewer::Draw()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // set the viewport's rectangle and draw it
    for (unsigned int i = 0 ; i < 2 ; i++)
    { 
       glViewport( int( i * screenSize.x() ), 0, int( screenSize.x() ), int( screenSize.y() ) ); 
       
       glDepthMask(GL_FALSE);
       
       glEnable(GL_BLEND); 
       glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

       
       glPushMatrix(); 
         { 
            SetWorldTransform();
            DrawBlocks(i);
         } 
       
       glPopMatrix();

       draw_globe(); 
       
       glDisable(GL_BLEND);
       glDepthMask(GL_TRUE);
       
    }
    glutSwapBuffers();
}

void
stereoViewer::draw_globe()
{
   if (!this->m_show_globe)
     return;

   stereoViewer::viewport_set *s = NULL;
   s = this->m_viewports[this->m_current_view];

   if (!s)
     return;

   if (!s->globe_png)
     return;

   glEnable( GL_TEXTURE_2D );


   float __width = s->globe_png_w;
   float __height = s->globe_png_h*2;

   /*float __width = s->globe_png_w * 2;
   float __height = s->globe_png_h * 2;*/

   glPushMatrix();

   {
      glBindTexture(GL_TEXTURE_2D, s->globe_texture);
      glBegin(GL_QUADS);

      glTexCoord2f(0.0, 0.0);
      glVertex2f(10, screenSize.y() - 10);

      glTexCoord2f(0.0, 1.0);
      glVertex2f(10, screenSize.y() - __height - 10);

      glTexCoord2f(1.0, 1.0);
      glVertex2f(10 + __width, screenSize.y() - __height - 10);

      glTexCoord2f(1.0, 0.0);
      glVertex2f(10 + __width, screenSize.y() - 10);

      glEnd();


      {
         int blk = 180-(minViewBlock + (maxViewBlock - minViewBlock) / 2);

         if (blk < 0)
           blk = 0;
		 else if (blk==79)
		   blk=80;
         else if (blk > 180)
           blk = 180;

		 //printf("block: %i\n", blk);
		 
         glBindTexture(GL_TEXTURE_2D, spath_texture[blk]);
         glBegin(GL_QUADS);

         glTexCoord2f(0.0, 0.0);
         glVertex2f(10, screenSize.y() - 10);

         glTexCoord2f(0.0, 1.0);
         glVertex2f(10, screenSize.y() - __height - 10);

         glTexCoord2f(1.0, 1.0);
         glVertex2f(10 + __width, screenSize.y() - __height - 10);

         glTexCoord2f(1.0, 0.0);
         glVertex2f(10 + __width, screenSize.y() - 10);

         glEnd();
      }
   }

   glPopMatrix();

    glDisable( GL_TEXTURE_2D );
//printf("minViewBlock: %i, minBlock: %i\n", minViewBlock, minBlock); 
//printf("maxViewBlock: %i, maxBlock: %i\n", maxViewBlock, maxBlock);
}

void stereoViewer::MakeBlockTexture( int blockIndex )
{
    if ( ! blockTextureValid[ blockIndex ] )
    {
        blockTextureValid[ blockIndex ] = true;

        stereoViewer::viewport_set *s = NULL;
        s = this->m_viewports[this->m_current_view];
        if (!s)
          return; 
        
        unsigned char * pixel1 = NULL;
        unsigned char * pixel2 = NULL;
        
        if (s->v1) 
          { 
             pixel1 = & (s->v1->BlockImage(blockIndex, maxVal))[0];
          }

        if (s->v2) 
          {
             pixel2 = & (s->v2->BlockImage(blockIndex, maxVal))[0];
          }
        
        
        unsigned char * pixel1end = NULL;
        
        if (s->v1)
          { 
             std::vector<unsigned char> & image1 = s->v1->BlockImage(blockIndex, maxVal);
             pixel1end = & image1[ image1.size() ]; 
          }
        
        if (pixel2 && pixel1 && pixel1end)
          { 
             while ( pixel1 < pixel1end ) 
               { 
                  if ( pixel1[3] == 0 || pixel2[3] == 0 ) 
                    { 
                       pixel1[0] = 0; 
                       pixel1[1] = 0; 
                       pixel1[2] = 0; 
                       
                       pixel2[0] = 0;
                       pixel2[1] = 0;
                       pixel2[2] = 0;
                    } 
                  
                  pixel1 += 4;
                  pixel2 += 4;
               }
          } 
        
        if (s->v1) 
          s->v1->CreateTextureFromImage( blockIndex );

        if (s->v2) 
          s->v2->CreateTextureFromImage( blockIndex ); 
    }
}

void 
stereoViewer::DrawBlocks(unsigned int view)
{
    glEnable( GL_TEXTURE_2D );

    for (int block = minViewBlock ; block <= maxViewBlock ; block++) 
      { 
         //MakeBlockTexture( block );
         if ( blockTextureValid[ block ] ) 
           { 
              stereoViewer::viewport_set *s = NULL;
              s = this->m_viewports[this->m_current_view];
              if (s)
                { 
                   if (view == 0 && s->v1)
                     s->v1->DrawBlock(block);

                   if (view == 1 && s->v2)
                     s->v2->DrawBlock(block);
                } 
           }
      }    
    glDisable( GL_TEXTURE_2D );
}

void 
stereoViewer::ClearBlockTextures()
{
    fill( blockTextureValid.begin(), blockTextureValid.end(), false );
}

void 
stereoViewer::GoToBlock( int blockIndex )
{
    MoveTo( vec2d( blockIndex, 0 ) );
}

void 
stereoViewer::Move( const vec2d & blockDelta )
{
    MoveTo( blockPosition + blockDelta );
}

void 
stereoViewer::MoveTo( const vec2d & newBlockPosition )
{
    blockPosition = newBlockPosition;
    //blockPosition.x() = clamp( blockPosition.x(), blockToWorld.KeyMin(), blockToWorld.KeyMax() );
    
    if (blockPosition.x() < blockToWorld.KeyMin())
      { 
         next_orbit();
      }
    else if (blockPosition.x() > blockToWorld.KeyMax())
      {
         prev_orbit();
      }
    blockPosition.y() = clamp( blockPosition.y(), -0.4, 0.4 );

    worldPosition = BlockToWorld( blockPosition );
    UpdateView();
}

void 
stereoViewer::UpdateView()
{
    minViewBlock = int( floor( ScreenToBlock( vec2d( 0, screenSize.y() / 2 ) ).x() ) );
    minViewBlock = max( minViewBlock, minBlock );

    maxViewBlock = int( ceil( ScreenToBlock( vec2d( screenSize.x(), screenSize.y() / 2 ) ).x() ) );
    maxViewBlock = min( maxViewBlock, maxBlock );

    glutPostRedisplay();
}

int 
stereoViewer::NextBlockToLoad()
{
    int closestLeft = int( blockPosition.x() );
    while (closestLeft >= minBlock)
    { 
       if (!blockTextureValid[ closestLeft ])
         break; 
       
       closestLeft--;
    }

    int closestRight = int( blockPosition.x() );
    while ( closestRight <= maxBlock )
    {
        if ( ! blockTextureValid[ closestRight ] ) break;
        closestRight++;
    }

    // if left is bad
    if ( closestLeft < minBlock || closestLeft > maxBlock )
    {
        // if right is bad
        if ( closestRight < minBlock || closestRight > maxBlock )
        {
            return -1;
        }
        else
        {
            return closestRight;
        }
    }

    // if right is bad
    if ( closestRight < minBlock || closestRight > maxBlock )
    {
        return closestLeft;
    }

    // pick closest
    if ( minViewBlock - closestLeft < closestRight - maxViewBlock )
    {
        return closestLeft;
    }
    else
    {
        return closestRight;
    }
}

vec2d 
stereoViewer::ScreenToWorld( const vec2d & pos ) const
{
    return ( pos - screenPosition ) / ( zoomRatio * pixelsPerMeter ) * vec2d( 1, -1 ) + worldPosition;
}

vec2d 
stereoViewer::WorldToScreen( const vec2d & pos ) const
{
    return ( pos - worldPosition ) * zoomRatio * pixelsPerMeter * vec2d( 1, -1 ) + screenPosition;
}

vec2d 
stereoViewer::WorldToBlock( const vec2d & pos ) const
{
    double blockX = ( pos.x() - blockToWorld.ValueMin().x() ) / blockSize.x() + blockToWorld.KeyMin();
    double worldY = BlockToWorld( vec2d( blockX, 0 ) ).y();
    return vec2d( blockX, ( pos.y() - worldY ) / blockSize.y() );
}

vec2d 
stereoViewer::BlockToWorld( const vec2d & pos ) const
{
    return blockToWorld( pos.x() ) + vec2d( 0, pos.y() * blockSize.y() );
}

vec2d 
stereoViewer::ScreenToBlock( const vec2d & pos ) const
{
    return WorldToBlock( ScreenToWorld( pos ) );
}

vec2d 
stereoViewer::BlockToScreen( const vec2d & pos ) const
{
    return WorldToScreen( BlockToWorld( pos ) );
}

vec2d 
stereoViewer::ScreenToBlockDelta( const vec2d & delta ) const
{
    return WorldToBlockDelta( ScreenToWorldDelta( delta ) );    
}

vec2d 
stereoViewer::ScreenToWorldDelta( const vec2d & delta ) const
{
    return delta * vec2d( 1, -1 ) / ( zoomRatio * pixelsPerMeter );
}

vec2d 
stereoViewer::WorldToBlockDelta( const vec2d & delta ) const
{
    return delta / blockSize;
}
