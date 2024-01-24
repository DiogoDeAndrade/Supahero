/******************************************************************************************
*                                      OBJECT3D                                           *
*                                                                                         *
* DESCRIPTION: Implements a class to enable easy movement, and some basic collision		  *
*              detection...																  *
*                                                                                         *
* AUTHOR: Pedro Pires <pedro.pires@acm.org>                                               *
*         Diogo de Andrade <diogo.andrade@netvisao.pt                                     *
*                                                                                         *
* COPYRIGHT: Copyright 2001, Pedro Pires & Diogo Andrade. All rights reserved.            *
******************************************************************************************/

#include "../Headers/ldObject3D.h"
#include <float.h>

// ************************************************************************************************
// Free Movement (FM)
// ************************************************************************************************

ldObject3D::ldObject3D()
{
	pos.set(0.0f,0.0f,0.0f);
	ori.rotation_quaternion(1.0f,0.0f,0.0f,0.0f);
}

ldObject3D::~ldObject3D()
{
	bounding_spheres.clear();
}

void ldObject3D::fm_forward(float amount = 1.0f)				// Na direcção de lookat
{
	Vec3f heading;
	ori.rotate_point(heading,Vec3f(0,0,-1));
	pos = pos + amount * heading;
}

void ldObject3D::fm_back(float amount = 1.0f)					// Na direcção oposta a lookat
{
	Vec3f heading;
	ori.rotate_point(heading,Vec3f(0,0,-1));
	pos = pos - amount * heading;
}

void ldObject3D::fm_sidestep_left(float amount = 1.0f)		// Na direcção oposta a vx
{
	Vec3f right;
	ori.rotate_point(right,Vec3f(1,0,0));
	pos = pos - amount * right;
}

void ldObject3D::fm_sidestep_right(float amount = 1.0f)		// Na direcção de vx
{
	Vec3f right;
	ori.rotate_point(right,Vec3f(1,0,0));
	pos = pos + amount * right;
}

void ldObject3D::fm_up(float amount = 1.0f)					// Na direcção de vup
{
	Vec3f up;
	ori.rotate_point(up,Vec3f(0,1,0));
	pos = pos + amount * up;
}

void ldObject3D::fm_down(float amount = 1.0f)					// Na direcção oposta a vup
{
	Vec3f up;
	ori.rotate_point(up,Vec3f(0,1,0));
	pos = pos - amount * up;
}

void ldObject3D::fm_turn_left(float amount = 5.0f)			// Sobre vup
{
	Quaternion r,oinit = ori;
	r.rotation_quaternion(0,1,0,amount);
	ori.mult(r,oinit);
}

void ldObject3D::fm_turn_right(float amount = 5.0f)			// Sobre vup
{
	Quaternion r,oinit = ori;
	r.rotation_quaternion(0,1,0,-amount);
	ori.mult(r,oinit);
}

void ldObject3D::fm_look_up(float amount = 5.0f)				// 
{
	Vec3f right;
	ori.rotate_point(right,Vec3f(1,0,0));
	Quaternion r, oinit = ori;
	r.rotation_quaternion(right,amount);
	ori.mult(r,oinit);
}

void ldObject3D::fm_look_down(float amount = 5.0f)			// 
{
	Vec3f right;
	ori.rotate_point(right,Vec3f(1,0,0));
	Quaternion r, oinit = ori;
	r.rotation_quaternion(right,-amount);
	ori.mult(r,oinit);
}

void ldObject3D::fm_roll_left(float amount = 5.0f)			// 
{
	Vec3f heading;
	ori.rotate_point(heading,Vec3f(0,0,-1));
	Quaternion r, oinit = ori;
	r.rotation_quaternion(heading,amount);
	ori.mult(r,oinit);
}

void ldObject3D::fm_roll_right(float amount = 5.0f)			// 
{
	Vec3f heading;
	ori.rotate_point(heading,Vec3f(0,0,-1));
	Quaternion r, oinit = ori;
	r.rotation_quaternion(heading,-amount);
	ori.mult(r,oinit);
}


