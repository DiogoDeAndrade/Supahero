/******************************************************************************************
* AUTHOR: Diogo de Andrade <diogo.andrade@netvisao.pt>                                    *
*                                                                                         *
* This file, and all files in the LudiEngine is released with the 'I don't give a damn'   *
* licence... This means I'm not responsible for anything that may happen due to the use   *
* of this engine, but you can use it in any way you like it...                            *
* I would just appreciate (although it is not mandatory) if some comments on it could be  *
* made, revisions you make yourself, new filters, etc, etc, etc...                        *
* Credit where credit is due...                                                           *
*                                                                                         *
* COPYRIGHT: Copyright 2002, Diogo Andrade. All rights reserved.                          *
******************************************************************************************/

#ifndef __LDUTILS_H
#define __LDUTILS_H

#include "ldGeometry.h"

// Converts 4 floats representing a color to a colorref (DWORD)
inline unsigned long float_to_colorref(float r,float g,float b,float a) 
{ 
	return (int(a*255.0f) << 24)+(int(r*255.0f) << 16)+(int(g*255.0f) << 8)+(int(b*255.0f)); 
}
// Converts a 4d vector representing a color to a colorref (DWORD)
inline unsigned long float_to_colorref(Vec4f *color) 
{ 
	return (int(color->w*255.0f) << 24)+(int(color->x*255.0f) << 16)+(int(color->y*255.0f) << 8)+(int(color->z*255.0f)); 
}

inline float get_random_number(float min,float max)
{
	if (min==max) return min;

	float d=float(rand())/float(RAND_MAX);

	return d*(max-min)+min;
}

#define Sqr(x) ((x)*(x))

float get_angle_betwen_vectors(float x1,float y1,float x2,float y2);

#endif