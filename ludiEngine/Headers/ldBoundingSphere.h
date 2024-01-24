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
// ldBoundingSphere.h: interface for the ldBoundingSphere class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDBOUNDINGSPHERE_H__1B91217E_FA66_439E_96D9_193A59F38213__INCLUDED_)
#define AFX_LDBOUNDINGSPHERE_H__1B91217E_FA66_439E_96D9_193A59F38213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldGeometry.h"

class ldBoundingSphere  
{
public:
	ldBoundingSphere();
	ldBoundingSphere(float r);
	ldBoundingSphere(Vec3f *center,float r);
	ldBoundingSphere(float x,float y,float z,float r);
	virtual ~ldBoundingSphere();

public:
	inline	void	set_center(Vec3f *vec) { _center=*vec; }
	inline	void	set_center(float x,float y,float z) { _center.set(x,y,z); }
	inline	void	set_radius(float r) { _radius=r; }

	inline	void	get_center(Vec3f *vec) { *vec=_center; }
	inline	void	get_center(float *x,float *y,float *z) { *x=_center.x; *y=_center.y; *z=_center.z; }
	inline	float	get_radius() { return _radius; }

	inline	void	move(Vec3f &pos) { _center=_center+pos; }

	// Collision methods
public:
	virtual bool	is_inside(Vec3f *pt);

	virtual	bool	is_colliding(ldBoundingSphere *sphere);
	virtual bool	is_colliding(Vec3f *pt1,Vec3f *pt2,Vec3f *pt3,float *s=NULL,float *t=NULL);

	virtual bool	get_ray_intersection(Vec3f *origin,Vec3f *dir,float *t=NULL);
	
protected:
	Vec3f	_center;
	float	_radius;
};

#include <list>

typedef std::list<ldBoundingSphere*>	ldBoundingSpheres;

#endif // !defined(AFX_LDBOUNDINGSPHERE_H__1B91217E_FA66_439E_96D9_193A59F38213__INCLUDED_)
