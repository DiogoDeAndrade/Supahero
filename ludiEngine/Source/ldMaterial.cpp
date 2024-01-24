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
// ldMaterial.cpp: implementation of the ldMaterial class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldMaterial.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldMaterial::ldMaterial()
{
	_diffuse.set(1.0f,1.0f,1.0f,1.0f);
	_ambient.set(1.0f,1.0f,1.0f,1.0f);
	_specular.set(0.0f,0.0f,0.0f,0.0f);
	_power=0.0f;
	_emissive.set(0.0f,0.0f,0.0f,0.0f);

	for (int i=0; i<LDTEXTURE_MAX_NUMBER; i++) 
	{
		_textures[i]=NULL;
		_blends[i].color_op=LDBLEND_DISABLE;
		_blends[i].alpha_op=LDBLEND_DISABLE;
	}
	_blends_no=0;
}

ldMaterial::~ldMaterial()
{

}