// ************************************************************************************************
// First Person Movement (FPM)
// ************************************************************************************************

void ldObject3D::fpm_forward(float amount = 1.0f)				// Na direcção de proj(lookat,plano_xz)
{
	Vec3f heading;
	ori.rotate_point(heading,Vec3f(0,0,-1));
	heading.y = 0;
	if((heading.x == 0) && (heading.z == 0)){
		return;
	}
	pos = pos + amount * heading;
}

void ldObject3D::fpm_back(float amount = 1.0f)				// Na direcção oposta a proj(lookat,plano_xz)
{
	Vec3f heading;
	ori.rotate_point(heading,Vec3f(0,0,-1));
	heading.y = 0;
	if((heading.x == 0) && (heading.z == 0)){
		return;
	}
	pos = pos - amount * heading;
}

void ldObject3D::fpm_sidestep_left(float amount = 1.0f)		// Na direcção oposta a proj(vx,plano_xz)
{
	Vec3f right;
	ori.rotate_point(right,Vec3f(1,0,0));
	right.y = 0;
	if((right.x == 0) && (right.z == 0)){
		return;
	}
	pos = pos - amount * right;
}

void ldObject3D::fpm_sidestep_right(float amount = 1.0f)		// Na direcção de proj(vx,plano_xz)
{
	Vec3f right;
	ori.rotate_point(right,Vec3f(1,0,0));
	right.y = 0;
	if((right.x == 0) && (right.z == 0)){
		return;
	}
	pos = pos + amount * right;
}

void ldObject3D::fpm_up(float amount = 1.0f)					// Na direcção de yy (world)
{
	pos.y += amount;
}

void ldObject3D::fpm_down(float amount = 1.0f)				// Na direcção oposta a yy (world)
{
	pos.y -= amount;
}

void ldObject3D::fpm_turn_left(float amount = 5.0f)			// Sobre yy (world)
{
	Quaternion r,oinit = ori;
	r.rotation_quaternion(0,1,0,amount);
	ori.mult(r,oinit);
}

void ldObject3D::fpm_turn_right(float amount = 5.0f)			// Sobre yy (world)
{
	Quaternion r,oinit = ori;
	r.rotation_quaternion(0,1,0,-amount);
	ori.mult(r,oinit);
}

void ldObject3D::fpm_look_up(float amount = 5.0f)				// 
{
	Vec3f right;
	ori.rotate_point(right,Vec3f(1,0,0));
	Quaternion r, oinit = ori;
	r.rotation_quaternion(right,amount);
	ori.mult(r,oinit);
}

void ldObject3D::fpm_look_down(float amount = 5.0f)			// 
{
	Vec3f right;
	ori.rotate_point(right,Vec3f(1,0,0));
	Quaternion r, oinit = ori;
	r.rotation_quaternion(right,-amount);
	ori.mult(r,oinit);
}

void ldObject3D::fpm_roll_left(float amount = 5.0f)			// 
{
	Vec3f heading;
	ori.rotate_point(heading,Vec3f(0,0,-1));
	Quaternion r, oinit = ori;
	r.rotation_quaternion(heading,amount);
	ori.mult(r,oinit);
}

void ldObject3D::fpm_roll_right(float amount = 5.0f)			// 
{
	Vec3f heading;
	ori.rotate_point(heading,Vec3f(0,0,-1));
	Quaternion r, oinit = ori;
	r.rotation_quaternion(heading,-amount);
	ori.mult(r,oinit);
}

void ldObject3D::no_forward(float amount = 1.0f)
{
	pos.z-=amount;
}

void ldObject3D::no_back(float amount = 1.0f)
{
	pos.z+=amount;
}

void ldObject3D::no_sidestep_left(float amount = 1.0f)
{
	pos.x-=amount;
}

void ldObject3D::no_sidestep_right(float amount = 1.0f)
{
	pos.x+=amount;
}

void ldObject3D::no_up(float amount = 1.0f)
{
	pos.y+=amount;
}

