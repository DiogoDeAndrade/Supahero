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
// ldParticleSystemD3D.h: interface for the ldParticleSystemD3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDPARTICLESYSTEMD3D_H__D5215DAE_5B1D_464E_A88C_17E4FD34CB02__INCLUDED_)
#define AFX_LDPARTICLESYSTEMD3D_H__D5215DAE_5B1D_464E_A88C_17E4FD34CB02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldParticleSystem.h"
#include <d3d8.h>
#include <d3dx8.h>

class ldParticleSystemD3D : public ldParticleSystem  
{
	struct ldPointVertex
	{
		float		x,y,z;
		float		size;
		D3DCOLOR    color;
	};

public:
	ldParticleSystemD3D();
	virtual ~ldParticleSystemD3D();

	virtual	void	draw();

protected:
	LPDIRECT3DVERTEXBUFFER8 create_vertex_buffer(unsigned long n_vertex);
	void					*lock_vertex_buffer(LPDIRECT3DVERTEXBUFFER8 buffer);
	void					unlock_vertex_buffer(LPDIRECT3DVERTEXBUFFER8 buffer);

protected:
	LPDIRECT3DVERTEXBUFFER8		vertex;
	unsigned long				buffer_size;
};

#endif // !defined(AFX_LDPARTICLESYSTEMD3D_H__D5215DAE_5B1D_464E_A88C_17E4FD34CB02__INCLUDED_)
