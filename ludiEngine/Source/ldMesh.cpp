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
// ldMesh.cpp: implementation of the ldMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldMesh.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldMesh::ldMesh()
{
	components=0;
	dynamic=false;
	indexed=false;
	primitive=LDPT_TRIANGLE_LIST;
}

ldMesh::~ldMesh()
{
	destroy();
}

void ldMesh::create(ldPrimitiveType prim,bool dyn,bool collide)
{
	dynamic=dyn;
	collidable=collide;
	primitive=prim;
}

void ldMesh::destroy()
{
	vertex_buffer.clear();
	index_buffer.clear();
}

long ldMesh::begin_vertex()
{
	ldVertex	tmp;

	vertex_buffer.push_back(tmp);

	current_vertex=&(vertex_buffer[vertex_buffer.size()-1]);

	return vertex_buffer.size()-1;
}

void ldMesh::load_pos(Vec3f *pos)
{
	components|=LDVF_XYZ;

	current_vertex->pos=*pos;
}

void ldMesh::load_pos(float x,float y,float z)
{
	components|=LDVF_XYZ;

	current_vertex->pos.x=x;
	current_vertex->pos.y=y;
	current_vertex->pos.z=z;
}

void ldMesh::load_normal(Vec3f *normal)
{
	components|=LDVF_NORMALS;

	current_vertex->normal=*normal;
}

void ldMesh::load_normal(float nx,float ny,float nz)
{
	components|=LDVF_NORMALS;

	current_vertex->normal.x=nx;
	current_vertex->normal.y=ny;
	current_vertex->normal.z=nz;
}

void ldMesh::load_diffuse_color(float r,float g,float b,float a)
{
	components|=LDVF_DIFFUSE_COLOR;

	current_vertex->diffuse.x=r;
	current_vertex->diffuse.y=g;
	current_vertex->diffuse.z=b;
	current_vertex->diffuse.w=a;
}

void ldMesh::load_specular_color(float r,float g,float b,float a)
{
	components|=LDVF_SPECULAR_COLOR;

	current_vertex->specular.x=r;
	current_vertex->specular.y=g;
	current_vertex->specular.z=b;
	current_vertex->specular.w=a;
}

void ldMesh::load_tex_coord(int i,float u,float v)
{
	components|=LDVF_TEX(i+1);

	current_vertex->uv[i].x=u;
	current_vertex->uv[i].y=v;
}

long ldMesh::end_vertex()
{
	return vertex_buffer.size()-1;
}

void ldMesh::goto_vertex(unsigned long index)
{
	current_vertex=&(vertex_buffer[index]);
}

void ldMesh::get_pos(unsigned long index,float *x,float *y,float *z)
{
	*x=vertex_buffer[index].pos.x;
	*y=vertex_buffer[index].pos.y;
	*z=vertex_buffer[index].pos.z;
}

void ldMesh::get_normal(unsigned long index,float *nx,float *ny,float *nz)
{
	*nx=vertex_buffer[index].normal.x;
	*ny=vertex_buffer[index].normal.y;
	*nz=vertex_buffer[index].normal.z;
}

void ldMesh::get_diffuse_color(unsigned long index,float *r,float *g,float *b,float *a)
{
	*r=vertex_buffer[index].diffuse.x;
	*g=vertex_buffer[index].diffuse.y;
	*b=vertex_buffer[index].diffuse.z;
	*a=vertex_buffer[index].diffuse.w;
}

void ldMesh::get_specular_color(unsigned long index,float *r,float *g,float *b,float *a)
{
	*r=vertex_buffer[index].specular.x;
	*g=vertex_buffer[index].specular.y;
	*b=vertex_buffer[index].specular.z;
	*a=vertex_buffer[index].specular.w;
}

void ldMesh::get_tex_coord(unsigned long index,int i,float *u,float *v)
{
	*u=vertex_buffer[index].uv[i].x;
	*v=vertex_buffer[index].uv[i].y;
}

void ldMesh::load_index(unsigned long index)
{
	index_buffer.push_back(index);
	indexed=true;
}

void ldMesh::clear_vertex_list()
{
	vertex_buffer.clear();
}

