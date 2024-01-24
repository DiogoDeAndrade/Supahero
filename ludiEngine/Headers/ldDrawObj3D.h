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
// ldDrawObj3D.h: interface for the ldDrawObj3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDDRAWOBJ3D_H__F3A84961_F41E_4418_8102_1370C187CF5A__INCLUDED_)
#define AFX_LDDRAWOBJ3D_H__F3A84961_F41E_4418_8102_1370C187CF5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldObject3d.h"

class ldMesh;
class ldMaterial;

class ldDrawObj3D : public ldObject3D  
{
public:
	ldDrawObj3D();
	virtual ~ldDrawObj3D();

	inline	void	set_mesh(ldMesh *mesh) { _mesh=mesh; }
	inline	ldMesh	*get_mesh() { return _mesh; }
	inline	void	set_material(ldMaterial *material) { _material=material; }

	virtual void	draw();

	virtual bool	is_perfect_colliding(ldBoundingSphere *sphere,
										 unsigned long *triangle=NULL,float *s=NULL,float *t=NULL);
	virtual	bool	is_perfect_intersection(Vec3f *ray_origin,Vec3f *ray_dir,
											unsigned long *triangle=NULL,Vec3f *intersection_point=NULL);
	virtual	bool	is_perfect_colliding(ldObject3D *object,
									     unsigned long *triangle1=NULL,
									     unsigned long *triangle2=NULL);
	virtual	bool	is_perfect_colliding(ldDrawObj3D *object,
									     unsigned long *triangle1=NULL,
									     unsigned long *triangle2=NULL);

protected:
	ldMesh		*_mesh;
	ldMaterial	*_material;
};

#endif // !defined(AFX_LDDRAWOBJ3D_H__F3A84961_F41E_4418_8102_1370C187CF5A__INCLUDED_)
