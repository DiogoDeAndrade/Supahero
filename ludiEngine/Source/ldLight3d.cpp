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
// ldLight3d.cpp: implementation of the ldLight3d class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldLight3d.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldLight3D::ldLight3D()
{
	_enable=true;
	_type=LDLIGHT_DIRECTIONAL;
	_diffuse.x=1.0f; _diffuse.y=1.0f; _diffuse.z=1.0f; _diffuse.w=1.0f; 
	_specular.x=1.0f; _specular.y=1.0f; _specular.z=1.0f; _specular.w=1.0f; 
	_ambient.x=0.1f; _ambient.y=0.1f; _ambient.z=0.1f; _ambient.w=0.1f; 
	_range=LDLIGHT_RANGE_MAX;
	_falloff=1.0f;
	_attenuation.x=1.0f; _attenuation.y=0.0f; _attenuation.z=0.0f;
	_cone.x=deg_to_rad(30.0f); _cone.y=deg_to_rad(50.0f);
}

ldLight3D::~ldLight3D()
{

}