void ldMesh::get_vertex(unsigned long index,ldVertex *vertex)
{
	memcpy(vertex,&(vertex_buffer[index]),sizeof(ldVertex));
}

void ldMesh::get_vertex(unsigned long index,ldVertex **vertex)
{
	*vertex=&(vertex_buffer[index]);
}

void ldMesh::load(VertexBuffer *vb)
{
	vertex_buffer.clear();

	for (unsigned long i=0; i<vb->size(); i++)
	{
		load_vertex(&(vb->at(i)));
	}
}

void ldMesh::load_vertex(ldVertex *vertex)
{
	ldVertex tmp;

	memcpy(&tmp,vertex,sizeof(ldVertex));

	vertex_buffer.push_back(tmp);
}

void ldMesh::load(IndexBuffer *ib)
{
	index_buffer.clear();

	for (unsigned long i=0; i<ib->size(); i++)
	{
		index_buffer.push_back(ib->at(i));
	}
	indexed=true;
}

ldBoundingSphere *ldMesh::get_new_bounding_sphere()
{
	// Get the center of the sphere
	Vec3f	center(0.0f,0.0f,0.0f);

	for (unsigned long i=0; i<vertex_buffer.size(); i++)
	{
		center=center+vertex_buffer[i].pos;
	}
	center.x=center.x/vertex_buffer.size();
	center.y=center.y/vertex_buffer.size();
	center.z=center.z/vertex_buffer.size();

	// Get radius to encompass all points
	float r=0.0f;
	for (i=0; i<vertex_buffer.size(); i++)
	{
		Vec3f tmp=center-vertex_buffer[i].pos;
		float xpto=tmp.norm_squared();
		
		if (xpto>r) r=xpto;
	
	}

	if (r>0.0f) r=sqrt(r); else r=0.0f;

	return new ldBoundingSphere(&center,r);
}

bool ldMesh::is_colliding(Vec3f *mesh_pos,Quaternion *mesh_ori,
						  ldBoundingSphere *sphere,unsigned long *index,float *s,float *t)
{
	Quaternion tmp_ori=(*mesh_ori); tmp_ori.w=-tmp_ori.w;

	for (unsigned long i=0; i<get_triangle_count(); i++)
	{
		ldVertex	*pos1,*pos2,*pos3;

		get_triangle(i,&pos1,&pos2,&pos3);

		// Apply transformations to point
		Vec3f	pt1,pt2,pt3;

		tmp_ori.rotate_point(pt1,pos1->pos); pt1=pt1+(*mesh_pos);
		tmp_ori.rotate_point(pt2,pos2->pos); pt2=pt2+(*mesh_pos);
		tmp_ori.rotate_point(pt3,pos3->pos); pt3=pt3+(*mesh_pos);

		// Check collision
		if (sphere->is_colliding(&pt1,&pt2,&pt3,s,t))
		{
			if (index) *index=i;
			return true;
		}
	}

	return false;
}

unsigned long ldMesh::get_triangle_count()
{
	unsigned long vertex=0;

	if (indexed)
	{
		vertex=index_buffer.size();
	}
	else
	{
		vertex=vertex_buffer.size();
	}

	switch (primitive)
	{
		case LDPT_TRIANGLE_LIST:
			return vertex/3;
			break;
		case LDPT_TRIANGLE_STRIP:
		case LDPT_TRIANGLE_FAN:
			return vertex-2;
			break;
	}

	return 0;
}

void ldMesh::get_triangle(unsigned long index,ldVertex **vert1,ldVertex **vert2,ldVertex **vert3)
{
	unsigned long i1,i2,i3;

	switch (primitive)
	{
		case LDPT_TRIANGLE_LIST:
			i1=(index*3); i2=(index*3)+1; i3=(index*3)+2;
			break;
		case LDPT_TRIANGLE_STRIP:
		case LDPT_TRIANGLE_FAN:
			i1=index; i2=index+1; i3=index+2;
			break;
	}

	if (indexed)
	{
		*vert1=&(vertex_buffer[index_buffer[i1]]);
		*vert2=&(vertex_buffer[index_buffer[i2]]);
		*vert3=&(vertex_buffer[index_buffer[i3]]);
	}
	else
	{
		*vert1=&(vertex_buffer[i1]);
		*vert2=&(vertex_buffer[i2]);
		*vert3=&(vertex_buffer[i3]);
	}
}

