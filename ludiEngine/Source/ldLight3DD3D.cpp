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
// ldLight3DD3D.cpp: implementation of the ldLight3DD3D class.
//
//////////////////////////////////////////////////////////////////////

#include "../Headers/ldLight3DD3D.h"
#include "../Headers/ldGfxD3D.h"
#include <d3d8.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldLight3DD3D::ldLight3DD3D()
{

}

ldLight3DD3D::~ldLight3DD3D()
{

}

void ldLight3DD3D::set(unsigned long index)
{
	D3DLIGHT8	prop;
	// Prepare the light properties
	switch (_type)
	{
		case LDLIGHT_POINT: prop.Type=D3DLIGHT_POINT; break;
		case LDLIGHT_DIRECTIONAL: prop.Type=D3DLIGHT_DIRECTIONAL; break;
		case LDLIGHT_SPOT: prop.Type=D3DLIGHT_SPOT; break;
		default: return; break;
	}
	prop.Position.x=pos.x; prop.Position.y=pos.y; prop.Position.z=pos.z;
	Vec3f source(0.0f,0.0f,-1.0f);
	Vec3f vec;
	ori.w=-ori.w;
	ori.rotate_point(vec,source);
	ori.w=-ori.w;
	prop.Direction.x=vec.x; prop.Direction.y=vec.y; prop.Direction.z=vec.z;
	prop.Diffuse.r=_diffuse.x; prop.Diffuse.g=_diffuse.y; prop.Diffuse.b=_diffuse.z; prop.Diffuse.a=_diffuse.w; 
	prop.Specular.r=_specular.x; prop.Specular.g=_specular.y; prop.Specular.b=_specular.z; prop.Specular.a=_specular.w; 
	prop.Ambient.r=_ambient.x; prop.Ambient.g=_ambient.y; prop.Ambient.b=_ambient.z; prop.Ambient.a=_ambient.w; 
	prop.Range=_range;
	prop.Falloff=_falloff;
	prop.Attenuation0=_attenuation.x;
	prop.Attenuation1=_attenuation.y;
	prop.Attenuation2=_attenuation.z;
	prop.Theta=_cone.x;
	prop.Phi=_cone.y;

	// Add the light
	((ldGfxD3D*)GFX)->_g_pd3dDevice->SetLight(index,&prop);
	((ldGfxD3D*)GFX)->_g_pd3dDevice->LightEnable(index,TRUE);
}
