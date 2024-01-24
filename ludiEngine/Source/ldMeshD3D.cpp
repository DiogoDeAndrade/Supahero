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
// ldMeshD3D.cpp: implementation of the ldMeshD3D class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldMeshD3D.h"
#include "..\HEADERS\ldGfxD3D.h"
#include "..\HEADERS\ldUtils.h"
#include "..\Headers\ldLogger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldMeshD3D::ldMeshD3D()
{
	vb=NULL;
	ib=NULL;
}

ldMeshD3D::~ldMeshD3D()
{
	destroy();
}

void ldMeshD3D::destroy()
{
	ldMesh::destroy();

	if (vb) vb->Release(); vb=NULL;
	if (ib) ib->Release(); ib=NULL;
}

void ldMeshD3D::close()
{
	unsigned long counter;

	// Create the vertex buffer
	vb=create_vertex_buffer(vertex_buffer.size(),components,dynamic);
	if (!vb) return;
	// Create the index buffer (if needed)
	if (index_buffer.size()>0)
	{
		ib=create_index_buffer(index_buffer.size());
		if (!ib)
		{
			vb->Release(); vb=NULL;
			return;
		}
		// Do the packing of the index buffer
		pack(ib,&(index_buffer));
		counter=index_buffer.size();
	}
	else
	{
		counter=vertex_buffer.size();
	}
	// Do the packing of the vertex buffer
	pack(components,vb,&(vertex_buffer));
	// Compute number of primives
	switch (primitive)
	{
		case LDPT_POINT_LIST:
			n_primitives=counter;
			break;
		case LDPT_LINE_LIST:
			n_primitives=counter/2;
			break;
		case LDPT_LINE_STRIP:
			n_primitives=counter-1;
			break;
		case LDPT_TRIANGLE_LIST:
			n_primitives=counter/3;
			break;
		case LDPT_TRIANGLE_STRIP:
			n_primitives=counter-2;
			break;
		case LDPT_TRIANGLE_FAN:
			n_primitives=counter-2;
			break;
	}

	// Cleanup
	if ((!dynamic) && (!collidable)) 
	{
		index_buffer.clear();
		vertex_buffer.clear();
	}
}

