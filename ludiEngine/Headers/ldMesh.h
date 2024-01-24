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
// ldMesh.h: interface for the ldMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDMESH_H__60C43145_DF27_4239_AC2C_634205B427F3__INCLUDED_)
#define AFX_LDMESH_H__60C43145_DF27_4239_AC2C_634205B427F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldGeometry.h"
#include "ldQuaternion.h"
#include "ldBoundingSphere.h"

// Primitive types
typedef enum { LDPT_POINT_LIST, LDPT_LINE_LIST, LDPT_LINE_STRIP, LDPT_LINE_LOOP, 
			   LDPT_TRIANGLE_LIST, LDPT_TRIANGLE_STRIP, LDPT_TRIANGLE_FAN } ldPrimitiveType;

// Vertex format flags and definitions
#define LDVF_XYZ			1
#define LDVF_NORMALS		2
#define LDVF_DIFFUSE_COLOR	4
#define LDVF_SPECULAR_COLOR	8
#define LDVF_TEX(x)			((x) << 4)
#define LDVF_TEX_COUNT(x)	((x) >> 4)

struct ldVertex 
{
	Vec3f	pos;
	Vec3f	normal;
	Vec4f	diffuse;
	Vec4f	specular;
	Vec2f	uv[8];
};

#include <vector>
typedef std::vector<ldVertex>	VertexBuffer;
typedef std::vector<unsigned long>	IndexBuffer;

class ldMesh  
{
public:
	ldMesh();
	virtual ~ldMesh();

	inline	bool	has(unsigned long c) { if (c & components) return true; else return false; }
	inline	int		get_texture_count() { return LDVF_TEX_COUNT(components); }

	// Creation/destruction
	virtual void	create(ldPrimitiveType prim=LDPT_TRIANGLE_LIST,bool dyn=false,bool collide=false);
	virtual void	destroy();

	// Loading the mesh
			void	clear_vertex_list();
			long	begin_vertex();
			void	load_pos(Vec3f *pos);
			void	load_pos(float x,float y,float z);
			void	load_normal(Vec3f *normal);
			void	load_normal(float nx,float ny,float nz);
			void	load_diffuse_color(float r,float g,float b,float a);
			void	load_specular_color(float r,float g,float b,float a);
			void	load_tex_coord(int i,float u,float v);
			long	end_vertex();
			void	load(VertexBuffer *v);
			void	load_vertex(ldVertex *vertex);

	// Indexes
			void	load_index(unsigned long index);
			void	load(IndexBuffer *i);

	// Dynamic, collidable and before closure methods
	virtual unsigned long	get_vertex_count();
	virtual unsigned long	get_index_count();
			void			goto_vertex(unsigned long index);

			void	get_pos(unsigned long index,float *x,float *y,float *z);
			void	get_normal(unsigned long index,float *nx,float *ny,float *nz);
			void	get_diffuse_color(unsigned long index,float *r,float *g,float *b,float *a);
			void	get_specular_color(unsigned long index,float *r,float *g,float *b,float *a);
			void	get_tex_coord(unsigned long index,int i,float *u,float *v);
			void	get_vertex(unsigned long index,ldVertex *vertex);
			void	get_vertex(unsigned long index,ldVertex **vertex);

	inline	unsigned long	get_index(unsigned long index) { return index_buffer[index]; }

	virtual	unsigned long	get_triangle_count();
			void			get_triangle(unsigned long index,ldVertex **vert1,ldVertex **vert2,ldVertex **vert3);
			void			get_triangle(unsigned long index,unsigned long *index1,unsigned long *index2,unsigned long *index3);

	// Closing the mesh
	virtual void	close() = 0;

	// Drawing the mesh
	virtual void	draw() = 0;

	// Collision detection stuff
	virtual	ldBoundingSphere	*get_new_bounding_sphere();

	virtual bool	is_colliding(Vec3f *mesh_pos,Quaternion *mesh_ori,
								 ldBoundingSphere *sphere,
								 unsigned long *index=NULL,float *s=NULL,float *t=NULL);
	virtual bool	is_colliding(Vec3f *mesh_pos1,Quaternion *mesh_ori1,
								 ldMesh *mesh,Vec3f *mesh_pos2,Quaternion *mesh_ori2,
								 unsigned long *index1=NULL,
								 unsigned long *index2=NULL);
	virtual	bool	is_intersected(Vec3f *mesh_pos,Quaternion *mesh_ori,
								   Vec3f *ray_origin,Vec3f *ray_dir,
								   unsigned long *triangle=NULL,Vec3f *intersection_point=NULL);

protected:
	unsigned long	components;
	ldVertex		*current_vertex;
	VertexBuffer	vertex_buffer;
	IndexBuffer		index_buffer;
	bool			indexed;
	bool			dynamic;
	bool			collidable;
	ldPrimitiveType	primitive;
};

#endif // !defined(AFX_LDMESH_H__60C43145_DF27_4239_AC2C_634205B427F3__INCLUDED_)
	