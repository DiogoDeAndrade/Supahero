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
// ldDrawObj3D.cpp: implementation of the ldDrawObj3D class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldDrawObj3D.h"
#include "..\HEADERS\ldGfxD3D.h"
#include "..\HEADERS\ldMaterial.h"
#include "..\HEADERS\ldMesh.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldDrawObj3D::ldDrawObj3D()
{
	_material=NULL;
	_mesh=NULL;
}

ldDrawObj3D::~ldDrawObj3D()
{

}

void ldDrawObj3D::draw()
{
	GFX->set_world_transformation(0,&pos,&ori);

	if (_material) _material->set();
	if (_mesh) _mesh->draw();
}

bool ldDrawObj3D::is_perfect_colliding(ldBoundingSphere *sphere,
									  unsigned long *triangle,float *s,float *t)
{
	if (bounding_spheres.size()>0)
	{
		if (!is_colliding(sphere)) return false;
	}

	return _mesh->is_colliding(&pos,&ori,sphere,triangle,s,t);
}

bool ldDrawObj3D::is_perfect_intersection(Vec3f *ray_origin,Vec3f *ray_dir,
										 unsigned long *triangle,Vec3f *intersection_point)
{
	if (bounding_spheres.size()>0)
	{
		float t;

		if (!is_intersected(ray_origin,ray_dir,&t)) return false;
	}

	return _mesh->is_intersected(&pos,&ori,ray_origin,ray_dir,triangle,intersection_point);
}

bool ldDrawObj3D::is_perfect_colliding(ldObject3D *object,
									   unsigned long *triangle1,
									   unsigned long *triangle2)
{
	return object->is_perfect_colliding(this,triangle1,triangle2);
}

bool ldDrawObj3D::is_perfect_colliding(ldDrawObj3D *object,
									   unsigned long *triangle1,
									   unsigned long *triangle2)
{
	return _mesh->is_colliding(&pos,&ori,object->_mesh,&(object->pos),&(object->ori),
							   triangle1,triangle2);
}
