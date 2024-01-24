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
// ldGeoFilter.cpp: implementation of the ldGeoFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldGeoFilter.h"
#include "..\HEADERS\ldMesh.h"
#include "..\HEADERS\ldUtils.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldGeoFilter::ldGeoFilter()
{
	set_tolerance(0.000001f);
}

ldGeoFilter::~ldGeoFilter()
{
	_acceptance_functions.clear();
}

void ldGeoFilter::scale_along_normals(ldMesh *mesh,float min,float max)
{
	for (unsigned long i=0; i<mesh->get_vertex_count(); i++)
	{
		float		x,y,z;
		float		nx,ny,nz;
		ldVertex	*vertex;

		mesh->get_vertex(i,&vertex);
		if (vertex_is_accepted(vertex))
		{
			mesh->goto_vertex(i);
			mesh->get_pos(i,&x,&y,&z);
			mesh->get_normal(i,&nx,&ny,&nz);
			
			float scale_value=get_random_number(min,max);

			// Normal scale
			x+=nx*scale_value;
			y+=ny*scale_value;
			z+=nz*scale_value;
			mesh->load_pos(x,y,z);
		}
	}
}

void ldGeoFilter::convert_to_indexed(ldMesh *mesh)
{
	VertexBuffer	vertex;
	IndexBuffer		index;
	ldVertex		tmp;
	float			tol_pow2=_tolerance*_tolerance;

	// Initialize
	mesh->get_vertex(0,&tmp); vertex.push_back(tmp);
	mesh->get_vertex(1,&tmp); vertex.push_back(tmp);
	mesh->get_vertex(2,&tmp); vertex.push_back(tmp);
	index.push_back(0); 
	index.push_back(1); 
	index.push_back(2);

	for (unsigned long i=3; i<mesh->get_vertex_count(); i++)
	{
		bool match;

		mesh->get_vertex(i,&tmp);

		if (vertex_is_accepted(&tmp))
		{
			match=false;
			for (unsigned long j=0; j<vertex.size(); j++)
			{
				Vec3f	vec=tmp.pos-vertex[j].pos;
				float	d=vec.norm_squared();

				if (d<tol_pow2)
				{
					// Positions match
					vec=tmp.normal-vertex[j].normal;

					d=vec.norm_squared();

					if (d<tol_pow2)
					{
						// Normals match or doesn't matter (smooth normals)
						Vec4f	vec;

						vec=tmp.diffuse-vertex[j].diffuse;
						d=vec.norm_squared();

						if ((d<tol_pow2) || (!mesh->has(LDVF_DIFFUSE_COLOR)))
						{
							// Diffuse color match
							Vec4f	vec;

							vec=tmp.specular-vertex[j].specular;
							d=vec.norm_squared();

							if ((d<tol_pow2) || (!mesh->has(LDVF_SPECULAR_COLOR)))
							{
								// Specular color match
								Vec2f	vec;

								match=true;
								for (int i=0; i<mesh->get_texture_count(); i++)
								{
									vec=tmp.uv[i]-vertex[j].uv[i];
									d=vec.norm_squared();
									if (d>tol_pow2)
									{
										match=false;
									}
								}
							}
						}
					}
				}
				if (match) break;
			}
			
			if (match)
			{
				// Add an index to this vertex
				index.push_back(j);
			}
			else
			{
				// New vertex, add the index
				vertex.push_back(tmp);
				index.push_back(vertex.size()-1);
			}
		}
		else
		{
			// New vertex, add the index
			vertex.push_back(tmp);
			index.push_back(vertex.size()-1);
		}
	}

	// Clear the old vertexes
	mesh->clear_vertex_list();

	mesh->load(&vertex);
	mesh->load(&index);
	
	vertex.clear();
	index.clear();
}

void ldGeoFilter::scale_texture_coords(ldMesh *mesh,long index,float su,float sv)
{
	for (unsigned long i=0; i<mesh->get_vertex_count(); i++)
	{
		float		u,v;
		ldVertex	*vertex;
		
		mesh->get_vertex(i,&vertex);
		if (vertex_is_accepted(vertex))
		{
			mesh->goto_vertex(i);
			mesh->get_tex_coord(i,index,&u,&v);
			mesh->load_tex_coord(index,u*su,v*sv);
		}
	}
}

bool ldGeoFilter::vertex_is_accepted(ldVertex *vertex)
{
	ldAcceptanceFunctions::iterator	i;

	for (i=_acceptance_functions.begin(); i!=_acceptance_functions.end(); i++)
	{
		if (!((*i)(vertex))) return false;
	}

	return true;
}

void ldGeoFilter::remove_acceptance_function(ldAcceptanceFunction func)
{
	ldAcceptanceFunctions::iterator	i;

	for (i=_acceptance_functions.begin(); i!=_acceptance_functions.end(); ++i)
	{
		if ((*i)==func) 
		{
			i=_acceptance_functions.erase(i);
			return;
		}
	}
}

