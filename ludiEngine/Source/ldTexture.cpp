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
// ldTexture.cpp: implementation of the ldTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldTexture.h"

#include <vector>
typedef std::vector<ldTexture*>	ldTextures;

ldTextures	textures;

ldTexture *get_texture(char *name)
{
	for (unsigned long i=0; i<textures.size(); i++)
	{
		if (textures[i])
		{
			if (!stricmp(name,textures[i]->get_name()))
			{
				return textures[i];
			}
		}
	}

	return NULL;
}

void add_texture(ldTexture *texture)
{
	for (unsigned long i=0; i<textures.size(); i++)
	{
		if (!textures[i])
		{
			textures[i]=texture;
			return;
		}
	}
	textures.push_back(texture);
}

void delete_texture(ldTexture *texture)
{
	for (unsigned long i=0; i<textures.size(); i++)
	{
		if (textures[i]==texture)
		{
			textures[i]=NULL;
		}
	}
}

void delete_all_textures()
{
	for (unsigned long i=0; i<textures.size(); i++)
	{
		if (textures[i]) delete textures[i];
	}
	textures.clear();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldTexture::ldTexture()
{
	_sx=_sy=0;
	_wrap_u=_wrap_v=false;
	_name[0]=0;
	add_texture(this);
}

ldTexture::~ldTexture()
{
	delete_texture(this);
}
