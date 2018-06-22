#ifdef WIN32
#pragma comment( lib, "hdfeos" )
#pragma comment( lib, "gctp" )
#endif

#include <memory>
#include <cstdio>
#include "glutaux.h"
#include "stereoviewer.h"
#include <iostream>       // std::cout
#include <string>         // std::string
#include <string.h>
#include "config.h"


#include <dirent.h> 

#include "misr_orbits.h"


#define ORBIT_PREFIX "MISR_AM1_GRP_ELLIPSOID_GM_P"

stereoViewer* stereo = 0;

int mouseX = 0;
int mouseY = 0;

bool mouseLeft = false;
bool mouseRight = false;


static unsigned int s_timer = 0;

static MISR_Orbits *_misr_get_orbits_list(const char *path, 
                                          const char *left_cam, 
                                          const char *right_cam,
                                          const char *prefix);
static void         _misr_print_help_msg(const char *prog);
static void         _misr_on_timer(int );





void OnMouseClick( int button, int state, int x, int y )
{
    if ( button == GLUT_LEFT_BUTTON )
    {
        mouseLeft = ( state == GLUT_DOWN );
        
        if ( mouseLeft )
        {
            stereo->MousePress( vec2d( x, y ), 0 );
        }
        else
        {
            stereo->MouseRelease( vec2d( x, y ), 0 );
        }  
    }
    else if ( button == GLUT_RIGHT_BUTTON )
    {
        mouseRight = ( state == GLUT_DOWN );
        
        if ( mouseRight )
        {
            stereo->MousePress( vec2d( x, y ), 1 );
        }
        else
        {
            stereo->MouseRelease( vec2d( x, y ), 1 );
        }
    }
	else // don't need to save state of other buttons
	{
		if ( state == GLUT_DOWN )
		{
            stereo->MousePress( vec2d( x, y ), button );
        }
        else
        {
            stereo->MouseRelease( vec2d( x, y ), button );
		}
	}

    mouseX = x;
    mouseY = y; 
    
    s_timer = 0; // Stop autoscrolling
}

void OnMouseMotion( int x, int y )
{
    if ( mouseLeft ) stereo->MouseMotion( vec2d( x - mouseX, y - mouseY ), 0 );
    if ( mouseRight ) stereo->MouseMotion( vec2d( x - mouseX, y - mouseY ), 1 );

    mouseX = x;
    mouseY = y;
}

void OnAsciiKeyPress( unsigned char key, int, int )
{
   // s_timer = 0;

    stereo->AsciiKeyPress( key );
}

void OnSpecialKeyPress( int key, int, int )
{
    s_timer = 0; // Stop autoscrolling

    stereo->SpecialKeyPress( key );
}

void OnResize( int width, int height )
{
    s_timer = 0;

    stereo->Resize( vec2d( width, height ) );
}

void OnUpdate()
{

   stereo->Update();
}

void OnDraw()
{
    stereo->Draw();

}


int 
main( int argc, char *argv[] )
{ 
   const char *__data_dir = NULL;
   const char *__lcam = NULL;
   const char *__rcam = NULL;
   const char *__prefix = NULL;

   printf("\nThe misr-stereo root path is : \n\t%s\n", PACKAGE_DATA_DIR);
   if (argc < 4)
     {
        _misr_print_help_msg(argv[0]);
        return -1;
     }
   else if (argc == 4)
     {
        __data_dir   = argv[1];
        __lcam       = argv[2];
        __rcam       = argv[3];
        __prefix     = ORBIT_PREFIX;
     }
   else if (argc == 5)
     {
        __data_dir   = argv[1];
        __lcam       = argv[2];
        __rcam       = argv[3];
        __prefix     = argv[4];
     }
   else
     {
        _misr_print_help_msg(argv[0]);
        return -1;
     }

   MISR_Orbits *mo = NULL;
   mo = _misr_get_orbits_list(__data_dir, __lcam, __rcam, __prefix);

   if (!mo)
     {
        printf("Error: Fail to load the list of MISR data files.\n");
        _misr_print_help_msg(argv[0]);
        return -1;
     }

   if (mo->get_orbits_num() == 0)
     {
        printf("Error: Could not locate any MISR *.hdf files :\n"
               "\t data_dir : %s\n"
               "\t lcam     : %s\n"
               "\t rcam     : %s\n"
               "\t prefix   : %s\n", __data_dir, __lcam, __rcam, __prefix);
        printf("\n"); 
        
        _misr_print_help_msg(argv[0]);
        return -1;
     } 

   printf("The current loaded *.hdf configuration:\n\n");
   for (unsigned int i = 0; i < mo->get_orbits_num(); i++)
     {
        const misr_orbit *o = mo->get_orbit(i);
        if (!o)
          continue;

        printf("[%d] : \n\t%s\n\t\torbit : %d\n\t\tpath  : %d\n\t%s\n\t\torbit : %d\n\t\tpath  : %d\n",
               i,
               o->o_left_cam_hdf, o->o_number, o->o_left_cam_path,
               o->o_right_cam_hdf, o->o_number, o->o_right_cam_path);
     }

   for (unsigned int i = 0; i< mo->get_orbits_num(); i++)
     {
        const misr_orbit *o = mo->get_orbit(i);
        if (!o)
          continue;

        if (o->o_left_cam_hdf == NULL || o->o_left_cam_hdf[0] == 0)
          { 
             printf("Error: Could not locate MISR *.hdf file for left camera %s in orbit %d. Exiting.\n",
                    __lcam,
                    o->o_number);
             exit(-1);
          }

        if (o->o_right_cam_hdf == NULL || o->o_right_cam_hdf[0] == 0)
          { 
             printf("Error: Could not locate MISR *.hdf file for right camera %s in orbit %d. Exiting.\n",
                    __rcam,
                    o->o_number);
             exit(-1);
          }
     }
         
   
   glutInit( &argc, argv );
   glutInitWindowPosition( 0,0 );
   glutInitWindowSize( 800, 600 );
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );

   glutCreateWindow( "MISR HDF Viewer" );

   glutReshapeFunc( OnResize );
   glutMouseFunc( OnMouseClick );
   glutMotionFunc( OnMouseMotion );
   glutKeyboardFunc( OnAsciiKeyPress );
   glutSpecialFunc( OnSpecialKeyPress );
   glutDisplayFunc( OnDraw );

   glutTimerFunc(1000, _misr_on_timer, 0);

   glutIdleFunc( OnUpdate );
   glClearColor( 0.0, 0.0, 0.0, 0.0 );

   stereo = new stereoViewer(mo);

   glutFullScreen();
   glutMainLoop();

    return 0;
}


