#ifndef __MISR_PNG_HELPER_H__
#define __MISR_PNG_HELPER_H__

#include <GL/glut.h>

/**
 * @brief Loads png image file into memory.
 *
 * @param name - The file name to load.
 * @param width - The width of the png image being loaded.
 * @param height - The height of the png image being loaded.
 * @param alpha - The existance of the alpha level in the png image being loaded.
 *
 * @return On success a pointer to an array containing image data is returned, otherwise NULL.
 */
GLubyte *misr_load_png(const char *name, int &width, int &height, bool &alpha);



#endif //__MISR_PNG_HELPER_H__
