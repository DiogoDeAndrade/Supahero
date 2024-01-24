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
// ldSprite2d.h: interface for the ldSprite2d class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDSPRITE2D_H__387A7308_53FE_42AA_AC2B_6E16F80750F2__INCLUDED_)
#define AFX_LDSPRITE2D_H__387A7308_53FE_42AA_AC2B_6E16F80750F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldGeometry.h"

class ldTexture;

class ldSprite2d  
{
public:
	ldSprite2d();
	virtual ~ldSprite2d();

	inline	void	set_image(ldTexture *texture) { _texture=texture; }
	inline	void	set_size(float size_x,float size_y) { _size_x=size_x; _size_y=size_y; }
	inline	void	set_image_limits(float u1,float v1,float u2,float v2) { _u1=u1; _v1=v1; _u2=u2; _v2=v2; }
	inline	void	set_color(float r,float g,float b,float a) { _color.x=r; _color.y=g; _color.z=b; _color.w=a; }

	virtual	void	draw();

public:
	Vec2f	_pos;

protected:
	float		_u1,_v1;
	float		_u2,_v2;
	Vec4f		_color;
	float		_size_x;
	float		_size_y;
	ldTexture	*_texture;
};

#endif // !defined(AFX_LDSPRITE2D_H__387A7308_53FE_42AA_AC2B_6E16F80750F2__INCLUDED_)
