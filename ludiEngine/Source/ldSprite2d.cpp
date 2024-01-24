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
// ldSprite2d.cpp: implementation of the ldSprite2d class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldSprite2d.h"
#include "..\HEADERS\ldGfx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldSprite2d::ldSprite2d()
{
	_pos.x=0; _pos.y=0;
	_u1=_v1=0.0f;
	_u2=_v2=1.0f;
	_color.set(1.0f,1.0f,1.0f,1.0f);
	_size_x=_size_y=32.0f;
	_texture=NULL;
}

ldSprite2d::~ldSprite2d()
{

}

void ldSprite2d::draw()
{
	if (!_texture) return;

	GFX->draw_sprite(&_pos,_size_x,_size_y,&_color,_u1,_v1,_u2,_v2,_texture);
}
