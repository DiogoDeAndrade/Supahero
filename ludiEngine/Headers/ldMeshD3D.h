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
// ldMeshD3D.h: interface for the ldMeshD3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDMESHD3D_H__49AA0126_7F60_4BA4_967A_E563C40ED84A__INCLUDED_)
#define AFX_LDMESHD3D_H__49AA0126_7F60_4BA4_967A_E563C40ED84A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldMesh.h"
#include <d3d8.h>

class ldMeshD3D : public ldMesh  
{
public:
	ldMeshD3D();
	virtual ~ldMeshD3D();

public:
	virtual void	destroy();
	virtual void	close();
	virtual void	draw();

protected:
		LPDIRECT3DVERTEXBUFFER8 create_vertex_buffer(unsigned long n_vertex,
													 unsigned long components,
													 bool dynamic);
		LPDIRECT3DINDEXBUFFER8 create_index_buffer(unsigned long size);
		D3DPRIMITIVETYPE		convert_primitive(ldPrimitiveType type);

		void pack(unsigned long components,LPDIRECT3DVERTEXBUFFER8 vb,VertexBuffer *mem);
		void pack(LPDIRECT3DINDEXBUFFER8 ib,IndexBuffer *mem);
		void *lock_vertex_buffer(LPDIRECT3DVERTEXBUFFER8 buffer);
		void unlock_vertex_buffer(LPDIRECT3DVERTEXBUFFER8 buffer);


protected:
		LPDIRECT3DVERTEXBUFFER8		vb;
		LPDIRECT3DINDEXBUFFER8		ib;
		unsigned long				vertex_size;
		unsigned long				n_primitives;
		unsigned long				n_vert;
		unsigned long				fvf;
};

#endif // !defined(AFX_LDMESHD3D_H__49AA0126_7F60_4BA4_967A_E563C40ED84A__INCLUDED_)
