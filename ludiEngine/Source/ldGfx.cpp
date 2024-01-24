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
// ldGfx.cpp: implementation of the ldGfx class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldGfx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldGfx	*GFX=NULL;

ldGfx::ldGfx()
{
	GFX=this;
	_windowName=NULL;
}

ldGfx::~ldGfx()
{
	if (_windowName) free(_windowName); _windowName=NULL;
}

bool ldGfx::init(unsigned long res_x,unsigned long res_y,unsigned long bit_depth,bool windowed,
				 char *window_name,
				 void *platform_param1,void *platform_param2,void *platform_param3)
{
	if (_windowName) free(_windowName);
	_windowName=strdup(window_name);
	_resX=res_x;
	_resY=res_y;
	_windowMode=windowed;
	_bitDepth=bit_depth;

	return true;
}

void ldGfx::add_light(ldLight3D *light)
{
	ldLight3DList::iterator	i;

	for (i=_lights.begin(); i!=_lights.end(); ++i)
	{
		if (*i==light) return;
	}

	_lights.push_back(light);
}

void ldGfx::remove_light(ldLight3D *light)
{
	ldLight3DList::iterator	i;

	for (i=_lights.begin(); i!=_lights.end(); ++i)
	{
		if (*i==light) i=_lights.erase(i);
	}
}