/********************************************/
static void
_misr_print_help_msg(const char *prog)
{
   printf("Usage: %s <data_dir> <lcam> <rcam> [file_prefix]\n", (prog == NULL ? "prog" : prog));
   printf("\n");
   printf("Displays MISR ellipsoid radiance data on a polarized stereo display\n" );

   printf("\n\t<data_dir> - The directory which contains MISR *.hdf files.\n");
   printf("\t<lcam> - The left camera to use.\n");
   printf("\t<rcam> - The right camera to use.\n");
   printf("\t[file_prefix]  - An optional file name prefix used in search for *.hdf files.\n");
   printf("\t                 The default value used is : MISR_AM1_GRP_ELLIPSOID_GM_\n");
   printf("\n");
   printf("Usage example :\n");
   printf("\t[1] misr_stereo ../data/ AA AN\n");
   printf("\t[2] misr_stereo ../data/ AA AN THIS_IS_MY_HDF_FILE_PREFIX\n");
}


static MISR_Orbits*
_misr_get_orbits_list(const char *path,
                      const char *left_cam,
                      const char *right_cam,
                      const char *prefix)
{
   DIR *d = NULL;
   struct dirent *dir = NULL;

   char _lc_suffix[20] = "";
   char _rc_suffix[20] = "";


   if (!path || !left_cam || !right_cam || !prefix)
     return NULL;

   MISR_Orbits *mo = NULL;


   mo = new MISR_Orbits(path);
   if (!mo)
     return NULL;


   snprintf(_lc_suffix, 20, "_%s_", left_cam);
   snprintf(_rc_suffix, 20, "_%s_", right_cam);


   d = opendir(path);
   if (!d)
     { 
        delete mo;
        return NULL;
     }

   for (;;)
     {
        char *p = NULL;
        char *p1 = NULL;
        char *p2 = NULL;
        char *__dir = NULL;
        unsigned int path = 0;

        dir = readdir(d);
        if (!dir)
          break;



        p = strstr(dir->d_name, prefix);
        if (p == NULL)
          continue;

        __dir = strdup(dir->d_name);
        p = __dir;
        p += strlen(ORBIT_PREFIX);

        /********** get the orbit path (The PXXX part) ******/
        {
           char *__p = strstr(p, "_O");
           if (!__p)
             {
                // the file contains no path definition. Try the next file.
                free(__dir);
                continue;
             }

           *__p = 0;
           path = (unsigned int)atoi(p);

           p = __p + 1;
        }

        p1 = strstr(p, _lc_suffix);
        p2 = strstr(p, _rc_suffix);

        if (p1 == NULL && p2 == NULL)
          {
             // the hdf file does not contain the cam wanted. skip it.
             free(__dir);
             continue;
          }

        if (p1)
          {
             // shift back.
             p1 -= 6;

             // cut off the rest of the string;
             p1[6] = 0;
             unsigned int __orbit_num = atoi(p1);


             mo->add(__orbit_num, path, dir->d_name, left_cam, true);

             // we could have 1 file as a description of 1 orbit;
          }
        else if (p2)
          {
             // shift back.
             p2 -= 6;

             // cut off the rest of the string;
             p2[6] = 0;
             unsigned int __orbit_num = atoi(p2);


             mo->add(__orbit_num, path, dir->d_name, right_cam, false); 
          }


        // Use free instead of delete since strdup uses malloc.
        free(__dir);
     }

   closedir(d);

   mo->sort_orbits();

   return mo;
}



static void
_misr_on_timer(int )
{

   if (s_timer >= 10)
     {
        stereo->MouseMotion( vec2d( 1, 0 ), 0 );
        glutPostRedisplay();

        glutTimerFunc(50, _misr_on_timer, 0);
        s_timer++;
     }
   else
     { 
        s_timer ++; 
        glutTimerFunc(1000, _misr_on_timer, 0);
     }
}