void ldGeoFilter::move(ldMesh *mesh,Vec3f *pos)
{
	for (unsigned long i=0; i<mesh->get_vertex_count(); i++)
	{
		ldVertex	*vertex;

		mesh->get_vertex(i,&vertex);
		if (vertex_is_accepted(vertex))
		{
			vertex->pos=(vertex->pos)+(*pos);
		}
	}
}

void ldGeoFilter::move(ldMesh *mesh,float x,float y,float z)
{
	Vec3f tmp(x,y,z);

	move(mesh,&tmp);
}

void ldGeoFilter::rotate(ldMesh *mesh,Quaternion *ori)
{
	for (unsigned long i=0; i<mesh->get_vertex_count(); i++)
	{
		ldVertex	*vertex;

		mesh->get_vertex(i,&vertex);
		if (vertex_is_accepted(vertex))
		{
			Vec3f tmp=vertex->pos;
			ori->rotate_point(vertex->pos,tmp);
			tmp=vertex->normal;
			ori->rotate_point(vertex->normal,tmp);
		}
	}
}

#define smooth_normals_add(sna_p,sna_n,sna_v) { smooth_normals_struct tmp; tmp.pos=(sna_p); tmp.normal=(sna_n); tmp.vertex.push_back(sna_v); aux.push_back(tmp); }

void ldGeoFilter::smooth_normals(ldMesh *mesh)
{
	if (mesh->get_index_count()<=0)
	{
		// Mesh is not indexed
		smooth_normals_structVector	aux;
		ldVertex					vert;
		float						tol_pow_2=_tolerance*_tolerance;

		// First three vertex on the aux vector
		mesh->get_vertex(0,&vert);
		smooth_normals_add(vert.pos,vert.normal,0);
		mesh->get_vertex(1,&vert);
		smooth_normals_add(vert.pos,vert.normal,1);
		mesh->get_vertex(2,&vert);
		smooth_normals_add(vert.pos,vert.normal,2);

		// Assemble conectivity information and other things... Prepare the normals for
		// ulterior processing...
		for (unsigned long i=3; i<mesh->get_vertex_count(); i++)
		{
			mesh->get_vertex(i,&vert);

			bool match=false;
			if (vertex_is_accepted(&vert))
			{
				for (unsigned long j=0; j<aux.size(); j++)
				{
					Vec3f vec=vert.pos-aux[j].pos;
					float d=vec.norm_squared();

					if (d<tol_pow_2)
					{
						// Positions match
						aux[j].normal=aux[j].normal+vert.normal;
						aux[j].vertex.push_back(i);
						match=true;
						break;
					}
				}
			}
			if (!match)
			{
				smooth_normals_add(vert.pos,vert.normal,i);
			}
		}

		// Process and change normals
		for (i=0; i<aux.size(); i++)
		{
			aux[i].normal=aux[i].normal/aux[i].vertex.size();
			aux[i].normal.normalize();
			for (unsigned long j=0; j<aux[i].vertex.size(); j++)
			{
				mesh->goto_vertex(aux[i].vertex[j]);
				mesh->load_normal(&(aux[i].normal));
			}
			aux[i].vertex.clear();
		}

		aux.clear();
	}
	else
	{
		// Mesh is indexed, the smoothing is made based on the triangles that share a vertex
		vectorVector	triangleNormals;
		
		for (unsigned long i=0; i<mesh->get_triangle_count(); i++)
		{
			ldVertex	*tmp1,*tmp2,*tmp3;
			Vec3f		vec1,vec2,n;

			mesh->get_triangle(i,&tmp1,&tmp2,&tmp3);
			vec1=(tmp2->pos)-(tmp1->pos);
			vec2=(tmp3->pos)-(tmp1->pos);
			n=cross_product(vec1,vec2);
			n.normalize();

			triangleNormals.push_back(n);
		}

		for (i=0; i<mesh->get_vertex_count(); i++)
		{
			Vec3f			normal(0.0f,0.0f,0.0f);
			unsigned long	sharing=0;

			for (unsigned long j=0; j<mesh->get_triangle_count(); j++)
			{
				unsigned long i1,i2,i3;

				mesh->get_triangle(j,&i1,&i2,&i3);
				if ((i==i1) || (i==i2) || (i==i3))
				{
					// Normal is afected by this triangle
					normal=normal+triangleNormals[j];
					sharing++;
				}
			}
			if (sharing>0)
			{
				normal.normalize();
				mesh->goto_vertex(i);
				mesh->load_normal(&normal);
			}
		}
		triangleNormals.clear();
	}
}

void ldGeoFilter::scale(ldMesh *mesh,float x,float y,float z)
{
	for (unsigned long i=0; i<mesh->get_vertex_count(); i++)
	{
		ldVertex	*vertex;

		mesh->get_vertex(i,&vertex);
		if (vertex_is_accepted(vertex))
		{
			vertex->pos.x=vertex->pos.x*x;
			vertex->pos.y=vertex->pos.y*y;
			vertex->pos.z=vertex->pos.z*z;
		}
	}
}

void ldGeoFilter::rotate(ldMesh *mesh,float x,float y,float z,float ang)
{
	Quaternion	qua;

	qua.rotation_quaternion(x,y,z,ang);

	rotate(mesh,&qua);
}
