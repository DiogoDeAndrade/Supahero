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
// ldGeoFactory.cpp: implementation of the ldGeoFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "../HEADERS/ldGeoFactory.h"
#include "../HEADERS/ldGfx.h"
#include "../HEADERS/ldMesh.h"
#include "../HEADERS/ldTriangulate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldGeoFactory::ldGeoFactory()
{
	_dynamic=_collidable=false;
	_n_tex=0;
	_x1=_y1=_z1=0.0f;
	_x2=_y2=_z2=1.0f;
	_res_x=_res_y=_res_z=1.0f;
}

ldGeoFactory::~ldGeoFactory()
{

}

void ldGeoFactory::set_size(float x1,float y1,float z1,float x2,float y2,float z2)
{
	if (x1<x2) { _x1=x1; _x2=x2; } else { _x1=x2; _x2=x1; }
	if (y1<y2) { _y1=y1; _y2=y2; } else { _y1=y2; _y2=y1; }
	if (z1<z2) { _z1=z1; _z2=z2; } else { _z1=z2; _z2=z1; }
}

ldMesh* ldGeoFactory::new_mesh()
{
	ldMesh *tmp=GFX->new_mesh();

	tmp->create(LDPT_TRIANGLE_LIST,_dynamic,_collidable);

	return tmp;
}

ldMesh*	ldGeoFactory::xz_quad()
{
	long ptX=long(_res_x*(_x2-_x1))+1;
	long ptZ=long(_res_z*(_z2-_z1))+1;

	// Create the mesh
	ldMesh *quad=new_mesh();
	if (!quad) return NULL;
	
	float sx,sz=_z1;
	float ix=(_x2-_x1)/ptX;
	float iz=(_z2-_z1)/ptZ;
	float u,v=0.0f;
	float ui=1.0f/(_x2-_x1)/_res_x;
	float vi=1.0f/(_z2-_z1)/_res_z;
	for (long i=0; i<ptZ-1; i++)
	{
		sx=_x1;
		u=0.0f;
		for (long j=0; j<ptX-1; j++)
		{
			load_quad(quad,
					  sx,0.0f,sz,0.0f,1.0f,0.0f,u,v,
					  sx,0.0f,sz+iz,0.0f,1.0f,0.0f,u,v+vi,
					  sx+ix,0.0f,sz+iz,0.0f,1.0f,0.0f,u+ui,v+vi,
					  sx+ix,0.0f,sz,0.0f,1.0f,0.0f,u+ui,v);
			sx+=ix;
			u+=ui;
		}
		sz+=iz;
		v+=vi;
	}

	return quad;
}

