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
// ldTextureD3D.cpp: implementation of the ldTextureD3D class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldTextureD3D.h"
#include "..\Headers\ldLogger.h"
#include "..\HEADERS\ldGfxD3D.h"
#include "il/ilut.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldTextureD3D::ldTextureD3D()
{
	_map=NULL;
	_sx=NULL;
	_sy=NULL;
}

ldTextureD3D::~ldTextureD3D()
{
	if (_map) _map->Release();
}

void ldTextureD3D::set(unsigned long index)
{
	HRESULT hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTexture(index,_map);

	// Set wrapping mode
	((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(index,D3DTSS_ADDRESSU,(_wrap_u)?(D3DTADDRESS_WRAP):(D3DTADDRESS_CLAMP));
	((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(index,D3DTSS_ADDRESSV,(_wrap_v)?(D3DTADDRESS_WRAP):(D3DTADDRESS_CLAMP));
}

bool ldTextureD3D::load(char *filename)
{
	if (_map) _map->Release();

	FILE *file=fopen(filename,"rb");
	if (!file)
	{
		char buffer[8192];

		GetCurrentDirectory(8192,(char*)&buffer);

		log("Couldn't load texture: File %s not found (current dir=%s)!",filename,buffer);
		return false;
	}

	fclose(file);

	if (ilutD3D8TexFromFile(((ldGfxD3D*)GFX)->_g_pd3dDevice,filename,&_map)) 
	{
		D3DSURFACE_DESC	info;

		_map->GetLevelDesc(0,&info);
		_sx=info.Width;
		_sy=info.Height;

		memcpy((char*)&_name,filename,strlen(filename)+1);

		return true;
	}

	ILenum tmp=ilGetError();
	log("Couldn't load texture: %s",iluErrorString(tmp));

	return false;
}

void ldTextureD3D::clear()
{
	if (_map) _map->Release(); _map=NULL;
}
