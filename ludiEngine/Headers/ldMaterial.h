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
// ldMaterial.h: interface for the ldMaterial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDMATERIAL_H__6003BB9D_C419_4972_A628_059706537C9C__INCLUDED_)
#define AFX_LDMATERIAL_H__6003BB9D_C419_4972_A628_059706537C9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldGeometry.h"
#include "ldTexture.h"

typedef enum { LDBLEND_DISABLE, LDBLEND_ARG1, LDBLEND_ARG2, LDBLEND_MODULATE,
			   LDBLEND_MODULATE2X, LDBLEND_MODULATE4X, LDBLEND_ADD,
			   LDBLEND_ADDSIGNED, LDBLEND_ADDSIGNED2X } ldBlendOp;
typedef enum { LDBLENDARG_NONE, LDBLENDARG_CURRENT, LDBLENDARG_DIFFUSE, 
			   LDBLENDARG_SPECULAR, LDBLENDARG_TEXTURE } ldBlendArgument;

struct ldBlendStage {
	ldBlendOp		color_op;
	ldBlendArgument	color_arg1;
	ldBlendArgument	color_arg2;
	ldBlendOp		alpha_op;
	ldBlendArgument	alpha_arg1;
	ldBlendArgument	alpha_arg2;
};

class ldMaterial  
{
public:
	ldMaterial();
	virtual ~ldMaterial();

	virtual	void	set() = 0;

	inline	void	set_diffuse_color(float r,float g,float b,float a) { _diffuse.x=r; _diffuse.y=g; _diffuse.z=b; _diffuse.w=a; }
	inline	void	set_diffuse_color(Vec4f &color) { _diffuse=color; }
	inline	void	set_specular_color(float r,float g,float b,float a) { _specular.x=r; _specular.y=g; _specular.z=b; _specular.w=a; }
	inline	void	set_specular_color(Vec4f &color) { _specular=color; }
	inline	void	set_ambient_color(float r,float g,float b,float a) { _ambient.x=r; _ambient.y=g; _ambient.z=b; _ambient.w=a; }
	inline	void	set_ambient_color(Vec4f &color) { _ambient=color; }
	inline	void	set_emissive_color(float r,float g,float b,float a) { _emissive.x=r; _emissive.y=g; _emissive.z=b; _emissive.w=a; }
	inline	void	set_emissive_color(Vec4f &color) { _emissive=color; }
	inline	void	set_power(float pow) { _power=pow; }
	
	inline	void	set_texture(unsigned long index,ldTexture *texture) { _textures[index]=texture; }
	inline	void	set_color_blend_op(unsigned long index,ldBlendOp op,ldBlendArgument arg1,ldBlendArgument arg2) { _blends[index].color_op=op; _blends[index].color_arg1=arg1; _blends[index].color_arg2=arg2; if (index>=_blends_no) _blends_no=index+1; }
	inline	void	set_alpha_blend_op(unsigned long index,ldBlendOp op,ldBlendArgument arg1,ldBlendArgument arg2) { _blends[index].alpha_op=op; _blends[index].alpha_arg1=arg1; _blends[index].alpha_arg2=arg2; if (index>=_blends_no) _blends_no=index+1; }
	inline	void	reset_blend_ops() { _blends_no=0; }

protected:
	Vec4f			_diffuse;
	Vec4f			_ambient;
	Vec4f			_specular;
	float			_power;
	Vec4f			_emissive;
	ldTexture*		_textures[LDTEXTURE_MAX_NUMBER];
	ldBlendStage	_blends[LDTEXTURE_MAX_NUMBER];
	int				_blends_no;
};

#endif // !defined(AFX_LDMATERIAL_H__6003BB9D_C419_4972_A628_059706537C9C__INCLUDED_)
