#ifndef __MISR_ORBITS_H__
#define __MISR_ORBITS_H__


/**
 * The class is used as a container to keep loaded *.hdf configuration.
 */


struct misr_orbit
{
  /**
   * @brief The orbit number
   */
  unsigned int o_number;


  /**
   * @brief The left cam name
   */
  char *o_left_cam;

  /**
   * @brief The right cam name
   */
  char *o_right_cam;

  /**
   * @brief The *.HDF file defining the left cam.
   */
  char *o_left_cam_hdf;

  /**
   * @brief The *.HDF file defining the right cam.
   */
  char *o_right_cam_hdf;

  unsigned int o_left_cam_path;

  unsigned int o_right_cam_path;
};

class MISR_Orbits
{
   public:
      MISR_Orbits(const char *path);
      virtual ~MISR_Orbits();

      /**
       * @brief The function adds a new orbit into the set.
       *
       * @param number  - The orbit number;
       * @param hdf     - The hdf file name defining the orbit;
       * @param cam     - The orbit camera used.
       * @param left    - Check if it is a left cam 
       *
       * @return On success the function retuns 0. Otherwise < 0
       */
      int add(unsigned int number, unsigned int path, const char *hdf, const char *cam, bool left);


      /**
       * @brief The function sorts orbits in the ascending order.
       *
       * @return On success 0 is returned. Otherwise < 0 is returned.
       *
       * @note The function should be called manually by the class user
       * in order to get the order of orbits in ascending order. Otherwise
       * the order of orbits will be defined by the order of *.hdf file creation
       * on the disc.
       */
      int sort_orbits();


      /**
       * @brief The function returns the number of orbits within the set.
       */
      unsigned int get_orbits_num() const;


      const misr_orbit *get_orbit(unsigned int index) const;


      const char *get_data_path() const;
          


   private:
      int allocate_orbits_array(unsigned int size);


   private:
      char *m_path;

      /**
       * @brief An array used to store orbits;
       */
      misr_orbit **m_orbits;

      /**
       * @brief The total number of valid orbits stored in the m_orbits
       */
      unsigned int m_orbits_num;

      /**
       * @brief The size of the m_orbits array
       */
      unsigned int m_orbits_size;

};





#endif // __MISR_ORBITS_H__
