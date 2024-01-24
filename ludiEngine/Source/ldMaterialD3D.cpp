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
// ldMaterialD3D.cpp: implementation of the ldMaterialD3D class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldMaterialD3D.h"
#include "..\HEADERS\ldGfxD3D.h"
#include "..\HEADERS\ldTextureD3D.h"
#include <d3d8.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldMaterialD3D::ldMaterialD3D()
{
}

ldMaterialD3D::~ldMaterialD3D()
{

}

void ldMaterialD3D::set()
{
	D3DMATERIAL8	prop;

	// Prepare properties
	prop.Diffuse.r=_diffuse.x; prop.Diffuse.g=_diffuse.y; prop.Diffuse.b=_diffuse.z; prop.Diffuse.a=_diffuse.w; 
	prop.Specular.r=_specular.x; prop.Specular.g=_specular.y; prop.Specular.b=_specular.z; prop.Specular.a=_specular.w; 
	prop.Ambient.r=_ambient.x; prop.Ambient.g=_ambient.y; prop.Ambient.b=_ambient.z; prop.Ambient.a=_ambient.w; 
	prop.Emissive.r=_emissive.x; prop.Emissive.g=_emissive.y; prop.Emissive.b=_emissive.z; prop.Emissive.a=_emissive.w; 
	prop.Power=_power;
	// Set the material
	((ldGfxD3D*)GFX)->_g_pd3dDevice->SetMaterial(&prop);

	// Set the textures
	for (int i=0; i<LDTEXTURE_MAX_NUMBER; i++)
	{
		if (_textures[i])
		{
			((ldTextureD3D*)_textures[i])->set(i);
		}
		else
		{
			((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTexture(i,NULL);
		}
	}
	// Set the blend stages
	for (i=0; i<_blends_no; i++)
	{
		HRESULT	hRet;

		hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(i,D3DTSS_COLOROP,ld_to_dx(_blends[i].color_op));
		hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(i,D3DTSS_COLORARG1,ld_to_dx(_blends[i].color_arg1));	
		hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(i,D3DTSS_COLORARG2,ld_to_dx(_blends[i].color_arg2));	
		hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(i,D3DTSS_ALPHAOP,ld_to_dx(_blends[i].alpha_op));
		hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(i,D3DTSS_ALPHAARG1,ld_to_dx(_blends[i].alpha_arg1));	
		hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(i,D3DTSS_ALPHAARG2,ld_to_dx(_blends[i].alpha_arg2));	
	}
	for (i=_blends_no; i<LDTEXTURE_MAX_NUMBER; i++)
	{
		((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(i,D3DTSS_COLOROP,D3DTOP_DISABLE);
		((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(i,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
	}
}

D3DTEXTUREOP ldMaterialD3D::ld_to_dx(ldBlendOp op)
{
	switch (op)
	{
		case LDBLEND_DISABLE: return D3DTOP_DISABLE; break;
		case LDBLEND_ARG1: return D3DTOP_SELECTARG1; break;
		case LDBLEND_ARG2: return D3DTOP_SELECTARG2; break;
		case LDBLEND_MODULATE: return D3DTOP_MODULATE; break;
		case LDBLEND_MODULATE2X: return D3DTOP_MODULATE2X; break;
		case LDBLEND_MODULATE4X: return D3DTOP_MODULATE4X; break;
		case LDBLEND_ADD: return D3DTOP_ADD; break;
		case LDBLEND_ADDSIGNED: return D3DTOP_ADDSIGNED; break;
		case LDBLEND_ADDSIGNED2X: return D3DTOP_ADDSIGNED2X; break;
	}

	return D3DTOP_DISABLE;
}

unsigned long ldMaterialD3D::ld_to_dx(ldBlendArgument arg)
{
	switch (arg)
	{
		case LDBLENDARG_CURRENT: return D3DTA_CURRENT; break;
		case LDBLENDARG_DIFFUSE: return D3DTA_DIFFUSE; break;
		case LDBLENDARG_SPECULAR: return D3DTA_SPECULAR; break;
		case LDBLENDARG_TEXTURE: return D3DTA_TEXTURE; break;
	}

	return D3DTA_CURRENT;
}