void ldMeshD3D::draw()
{
	if (!vb) 
	{
		log("Vertex buffer not created (is the mesh closed?)!");
		return;
	}
	
	if (dynamic)
	{
		// Need to update the data
		pack(components,vb,&(vertex_buffer));
		if (indexed) pack(ib,&(index_buffer));
	}

	HRESULT hRet;

	hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->BeginScene();
	if (hRet!=D3D_OK)
	{
		log("Couldn't begin scene!");
		return;
	}

	hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->SetStreamSource(0,vb,vertex_size);
	if (hRet!=D3D_OK)
	{
		log("Couldn't set the stream source!");
		return;
	}

	hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->SetVertexShader(fvf);
	if (hRet!=D3D_OK)
	{
		log("Couldn't set the vertex shader!");
		return;
	}

	if (components & (LDVF_DIFFUSE_COLOR | LDVF_SPECULAR_COLOR))
	{
		((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_COLORVERTEX,TRUE);
		if (components & LDVF_DIFFUSE_COLOR)
		{
			((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_COLORVERTEX,TRUE);
			((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE,D3DMCS_COLOR1);
			((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_COLOR1);
			((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE,D3DMCS_COLOR1);
		}
		if (components & LDVF_SPECULAR_COLOR)
		{
			((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE,D3DMCS_COLOR2);
		}
	}
	else
	{
		((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_COLORVERTEX,FALSE);
	}

	if (!indexed)
	{
		hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->DrawPrimitive(convert_primitive(primitive),0,n_primitives);
		if (hRet!=D3D_OK)
		{
			log("Couldn't draw primitive!");
			return;
		}
	}
	else
	{
		hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->SetIndices(ib,0);
		hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->DrawIndexedPrimitive(convert_primitive(primitive),
																   0,n_vert,0,n_primitives);
		if (hRet!=D3D_OK)
		{
			log("Couldn't draw indexed primitive!");
			return;
		}
		hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->SetIndices(NULL,0);
	}

	hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->SetStreamSource(0,NULL,0);

	hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->EndScene();
}

LPDIRECT3DVERTEXBUFFER8 ldMeshD3D::create_vertex_buffer(unsigned long n_vertex,unsigned long components,bool dynamic)
{
	LPDIRECT3DVERTEXBUFFER8		buffer;
	
	fvf=0;
	vertex_size=0;
	if (components & LDVF_XYZ) { fvf|=D3DFVF_XYZ; vertex_size+=sizeof(float)*3; }
	if (components & LDVF_NORMALS) { fvf|=D3DFVF_NORMAL; vertex_size+=sizeof(float)*3; }
	if (components & LDVF_DIFFUSE_COLOR) { fvf|=D3DFVF_DIFFUSE; vertex_size+=sizeof(DWORD); }
	if (components & LDVF_SPECULAR_COLOR) { fvf|=D3DFVF_SPECULAR; vertex_size+=sizeof(DWORD); }

	switch (LDVF_TEX_COUNT(components))
	{
		case 1: fvf|=D3DFVF_TEX1; break;
		case 2: fvf|=D3DFVF_TEX2; break;
		case 3: fvf|=D3DFVF_TEX3; break;
		case 4: fvf|=D3DFVF_TEX4; break;
		case 5: fvf|=D3DFVF_TEX5; break;
		case 6: fvf|=D3DFVF_TEX6; break;
		case 7: fvf|=D3DFVF_TEX7; break;
		case 8: fvf|=D3DFVF_TEX8; break;
	}
	for (int i=0; i<LDVF_TEX_COUNT(components); i++) { fvf|=D3DFVF_TEXCOORDSIZE2(i); vertex_size+=sizeof(float)*2; }

	if (!dynamic)
	{
		HRESULT hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->CreateVertexBuffer(vertex_size*n_vertex,D3DUSAGE_WRITEONLY,fvf,D3DPOOL_DEFAULT,&buffer);

		if (hRet!=D3D_OK)
		{
			log("Couldn't create static vertex buffer!");
			return NULL;
		}
		n_vert=n_vertex;
		return buffer;
	}
	else
	{
		HRESULT hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->CreateVertexBuffer(vertex_size*n_vertex,D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,fvf,D3DPOOL_DEFAULT,&buffer);

		if (hRet!=D3D_OK)
		{
			log("Couldn't create dynamic vertex buffer!");
			return NULL;
		}
		n_vert=n_vertex;
		return buffer;
	}

	return NULL;
}

void ldMeshD3D::pack(unsigned long components,LPDIRECT3DVERTEXBUFFER8 vb,VertexBuffer *mem)
{
	void	*ptr=lock_vertex_buffer(vb);
	float	*f_ptr;
	DWORD	*dw_ptr;

	if (!ptr) return;

	for (unsigned long i=0; i<mem->size(); i++)
	{
		ldVertex tmp=mem->at(i);

		// Pack pos
		if (components & LDVF_XYZ)
		{
			f_ptr=(float*)ptr;
			*f_ptr=mem->at(i).pos.x; f_ptr++;
			*f_ptr=mem->at(i).pos.y; f_ptr++;
			*f_ptr=mem->at(i).pos.z; f_ptr++;
			ptr=(void*)f_ptr;
		}
		// Pack normals
		if (components & LDVF_NORMALS)
		{
			f_ptr=(float*)ptr;
			*f_ptr=mem->at(i).normal.x; f_ptr++;
			*f_ptr=mem->at(i).normal.y; f_ptr++;
			*f_ptr=mem->at(i).normal.z; f_ptr++;
			ptr=(void*)f_ptr;
		}
		// Pack diffuse color
		if (components & LDVF_DIFFUSE_COLOR)
		{
			dw_ptr=(DWORD*)ptr;
			DWORD color=float_to_colorref(&(mem->at(i).diffuse));
			*dw_ptr=color; dw_ptr++;
			ptr=(void*)dw_ptr;
		}
		// Pack specular color
		if (components & LDVF_SPECULAR_COLOR)
		{
			dw_ptr=(DWORD*)ptr;
			DWORD color=float_to_colorref(&(mem->at(i).specular));
			*dw_ptr=color; dw_ptr++;
			ptr=(void*)dw_ptr;
		}
		// Pack texture coordinates
		f_ptr=(float*)ptr;
		for (int j=0; j<LDVF_TEX_COUNT(components); j++)
		{
			*f_ptr=mem->at(i).uv[j].x; f_ptr++;
			*f_ptr=mem->at(i).uv[j].y; f_ptr++;
		}
		ptr=(void*)f_ptr;
	}

	unlock_vertex_buffer(vb);
}

void *ldMeshD3D::lock_vertex_buffer(LPDIRECT3DVERTEXBUFFER8 buffer)
{
	HRESULT hRet;
	void	*tmp;

	hRet=buffer->Lock(0,0,(unsigned char**)&tmp,D3DLOCK_NOSYSLOCK);
	if (hRet!=D3D_OK)
	{
		if (hRet==D3DERR_INVALIDCALL) log("Can't lock vertex buffer - Invalid call!");
		tmp=NULL;
	}

	return tmp;
}

void ldMeshD3D::unlock_vertex_buffer(LPDIRECT3DVERTEXBUFFER8 buffer)
{
	HRESULT hRet;

	hRet=buffer->Unlock();
	if (hRet!=D3D_OK)
	{
		if (hRet==D3DERR_INVALIDCALL) log("Can't unlock vertex buffer - Invalid call!");
	}
}

D3DPRIMITIVETYPE ldMeshD3D::convert_primitive(ldPrimitiveType type)
{
	switch (type)
	{
		case LDPT_POINT_LIST:
			return D3DPT_POINTLIST;
			break;
		case LDPT_LINE_LIST:
			return D3DPT_LINELIST;
			break;
		case LDPT_LINE_STRIP:
			return D3DPT_LINESTRIP;
			break;
		case LDPT_TRIANGLE_LIST:
			return D3DPT_TRIANGLELIST;
			break;
		case LDPT_TRIANGLE_STRIP:
			return D3DPT_TRIANGLESTRIP;
			break;
		case LDPT_TRIANGLE_FAN:
			return D3DPT_TRIANGLEFAN;
			break;
	}

	return D3DPT_POINTLIST;
}

LPDIRECT3DINDEXBUFFER8 ldMeshD3D::create_index_buffer(unsigned long size)
{
	HRESULT					hRet;
	LPDIRECT3DINDEXBUFFER8	indexes;

	hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->CreateIndexBuffer(size*sizeof(WORD),
															  D3DUSAGE_WRITEONLY,
															  D3DFMT_INDEX16,
															  D3DPOOL_DEFAULT,
															  &indexes);
	if (hRet!=D3D_OK)
	{
		switch (hRet)
		{
			case D3DERR_OUTOFVIDEOMEMORY: 
				log("Can't create index buffer - Create failed!");
				break;
			case D3DERR_INVALIDCALL: 
				log("Can't create index buffer - Invalid call!");
				break;
			case E_OUTOFMEMORY: 
				log("Can't create index buffer - Out of memory!");
				break;
		}
		return NULL;
	}

	return indexes;
}

void ldMeshD3D::pack(LPDIRECT3DINDEXBUFFER8 ib,IndexBuffer *mem)
{
	// Fill index buffer with data
	WORD	*lock;
	HRESULT hRet;

	hRet=ib->Lock(0,sizeof(WORD)*mem->size(),(BYTE**)&lock,0);
	if (hRet!=D3D_OK)
	{
		log("Can't lock index buffer!");
		return;
	}

	for (unsigned long j=0; j<mem->size(); j++)
	{
		*lock=mem->at(j);
		lock++;
	}

	ib->Unlock();
}