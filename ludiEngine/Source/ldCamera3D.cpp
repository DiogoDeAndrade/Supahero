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
// ldCamera3D.cpp: implementation of the ldCamera3D class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldCamera3D.h"
#include "..\HEADERS\ldGfx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldCamera3D::ldCamera3D()
{
	_fov=80.0f;
	_near_plane=0.1f;
	_far_plane=1000.0f;
}

ldCamera3D::~ldCamera3D()
{

}

void ldCamera3D::set()
{
	GFX->set_perspective(_fov,_near_plane,_far_plane);
	GFX->set_camera(&pos,&ori);
}