ldMesh*	ldGeoFactory::spheroid(float thetaStart,float thetaEnd,float phiStart,float phiEnd,bool top_cap,bool bottom_cap)
{
	// Create the mesh
	ldMesh *sphere=new_mesh();
	if (!sphere) return NULL;

	// Lets try to do a triangle sphere, instead of an indexed triangle sphere
	long	ptTheta=long(((thetaEnd-thetaStart)/PI)*_res_x*100.0f*(_x2-_x1)); if (ptTheta==0) ptTheta=1;
	long	ptPhi=long(((phiEnd-phiStart)/PI_TIMES_2)*_res_z*150.0f*(_z2-_z1)); if (ptPhi==0) ptPhi=1;

	Vec3f	vec1,vec2,vec3,vec4;
	Vec3f	nvec1,nvec2,nvec3,nvec4;

	// Dimensions
	Vec3f	center,size,tmp;
	
	center.set(_x1,_y1,_z1);
	tmp.set(_x2,_y2,_z2);
	center=center+tmp;
	center=center/2.0f;

	size.set((_x2-_x1)/2.0f,(_y2-_y1)/2.0f,(_z2-_z1)/2.0f);

	// Top cap

	// Get the increments, and all that
	float phi=phiStart;
	float phiInc=(phiEnd-phiStart)/ptPhi;
	float thetaInc=(thetaEnd-thetaStart)/ptTheta;
	float theta=thetaStart+thetaInc;

	float sint=sin(theta);
	float cost=cos(theta);
	float sinti=sin(theta+thetaInc);
	float costi=cos(theta+thetaInc);
	float u=0.0f;
	float uInc=1.0f/ptPhi;
	float v=0.0f;
	float vInc=1.0f/ptTheta;

	if (top_cap)
	{
		// First, the top appex
		if (thetaStart!=0.0f) vec1.set(center.x,center.y+cost*size.y,center.z);
		else vec1.set(center.x,center.y+cos(thetaStart)*size.y,center.z);

		// Now, all the sphere around (to build the top)
		for (unsigned long pi=0; pi<ptPhi; pi++)
		{
			float cosp=cos(phi);
			float sinp=sin(phi);
			float cospi=cos(phi+phiInc);
			float sinpi=sin(phi+phiInc);

			nvec2.set(cosp*sint,cost,sinp*sint);
			vec2.set(nvec2.x*size.x+center.x,nvec2.y*size.y+center.y,nvec2.z*size.z+center.z);
			
			nvec3.set(cospi*sint,cost,sinpi*sint);
			vec3.set(nvec3.x*size.x+center.x,nvec3.y*size.y+center.y,nvec3.z*size.z+center.z);

			load_tri(sphere,
					 vec1.x,vec1.y,vec1.z,0.0f,1.0f,0.0f,0.5f,0.0f,
					 vec3.x,vec3.y,vec3.z,nvec3.x,nvec3.y,nvec3.z,u+uInc,v+vInc,
					 vec2.x,vec2.y,vec2.z,nvec2.x,nvec2.y,nvec2.z,u,v+vInc);
			
			phi+=phiInc;
			u+=uInc;
		}
	}

	v+=vInc;

	// Build sphere body
	for (unsigned long ti=1; ti<ptTheta-1; ti++)
	{
		float sint=sin(theta);
		float cost=cos(theta);
		float sinti=sin(theta+thetaInc);
		float costi=cos(theta+thetaInc);

		nvec1.y=nvec2.y=cost;
		nvec3.y=nvec4.y=costi;
		
		vec1.y=vec2.y=center.y+size.y*nvec1.y;
		vec3.y=vec4.y=center.y+size.y*nvec3.y;

		phi=phiStart;
		u=0.0f;
		for (unsigned long pi=0; pi<ptPhi; pi++)
		{
			float cosp=cos(phi);
			float sinp=sin(phi);
			float cospi=cos(phi+phiInc);
			float sinpi=sin(phi+phiInc);

			nvec1.x=cosp*sint; nvec1.z=sinp*sint;
			nvec2.x=cospi*sint; nvec2.z=sinpi*sint;
			nvec3.x=cospi*sinti; nvec3.z=sinpi*sinti;
			nvec4.x=cosp*sinti; nvec4.z=sinp*sinti;

			vec1.x=nvec1.x*size.x+center.x; vec1.z=nvec1.z*size.z+center.z; 
			vec2.x=nvec2.x*size.x+center.x; vec2.z=nvec2.z*size.z+center.z; 
			vec3.x=nvec3.x*size.x+center.x; vec3.z=nvec3.z*size.z+center.z; 
			vec4.x=nvec4.x*size.x+center.x; vec4.z=nvec4.z*size.z+center.z; 

			load_quad(sphere,
					  vec1.x,vec1.y,vec1.z,nvec1.x,nvec1.y,nvec1.z,u,v,
					  vec2.x,vec2.y,vec2.z,nvec2.x,nvec2.y,nvec2.z,u+uInc,v,
					  vec3.x,vec3.y,vec3.z,nvec3.x,nvec3.y,nvec3.z,u+uInc,v+vInc,
					  vec4.x,vec4.y,vec4.z,nvec4.x,nvec4.y,nvec4.z,u,v+vInc);
			
			phi+=phiInc;
			u+=uInc;
		}

		theta+=thetaInc;
		v+=vInc;
	}

	if (bottom_cap)
	{
		float sint=sin(theta);
		float cost=cos(theta);
		float sinti=sin(theta+thetaInc);
		float costi=cos(theta+thetaInc);

		// First, the bottom appex
		if (thetaEnd!=PI) vec1.set(center.x,center.y+cost*size.y,center.z);
		else vec1.set(center.x,center.y+cos(thetaEnd)*size.y,center.z);

		phi=phiStart;
		u=0.0f;
		// Now, all the sphere around (to build the bottom)
		for (unsigned long pi=0; pi<ptPhi; pi++)
		{
			float cosp=cos(phi);
			float sinp=sin(phi);
			float cospi=cos(phi+phiInc);
			float sinpi=sin(phi+phiInc);

			nvec2.set(cosp*sint,cost,sinp*sint);
			vec2.set(nvec2.x*size.x+center.x,nvec2.y*size.y+center.y,nvec2.z*size.z+center.z);
			
			nvec3.set(cospi*sint,cost,sinpi*sint);
			vec3.set(nvec3.x*size.x+center.x,nvec3.y*size.y+center.y,nvec3.z*size.z+center.z);

			load_tri(sphere,
					 vec1.x,vec1.y,vec1.z,0.0f,-1.0f,0.0f,0.5f,1.0f,
					 vec2.x,vec2.y,vec2.z,nvec2.x,nvec2.y,nvec2.z,u,1.0f,
					 vec3.x,vec3.y,vec3.z,nvec3.x,nvec3.y,nvec3.z,u+uInc,1.0f);
			
			phi+=phiInc;
			u+=uInc;
		}
	}

	return sphere;
}