bool ldMesh::is_intersected(Vec3f *mesh_pos,Quaternion *mesh_ori,
							Vec3f *ray_origin,Vec3f *ray_dir,
							unsigned long *triangle,Vec3f *intersection_point)
{
	Quaternion tmp_ori=(*mesh_ori); tmp_ori.w=-tmp_ori.w;

	for (unsigned long i=0; i<get_triangle_count(); i++)
	{
		ldVertex	*pos1,*pos2,*pos3;

		get_triangle(i,&pos1,&pos2,&pos3);

		// Apply transformations to point
		Vec3f	pt1,pt2,pt3;

		tmp_ori.rotate_point(pt1,pos1->pos); pt1=pt1+(*mesh_pos);
		tmp_ori.rotate_point(pt2,pos2->pos); pt2=pt2+(*mesh_pos);
		tmp_ori.rotate_point(pt3,pos3->pos); pt3=pt3+(*mesh_pos);

		// Check collision
		if (is_ray_intersecting_tri(ray_origin,ray_dir,&pt1,&pt2,&pt3,intersection_point))
		{
			if (triangle) *triangle=i;
			return true;
		}
	}

	return false;
}

bool ldMesh::is_colliding(Vec3f *mesh_pos1,Quaternion *mesh_ori1,
						  ldMesh *mesh,Vec3f *mesh_pos2,Quaternion *mesh_ori2,
						  unsigned long *index1,
						  unsigned long *index2)
{
	Quaternion tmp_ori1=(*mesh_ori1); tmp_ori1.w=-tmp_ori1.w;
	Quaternion tmp_ori2=(*mesh_ori2); tmp_ori2.w=-tmp_ori2.w;

	for (unsigned long i=0; i<get_triangle_count(); i++)
	{
		ldVertex	*pos11,*pos21,*pos31;

		get_triangle(i,&pos11,&pos21,&pos31);

		// Apply transformations to point
		Vec3f	pt11,pt21,pt31;

		tmp_ori1.rotate_point(pt11,pos11->pos); pt11=pt11+(*mesh_pos1);
		tmp_ori1.rotate_point(pt21,pos21->pos); pt21=pt21+(*mesh_pos1);
		tmp_ori1.rotate_point(pt31,pos31->pos); pt31=pt31+(*mesh_pos1);

		for (unsigned long j=0; j<mesh->get_triangle_count(); j++)
		{
			ldVertex	*pos12,*pos22,*pos32;

			mesh->get_triangle(j,&pos12,&pos22,&pos32);

			// Apply transformations to point
			Vec3f	pt12,pt22,pt32;

			tmp_ori2.rotate_point(pt12,pos12->pos); pt12=pt12+(*mesh_pos2);
			tmp_ori2.rotate_point(pt22,pos22->pos); pt22=pt22+(*mesh_pos2);
			tmp_ori2.rotate_point(pt32,pos32->pos); pt32=pt32+(*mesh_pos2);

			if (are_triangles_intersecting(&pt11,&pt21,&pt31,&pt12,&pt22,&pt32))
			{
				if (*index1) *index1=i;
				if (*index2) *index2=j;
				return true;
			}
		}
	}

	return false;
}

void ldMesh::get_triangle(unsigned long index,unsigned long *index1,unsigned long *index2,unsigned long *index3)
{
	unsigned long i1,i2,i3;

	switch (primitive)
	{
		case LDPT_TRIANGLE_LIST:
			i1=(index*3); i2=(index*3)+1; i3=(index*3)+2;
			break;
		case LDPT_TRIANGLE_STRIP:
		case LDPT_TRIANGLE_FAN:
			i1=index; i2=index+1; i3=index+2;
			break;
	}

	if (indexed)
	{
		*index1=index_buffer[i1];
		*index2=index_buffer[i2];
		*index3=index_buffer[i3];
	}
}

unsigned long ldMesh::get_vertex_count() 
{ 
	return vertex_buffer.size(); 
}

unsigned long ldMesh::get_index_count() 
{ 
	return index_buffer.size(); 
}
