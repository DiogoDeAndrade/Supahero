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
// ldLight3d.h: interface for the ldLight3d class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDLIGHT3D_H__DEDF91FA_FE2B_4611_B901_6D3EC1C58C54__INCLUDED_)
#define AFX_LDLIGHT3D_H__DEDF91FA_FE2B_4611_B901_6D3EC1C58C54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldObject3d.h"
#include <float.h>
#include <math.h>

typedef enum { LDLIGHT_POINT, LDLIGHT_SPOT, LDLIGHT_DIRECTIONAL } ldLightType;

#define LDLIGHT_RANGE_MAX	sqrt(FLT_MAX)

class ldLight3D : public ldObject3D 
{
public:
	ldLight3D();
	virtual ~ldLight3D();

	inline	bool	is_enable() { return _enable; }
	inline	void	set_enable(bool b) { _enable=b; }
	
	inline	void	set_light_type(ldLightType t) { _type=t; }

	inline	void	set_diffuse_color(float r,float g,float b,float a) { _diffuse.x=r; _diffuse.y=g; _diffuse.z=b; _diffuse.w=a; }
	inline	void	set_diffuse_color(Vec4f &color) { _diffuse=color; }
	inline	void	set_specular_color(float r,float g,float b,float a) { _specular.x=r; _specular.y=g; _specular.z=b; _specular.w=a; }
	inline	void	set_specular_color(Vec4f &color) { _specular=color; }
	inline	void	set_ambient_color(float r,float g,float b,float a) { _ambient.x=r; _ambient.y=g; _ambient.z=b; _ambient.w=a; }
	inline	void	set_ambient_color(Vec4f &color) { _ambient=color; }
	inline	void	set_range(float val) { _range=val; }
	inline	void	set_falloff(float val) { _falloff=val; }
	inline	void	set_attenuation(float a1,float a2,float a3) { _attenuation.x=a1; _attenuation.y=a2; _attenuation.z=a3; }
	inline	void	set_attenuation(Vec3f &att) { _attenuation=att; }
	inline	void	set_cone_angles(float inner,float outer) { _cone.x=inner; _cone.y=outer; }

protected:
	bool		_enable;
	ldLightType	_type;
	Vec4f		_diffuse;
	Vec4f		_specular;
	Vec4f		_ambient;
	float		_range;
	float		_falloff;
	Vec3f		_attenuation;
	Vec2f		_cone;
};

#endif // !defined(AFX_LDLIGHT3D_H__DEDF91FA_FE2B_4611_B901_6D3EC1C58C54__INCLUDED_)
