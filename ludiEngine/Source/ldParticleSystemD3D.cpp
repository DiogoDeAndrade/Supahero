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
// ldParticleSystemD3D.cpp: implementation of the ldParticleSystemD3D class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldParticleSystemD3D.h"
#include "..\HEADERS\ldGfxD3D.h"
#include "..\HEADERS\ldTextureD3D.h"
#include "..\HEADERS\ldUtils.h"
#include "..\Headers\ldLogger.h"

inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldParticleSystemD3D::ldParticleSystemD3D()
{
	vertex=NULL;
	buffer_size=0;
	vertex=create_vertex_buffer(100);
}

ldParticleSystemD3D::~ldParticleSystemD3D()
{
	if (vertex) vertex->Release();
}

void ldParticleSystemD3D::draw()
{
    HRESULT hRet;

	if (_particles.size()<=0) return;

	Vec3f		ident_p(0.0f,0.0f,0.0f);
	Quaternion	ident_o(0.0f,0.0f,0.0f,1.0f);
	GFX->set_world_transformation(0,&ident_p,&ident_o);
    
	// Set the render states for using point sprites
    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE,TRUE);
    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE,TRUE);
    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_POINTSIZE_MIN,FtoDW(0.00f));
    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_A,FtoDW(0.00f));
    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_B,FtoDW(0.00f));
    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_C,FtoDW(1.00f));

    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);

	((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_COLORVERTEX,TRUE);
	((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE,D3DMCS_COLOR1);
	((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_COLOR1);
	((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE,D3DMCS_COLOR1);
	((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE,D3DMCS_COLOR1);

	((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);

	ldParticles::iterator	i;

    ldPointVertex*	pVertices;

	// Check size of current vertex buffer, if needed, destroy it and create a new bigger one
	if (buffer_size<_particles.size())
	{
		if (vertex) vertex->Release();
		vertex=create_vertex_buffer(_particles.size());
		if (!vertex) return;
	}

	// Lock the vertex buffer
	pVertices=(ldPointVertex*)lock_vertex_buffer(vertex);

	for (i=_particles.begin(); i!=_particles.end(); ++i)
	{
		ldParticle *tmp=*i;
		// Fill vertex buffer with the particles
		pVertices->x=tmp->pos.x;
		pVertices->y=tmp->pos.y;
		pVertices->z=tmp->pos.z;
		pVertices->size=tmp->size;
		pVertices->color=float_to_colorref(tmp->color.x,tmp->color.y,tmp->color.z,tmp->color.w);
		pVertices++;
	}

	// Unlock the vertex buffer
	unlock_vertex_buffer(vertex);

	LDBlendMode tmp=GFX->get_blend_mode();
	GFX->set_blend_mode(_blend_mode);

    // Set up the vertex buffer to be rendered
    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetStreamSource(0,vertex,sizeof(ldPointVertex));
    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetVertexShader(D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE);

	// Set the texture
	((ldTextureD3D*)_texture)->set(0);

	// Render the vertex buffer
	hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->BeginScene();
	hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->DrawPrimitive(D3DPT_POINTLIST,0,_particles.size());
	hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->EndScene();

    // Reset render states
    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE,FALSE);
    ((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE,FALSE);
	((ldGfxD3D*)GFX)->_g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
	
	GFX->set_blend_mode(tmp);
}

LPDIRECT3DVERTEXBUFFER8 ldParticleSystemD3D::create_vertex_buffer(unsigned long n_vertex)
{
	LPDIRECT3DVERTEXBUFFER8		buffer;
	unsigned long				fvf;
	unsigned long				vertex_size;
	
	fvf=D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE;
	vertex_size=sizeof(float)*4+sizeof(DWORD);

	HRESULT hRet=((ldGfxD3D*)GFX)->_g_pd3dDevice->CreateVertexBuffer(vertex_size*n_vertex,D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,fvf,D3DPOOL_DEFAULT,&buffer);

	if (hRet!=D3D_OK)
	{
		log("Couldn't create dynamic vertex buffer!");
		return NULL;
	}
	buffer_size=n_vertex;
	return buffer;
}

void *ldParticleSystemD3D::lock_vertex_buffer(LPDIRECT3DVERTEXBUFFER8 buffer)
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

void ldParticleSystemD3D::unlock_vertex_buffer(LPDIRECT3DVERTEXBUFFER8 buffer)
{
	HRESULT hRet;

	hRet=buffer->Unlock();
	if (hRet!=D3D_OK)
	{
		if (hRet==D3DERR_INVALIDCALL) log("Can't unlock vertex buffer - Invalid call!");
	}
}