unsigned long ldGeoFactory::load_quad(ldMesh *mesh,
							 float x1,float y1,float z1,float nx1,float ny1,float nz1,float u1,float v1,
							 float x2,float y2,float z2,float nx2,float ny2,float nz2,float u2,float v2,
							 float x3,float y3,float z3,float nx3,float ny3,float nz3,float u3,float v3,
							 float x4,float y4,float z4,float nx4,float ny4,float nz4,float u4,float v4)
{
	unsigned long tmp;

	tmp=load_tri(mesh,
				 x1,y1,z1,nx1,ny1,nz1,u1,v1,
				 x2,y2,z2,nx2,ny2,nz2,u2,v2,
				 x3,y3,z3,nx3,ny3,nz3,u3,v3);
	load_tri(mesh,
			 x1,y1,z1,nx1,ny1,nz1,u1,v1,
			 x3,y3,z3,nx3,ny3,nz3,u3,v3,
			 x4,y4,z4,nx4,ny4,nz4,u4,v4);

	return tmp;
}

unsigned long ldGeoFactory::load_tri(ldMesh *mesh,
							 float x1,float y1,float z1,float nx1,float ny1,float nz1,float u1,float v1,
							 float x2,float y2,float z2,float nx2,float ny2,float nz2,float u2,float v2,
							 float x3,float y3,float z3,float nx3,float ny3,float nz3,float u3,float v3)
{
	unsigned long tmp;

	mesh->begin_vertex();
	mesh->load_pos(x1,y1,z1);
	mesh->load_normal(nx1,ny1,nz1);
	for (long i=0; i<_n_tex; i++) mesh->load_tex_coord(i,u1,v1);
	tmp=mesh->end_vertex();
	
	mesh->begin_vertex();
	mesh->load_pos(x2,y2,z2);
	mesh->load_normal(nx2,ny2,nz2);
	for (i=0; i<_n_tex; i++) mesh->load_tex_coord(i,u2,v2);
	mesh->end_vertex();

	mesh->begin_vertex();
	mesh->load_pos(x3,y3,z3);
	mesh->load_normal(nx3,ny3,nz3);
	for (i=0; i<_n_tex; i++) mesh->load_tex_coord(i,u3,v3);
	tmp=mesh->end_vertex();

	return tmp;
}

