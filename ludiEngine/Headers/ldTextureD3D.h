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
// ldTextureD3D.h: interface for the ldTextureD3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDTEXTURED3D_H__DE5A8DFB_125F_4357_8DD4_531F87688310__INCLUDED_)
#define AFX_LDTEXTURED3D_H__DE5A8DFB_125F_4357_8DD4_531F87688310__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldTexture.h"
#include <d3d8.h>

class ldTextureD3D : public ldTexture  
{
public:
	ldTextureD3D();
	virtual ~ldTextureD3D();

			void	set(unsigned long index);

	virtual bool	load(char *filename);
	virtual void	clear();

protected:
	LPDIRECT3DTEXTURE8	_map;
};

#endif // !defined(AFX_LDTEXTURED3D_H__DE5A8DFB_125F_4357_8DD4_531F87688310__INCLUDED_)
