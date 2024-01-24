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
// ldGfxD3D.h: interface for the ldGfxD3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDGFXD3D_H__4FFEB4C0_6DB7_45C0_8A66_052F24B8E1A9__INCLUDED_)
#define AFX_LDGFXD3D_H__4FFEB4C0_6DB7_45C0_8A66_052F24B8E1A9__INCLUDED_

#ifdef _WIN32

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldGfx.h"
#include <d3d8.h>

//-----------------------------------------------------------------------------
// Name: struct D3DModeInfo
// Desc: Structure for holding information about a display mode
//-----------------------------------------------------------------------------
struct LD_D3DModeInfo
{
	DWORD      Width;      // Screen width in this mode
	DWORD      Height;     // Screen height in this mode
	D3DFORMAT  Format;     // Pixel format in this mode
	DWORD      dwBehavior; // Hardware / Software / Mixed vertex processing
	D3DFORMAT  DepthStencilFormat; // Which depth/stencil format to use with this mode
};

//-----------------------------------------------------------------------------
// Name: struct D3DDeviceInfo
// Desc: Structure for holding information about a Direct3D device, including
//       a list of modes compatible with this device
//-----------------------------------------------------------------------------
struct LD_D3DDeviceInfo
{
	// Device data
	D3DDEVTYPE   DeviceType;      // Reference, HAL, etc.
	D3DCAPS8     d3dCaps;         // Capabilities of this device
	const TCHAR* strDesc;         // Name of this device
	BOOL         bCanDoWindowed;  // Whether this device can work in windowed mode

	// Modes for this device
	DWORD			dwNumModes;
	LD_D3DModeInfo	modes[150];

	// Current state
	DWORD				dwCurrentMode;
	BOOL				bWindowed;
	D3DMULTISAMPLE_TYPE MultiSampleType;
};

//-----------------------------------------------------------------------------
// Name: struct D3DAdapterInfo
// Desc: Structure for holding information about an adapter, including a list
//       of devices available on this adapter
//-----------------------------------------------------------------------------
struct LD_D3DAdapterInfo
{
	// Adapter data
	D3DADAPTER_IDENTIFIER8	d3dAdapterIdentifier;
	D3DDISPLAYMODE			d3ddmDesktop;      // Desktop display mode for this adapter

	// Devices for this adapter
	DWORD				dwNumDevices;
	LD_D3DDeviceInfo	devices[5];

	// Current state
	DWORD          dwCurrentDevice;
};

class ldGfxD3D : public ldGfx  
{
public:
	ldGfxD3D();
	virtual ~ldGfxD3D();

public:
	// Initialize graphic system...
	// platform_param1=HINSTANCE
	virtual	bool	init(unsigned long res_x,unsigned long res_y,unsigned long bit_depth,
						 bool windowed,char *window_name,
						 void *platform_param1=NULL,
						 void *platform_param2=NULL,
						 void *platform_param3=NULL);
	virtual void	shutdown();

	virtual void	begin_render();
	virtual void	end_render();

	virtual void	clear(unsigned int clear_buffers,float r=0.0f,float g=0.0f,float b=0.0f,float a=1.0f,float zVal=1.0f,unsigned long sVal=0);
	virtual void	swap_buffers();

	virtual	ldMesh		*new_mesh();
	virtual	ldLight3D	*new_light();
	virtual	ldMaterial	*new_material();
	virtual	ldTexture	*new_texture();
	virtual	ldFont		*new_font();

	virtual	ldParticleSystem		*new_particle_system();

	virtual void	set_perspective(float fov,float near_plane,float far_plane);
	virtual void	set_camera(Vec3f *pos,Quaternion *ori);
	virtual void	set_world_transformation(int world_trans_id,Vec3f *pos,Quaternion *ori);

	virtual void	draw_sprite(Vec2f *pos,float size_x,float size_y,Vec4f *color,float u1,float v1,float u2,float v2,ldTexture *texture);

	virtual void	set_lighting(bool b);
	virtual void	set_ambient_light(float r,float g,float b,float a);
	virtual void	set_blend_mode(LDBlendMode mode);
	virtual void	set_render_mode(LDRenderMode mode);

protected:
			bool	init_window();
			bool	init_direct3d();
			bool	init_device();
			bool	init_3d_environment();
			bool	build_device_list();
			bool	confirm_device(D3DCAPS8* caps,DWORD flags,D3DFORMAT format);
			bool	find_depth_stencil_format(UINT iAdapter,D3DDEVTYPE DeviceType,
											  D3DFORMAT TargetFormat,D3DFORMAT* pDepthStencilFormat);

protected:
	LD_D3DAdapterInfo	_adapters[10];
	DWORD				_adapter;
	DWORD				_num_adapters;
	int					_min_depth_bits;
	int					_min_stencil_bits;

public:
	LPDIRECT3D8			_g_pD3D;
	LPDIRECT3DDEVICE8	_g_pd3dDevice;
	D3DCAPS8			_d3dCaps;
	HWND				_mainWindow;
	HINSTANCE			_hInstance;
};

#endif // _WIN32

#endif // !defined(AFX_LDGFXD3D_H__4FFEB4C0_6DB7_45C0_8A66_052F24B8E1A9__INCLUDED_)