ldMesh*	ldGeoFactory::prism(long n_sides,bool capped)
{
	if (n_sides<3) return NULL;

	// Create the mesh
	ldMesh *prism=new_mesh();
	if (!prism) return NULL;

	Vec3f	center,size,tmp;
	
	center.set(_x1,_y1,_z1);
	tmp.set(_x2,_y2,_z2);
	center=center+tmp;
	center=center/2.0f;

	size.set((_x2-_x1)/2.0f,(_y2-_y1)/2.0f,(_z2-_z1)/2.0f);

	float xAng=PI_OVER_4;
	float xInc=PI_TIMES_2/n_sides;

	Vec3f	n;
	
	for (long i=0; i<n_sides; i++)
	{		
		float c1=cos(xAng);
		float s1=sin(xAng);
		float c2=cos(xAng+xInc);
		float s2=sin(xAng+xInc);
		float u1=1.0f-(xAng-PI_OVER_4)/PI_TIMES_2;
		float u2=1.0f-((xAng-PI_OVER_4)+xInc)/PI_TIMES_2;
		
		Vec3f	vec1(c1*size.x+center.x,_y1,s1*size.z+center.z);
		Vec3f	vec2(c1*size.x+center.x,_y2,s1*size.z+center.z);
		Vec3f	vec3(c2*size.x+center.x,_y2,s2*size.z+center.z);

		vec2=vec2-vec1;
		vec3=vec3-vec1;
		n=cross_product(vec2,vec3);
		n.normalize();

		load_quad(prism,
				  c1*size.x+center.x,_y1,s1*size.z+center.z,n.x,n.y,n.z,u1,1.0f,
				  c1*size.x+center.x,_y2,s1*size.z+center.z,n.x,n.y,n.z,u1,0.0f,
				  c2*size.x+center.x,_y2,s2*size.z+center.z,n.x,n.y,n.z,u2,0.0f,
				  c2*size.x+center.x,_y1,s2*size.z+center.z,n.x,n.y,n.z,u2,1.0f);

		xAng+=xInc;
	}

	if (capped)
	{
		VertexBuffer	buffer;

		// Upper cap
		xAng=PI_OVER_4;

		for (long i=0; i<n_sides; i++)
		{		
			float c1=cos(xAng);
			float s1=sin(xAng);
			float c2=cos(xAng+xInc);
			float s2=sin(xAng+xInc);

			ldVertex	vert;

			vert.pos.x=c1*size.x+center.x;
			vert.pos.y=_y2;
			vert.pos.z=s1*size.z+center.z;
			vert.normal.x=0.0f;
			vert.normal.y=1.0f;
			vert.normal.z=0.0f;
			for (long k=0; k<_n_tex; k++)
				vert.uv[k].set(0.0f,0.0f);

			buffer.push_back(vert);

			xAng+=xInc;
		}

		add_cap(prism,&buffer,false);
		buffer.clear();
		
		// Lower cap
		xAng=PI_OVER_4;

		for (i=0; i<n_sides; i++)
		{		
			float c1=cos(xAng);
			float s1=sin(xAng);
			float c2=cos(xAng+xInc);
			float s2=sin(xAng+xInc);

			ldVertex	vert;

			vert.pos.x=c1*size.x+center.x;
			vert.pos.y=_y1;
			vert.pos.z=s1*size.z+center.z;
			vert.normal.x=0.0f;
			vert.normal.y=1.0f;
			vert.normal.z=0.0f;
			for (long k=0; k<_n_tex; k++)
				vert.uv[k].set(1.0f,1.0f);

			buffer.push_back(vert);

			xAng+=xInc;
		}

		add_cap(prism,&buffer,true);
		buffer.clear();
	}

	return prism;
}

