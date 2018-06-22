
#include <stdlib.h>
#include <string.h>

#include "misr_orbits.h"


#define DEFAULT_ORBITS_SIZE   2


/****************************/

static int __sort_orbit_compare_fn(const void *l, const void *r);

/****************************/

MISR_Orbits::MISR_Orbits(const char *path) :
   m_path(NULL),
   m_orbits(NULL),
   m_orbits_num(0),
   m_orbits_size(0)
{

   m_path = strdup(path); 
   
   allocate_orbits_array(DEFAULT_ORBITS_SIZE);
}


MISR_Orbits::~MISR_Orbits()
{
   if (m_path)
     free(m_path);

   if (m_orbits)
     {
        for (unsigned int i = 0; i < m_orbits_num; i++)
          { 
             misr_orbit *o = this->m_orbits[i];

             if (o->o_left_cam) 
               free(o->o_left_cam);
             if (o->o_right_cam) 
               free(o->o_right_cam);
             if (o->o_left_cam_hdf) 
               free(o->o_left_cam_hdf);
             if (o->o_right_cam_hdf) 
               free(o->o_right_cam_hdf);

             delete this->m_orbits[i];
          }
     }

   delete m_orbits;
}

int
MISR_Orbits::add(unsigned int number, unsigned int path, const char *hdf, const char *cam, bool left)
{
   int err;
   misr_orbit *_orbit = NULL;

   if (!hdf || !cam)
     return -1;


   // first find if we already have such an orbit defined.
   if (!m_orbits)
     {
        err = allocate_orbits_array(DEFAULT_ORBITS_SIZE);
        if (err < 0)
          return -1;
     }

   for (unsigned int i = 0; i < this->m_orbits_num; i++)
     {
        if (this->m_orbits[i] == NULL)
          continue;

        if (this->m_orbits[i]->o_number == number)
          { 
             _orbit = this->m_orbits[i]; 
             break;
          }

     }

   if (_orbit == NULL)
     {
        // check if we are not out of the array size;
        if (this->m_orbits_num == this->m_orbits_size)
          { 
             err = allocate_orbits_array(this->m_orbits_size * 2);
             if (err < 0)
               return -1;
                
          }

        // create new orbit;
        this->m_orbits[this->m_orbits_num]                   = new misr_orbit;
        this->m_orbits[this->m_orbits_num]->o_number         = number;
        this->m_orbits[this->m_orbits_num]->o_left_cam       = NULL;
        this->m_orbits[this->m_orbits_num]->o_right_cam      = NULL;
        this->m_orbits[this->m_orbits_num]->o_left_cam_hdf   = NULL;
        this->m_orbits[this->m_orbits_num]->o_right_cam_hdf  = NULL;
        this->m_orbits[this->m_orbits_num]->o_left_cam_path  = 0;
        this->m_orbits[this->m_orbits_num]->o_right_cam_path = 0;

        _orbit = this->m_orbits[this->m_orbits_num];

        this->m_orbits_num ++;
     }

   if (left)
     {
        if (_orbit->o_left_cam != NULL || _orbit->o_left_cam_hdf != NULL)
          return -1;

        _orbit->o_left_cam = strdup(cam);
        _orbit->o_left_cam_hdf = strdup(hdf);
        _orbit->o_left_cam_path = path;
     }
   else
     {
        if (_orbit->o_right_cam != NULL || _orbit->o_right_cam_hdf != NULL)
          return -1;

        _orbit->o_right_cam = strdup(cam);
        _orbit->o_right_cam_hdf = strdup(hdf);
        _orbit->o_right_cam_path = path;
     }



   return 0;
}

unsigned int
MISR_Orbits::get_orbits_num() const
{
   return this->m_orbits_num;
}

const misr_orbit *
MISR_Orbits::get_orbit(unsigned int index) const
{
   if (index >= this->m_orbits_num)
     return NULL;

   return this->m_orbits[index];
}

const char *
MISR_Orbits::get_data_path() const
{
   return this->m_path;
}



int
MISR_Orbits::sort_orbits()
{

   if (this->m_orbits == NULL)
     return -1;


   qsort(this->m_orbits, this->m_orbits_num, sizeof(misr_orbit *), __sort_orbit_compare_fn);

   return 0;
}

int
MISR_Orbits::allocate_orbits_array(unsigned int size)
{
   misr_orbit **new_list = NULL;

   if (size < this->m_orbits_size)
     return 0;

   new_list = new misr_orbit* [size];
   if (!new_list)
     return -1;

   for (unsigned int i = 0; i < size; i++)
     {
        if (i < m_orbits_num && this->m_orbits != NULL)
          {
             new_list[i] = this->m_orbits[i];
          }
        else
          {
             new_list[i] = NULL;
          }
     }


   if (this->m_orbits) 
     delete this->m_orbits;

   this->m_orbits = new_list;
   this->m_orbits_size = size;

   return 0;
}

/*******************************************************/
static int
__sort_orbit_compare_fn(const void *l, const void *r)
{
   misr_orbit *o1 = *(misr_orbit **)l;
   misr_orbit *o2 = *(misr_orbit **)r;

   if (o1->o_number < o2->o_number)
     return -1;
   else if (o1->o_number > o2->o_number)
     return 1;

   return 0;
}