void ldObject3D::no_down(float amount = 1.0f)
{
	pos.y-=amount;
}

void ldObject3D::no_move_to(float x,float y,float z)
{
	pos.x=x;
	pos.y=y;
	pos.z=z;
}

void ldObject3D::remove_bounding_sphere(ldBoundingSphere *sphere)
{
	ldBoundingSpheres::iterator i;

	for (i=bounding_spheres.begin(); i!=bounding_spheres.end(); ++i)
	{
		if (*i==sphere) i=bounding_spheres.erase(i);
	}
}

void ldObject3D::delete_bounding_sphere(ldBoundingSphere *sphere)
{
	ldBoundingSpheres::iterator i;

	for (i=bounding_spheres.begin(); i!=bounding_spheres.end(); ++i)
	{
		if (*i==sphere) 
		{
			delete sphere;
			i=bounding_spheres.erase(i);
			return;
		}
	}
}

void ldObject3D::delete_all_bounding_spheres()
{
	ldBoundingSpheres::iterator i;

	for (i=bounding_spheres.begin(); i!=bounding_spheres.end(); ++i)
	{
		delete *i;
	}

	bounding_spheres.clear();
}

bool ldObject3D::is_colliding(ldObject3D *object)
{
	ldBoundingSpheres::iterator i;

	for (i=bounding_spheres.begin(); i!=bounding_spheres.end(); ++i)
	{
		if (*i)
		{
			(*i)->move(pos);
			if (object->is_colliding(*i)) 
			{
				(*i)->move(-pos);
				return true;
			}
			(*i)->move(-pos);
		}
	}	

	return false;
}

bool ldObject3D::is_colliding(ldBoundingSphere *sphere)
{
	ldBoundingSpheres::iterator i;

	for (i=bounding_spheres.begin(); i!=bounding_spheres.end(); ++i)
	{
		if (*i)
		{
			(*i)->move(pos);
			if (sphere->is_colliding(*i)) 
			{
				(*i)->move(-pos);
				return true;
			}
			(*i)->move(-pos);
		}
	}	

	return false;
}

bool ldObject3D::is_intersected(Vec3f *ray_origin,Vec3f *ray_dir,float *t)
{
	ldBoundingSpheres::iterator i;
	float						tTmp,tMin=FLT_MAX;
	bool						col=false;

	for (i=bounding_spheres.begin(); i!=bounding_spheres.end(); ++i)
	{
		if (*i)
		{
			(*i)->move(pos);
			col=col || (*i)->get_ray_intersection(ray_origin,ray_dir,&tTmp);
			if (tTmp<tMin) tMin=tTmp;
			(*i)->move(-pos);
		}
	}	

	if (t) *t=tMin;

	return col;
}

bool ldObject3D::is_perfect_colliding(ldBoundingSphere *sphere,
									  unsigned long *triangle,float *s,float *t)
{
	if (triangle) *triangle=0x00FFFFFFFF;
	if (*s) *s=-1.0f;
	if (*t) *t=-1.0f;

	return is_colliding(sphere);
}

bool ldObject3D::is_perfect_intersection(Vec3f *ray_origin,Vec3f *ray_dir,
										 unsigned long *triangle,float *t)
{
	if (triangle) *triangle=0x00FFFFFFFF;
	if (*t) *t=-1.0f;

	return is_intersected(ray_origin,ray_dir,t);
}

bool ldObject3D::is_perfect_colliding(ldObject3D *object,
									  unsigned long *triangle1,
									  unsigned long *triangle2)
{
	if (triangle1) *triangle1=0x00FFFFFFFF;
	if (triangle2) *triangle2=0x00FFFFFFFF;

	ldBoundingSpheres::iterator i;

	for (i=bounding_spheres.begin(); i!=bounding_spheres.end(); ++i)
	{
		if (*i)
		{
			(*i)->move(pos);
			if (object->is_colliding(*i))
			{
				(*i)->move(-pos);
				return true;
			}
		}
	}	

	return false;
}