void ldGeoFactory::add_cap(ldMesh *mesh,VertexBuffer *vertex,bool invert)
{
	VertexBuffer	dest;

	// Triangulate
	ldTriangulator::Process(*vertex,dest);

	// Set the other values
	Vec3f normal;

	if (invert)
		normal.set(0.0f,-1.0f,0.0f);
	else
		normal.set(0.0f,1.0f,0.0f);
	for (unsigned long i=0; i<dest.size(); i+=3)
	{
		if (invert)
		{
			load_tri(mesh,
					 dest[i].pos.x,dest[i].pos.y,dest[i].pos.z,normal.x,normal.y,normal.z,dest[i].uv[0].x,dest[i].uv[0].y,
					 dest[i+1].pos.x,dest[i+1].pos.y,dest[i+1].pos.z,normal.x,normal.y,normal.z,dest[i+1].uv[0].x,dest[i+1].uv[0].y,
					 dest[i+2].pos.x,dest[i+2].pos.y,dest[i+2].pos.z,normal.x,normal.y,normal.z,dest[i+2].uv[0].x,dest[i+2].uv[0].y);
		}
		else
		{
			load_tri(mesh,
					 dest[i].pos.x,dest[i].pos.y,dest[i].pos.z,normal.x,normal.y,normal.z,dest[i].uv[0].x,dest[i].uv[0].y,
					 dest[i+2].pos.x,dest[i+2].pos.y,dest[i+2].pos.z,normal.x,normal.y,normal.z,dest[i+2].uv[0].x,dest[i+2].uv[0].y,
					 dest[i+1].pos.x,dest[i+1].pos.y,dest[i+1].pos.z,normal.x,normal.y,normal.z,dest[i+1].uv[0].x,dest[i+1].uv[0].y);
		}
	}

	dest.clear();
}

ldMesh* ldGeoFactory::pyramid(long n_sides,bool capped)
{
	if (n_sides<3) return NULL;

	// Create the mesh
	ldMesh *pyramid=new_mesh();
	if (!pyramid) return NULL;

	Vec3f	center,size,tmp;
	
	center.set(_x1,_y1,_z1);
	tmp.set(_x2,_y2,_z2);
	center=center+tmp;
	center=center/2.0f;

	size.set((_x2-_x1)/2.0f,(_y2-_y1)/2.0f,(_z2-_z1)/2.0f);

	float xAng=0.0f;
	float xInc=PI_TIMES_2/n_sides;

	Vec3f	n;
	Vec3f	appex(center.x,_y2,center.z);
		
	for (long i=0; i<n_sides; i++)
	{		
		float c1=cos(xAng);
		float s1=sin(xAng);
		float c2=cos(xAng+xInc);
		float s2=sin(xAng+xInc);
		float u1=xAng/PI_TIMES_2;
		float u2=(xAng+xInc)/PI_TIMES_2;

		Vec3f	vec1(c1*size.x+center.x,_y1,s1*size.z+center.z);
		Vec3f	vec2(c2*size.x+center.x,_y1,s2*size.z+center.z);

		vec1=appex-vec1;
		vec2=vec2-appex;
		n=cross_product(vec1,vec2);
		n.normalize();

		load_tri(pyramid,
				 appex.x,appex.y,appex.z,n.x,n.y,n.z,0.0f,0.0f,
				 c2*size.x+center.x,_y1,s2*size.z+center.z,n.x,n.y,n.z,u2,1.0f,
				 c1*size.x+center.x,_y1,s1*size.z+center.z,n.x,n.y,n.z,u1,1.0f);

		xAng+=xInc;
	}

	if (capped)
	{
		VertexBuffer	buffer;

		// Lower cap
		xAng=0.0f;

		for (long i=0; i<n_sides; i++)
		{		
			float c1=cos(xAng);
			float s1=sin(xAng);
			float c2=cos(xAng+xInc);
			float s2=sin(xAng+xInc);

			ldVertex	vert;

			vert.pos.x=c1*size.x+center.x;
			vert.pos.y=_y1;
			vert.pos.z=s1*size.z+center.z;
			vert.normal.x=0.0f;
			vert.normal.y=1.0f;
			vert.normal.z=0.0f;
			for (long k=0; k<_n_tex; k++)
				vert.uv[k].set(0.0f,0.0f);

			buffer.push_back(vert);

			xAng+=xInc;
		}

		add_cap(pyramid,&buffer,true);
		buffer.clear();		
	}

	return pyramid;

}
