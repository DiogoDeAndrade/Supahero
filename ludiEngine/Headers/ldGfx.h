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
// ldGfx.h: interface for the ldGfx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDGFX_H__DE5DAB4A_90B8_46D0_B878_A2443BD714BE__INCLUDED_)
#define AFX_LDGFX_H__DE5DAB4A_90B8_46D0_B878_A2443BD714BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN32
#include <windows.h>
#endif

#include "ldGeometry.h"
#include "ldQuaternion.h"

// Clear flags 
#define LD_FRAME_BUFFER		1
#define LD_Z_BUFFER			2
#define LD_STENCIL_BUFFER	4

class ldMesh;
class ldLight3D;
class ldMaterial;
class ldTexture;
class ldFont;
class ldParticleSystem;

#include <list>
typedef std::list<ldLight3D*> ldLight3DList;

typedef enum { LDBLENDMODE_NONE, LDBLENDMODE_NORMAL, LDBLENDMODE_ADD } LDBlendMode;
typedef enum { LDRENDERMODE_POINT, LDRENDERMODE_WIREFRAME, LDRENDERMODE_SOLID } LDRenderMode;

#include <vector>

class ldGfx  
{
public:
	ldGfx();
	virtual ~ldGfx();

public:
	// Initialize graphic system... Besides the normal parameters, it also receives platform
	// specific params
	virtual	bool	init(unsigned long res_x,unsigned long res_y,unsigned long bit_depth,
						 bool windowed,char *window_name,
						 void *platform_param1=NULL,
						 void *platform_param2=NULL,
						 void *platform_param3=NULL);

	// Shutdown system
	virtual void	shutdown() = 0;

	// Rendering stuff
	virtual void	begin_render() = 0;
	virtual void	end_render() = 0;

	// Clear frame, z and stencil buffers (according to flags passed... See the clear flags above)
	virtual void	clear(unsigned int clear_buffers,float r=0.0f,float g=0.0f,float b=0.0f,float a=1.0f,float zVal=1.0f,unsigned long sVal=0) = 0;

	// Exchanges back buffer for front buffer
	virtual void	swap_buffers() = 0;

	// Creation/destruction of meshes
	virtual	ldMesh	*new_mesh() = 0;

	// Creation/destruction of lights
	virtual	ldLight3D	*new_light() = 0;
	virtual void		add_light(ldLight3D *light);
	virtual void		remove_light(ldLight3D *light);

	// Creation/destruction of material
	virtual	ldMaterial	*new_material() = 0;

	// Creation/destruction of textures
	virtual	ldTexture	*new_texture() = 0;

	// Creation/destruction of textures
	virtual	ldFont		*new_font() = 0;

	// Creation/destruction of textures
	virtual	ldParticleSystem		*new_particle_system() = 0;

	// Perspectives, cameras, etc
	virtual void	set_perspective(float fov,float near_plane,float far_plane) = 0;
	virtual void	set_camera(Vec3f *pos,Quaternion *ori) = 0; 
	virtual void	set_world_transformation(int world_trans_id,Vec3f *pos,Quaternion *ori) = 0;

	// 2d stuff
	virtual void	draw_sprite(Vec2f *pos,float size_x,float size_y,Vec4f *color,float u1,float v1,float u2,float v2,ldTexture *texture) = 0;

	// Renderstates, etc
	virtual void	set_lighting(bool b) = 0;
	virtual void	set_ambient_light(float r,float g,float b,float a) = 0;
	virtual void	set_blend_mode(LDBlendMode mode) = 0;
	virtual void	set_render_mode(LDRenderMode mode) = 0;

	inline	LDBlendMode	get_blend_mode() { return _blend_mode; }

protected:
	char*				_windowName;
	unsigned long		_resX,_resY;
	unsigned long		_bitDepth;
	bool				_windowMode;
	ldLight3DList		_lights;
	LDBlendMode			_blend_mode;
};

extern	ldGfx	*GFX;

#endif // !defined(AFX_LDGFX_H__DE5DAB4A_90B8_46D0_B878_A2443BD714BE__INCLUDED_)
