/******************************************************************************************
*                                      OBJECT3D											  *
*                                                                                         *
* DESCRIPTION: Implements a class to enable easy movement and simple collision detection  *
*                                                                                         *
* AUTHORS: Pedro Pires <pedro.pires@acm.org>                                              *
*		   Diogo de Andrade <Diogo.Andrade@netvisao.pt>									  *
*                                                                                         *
* COPYRIGHT: Copyright 2001, Pedro Pires & Diogo Andrade. All rights reserved.            *
******************************************************************************************/

#ifndef __PP_SSTORM_ldObject3D__
#define __PP_SSTORM_ldObject3D__

#include <string>
#include "ldGeometry.h"
#include "ldQuaternion.h"
#include "ldBoundingSphere.h"

class ldObject3D
{
public:
	ldObject3D();
	~ldObject3D();

	// Funções auxiliares para a movimentação do objecto

	// Free Movement (FM)
	void fm_forward(float amount);				// Na direcção de lookat
	void fm_back(float amount);					// Na direcção oposta a lookat
	void fm_sidestep_left(float amount);		// Na direcção oposta a vx
	void fm_sidestep_right(float amount);		// Na direcção de vx
	void fm_up(float amount);					// Na direcção de vup
	void fm_down(float amount);					// Na direcção oposta a vup
	void fm_turn_left(float amount);			// Sobre vup
	void fm_turn_right(float amount);			// Sobre vup
	void fm_look_up(float amount);				// 
	void fm_look_down(float amount);			// 
	void fm_roll_left(float amount);			// 
	void fm_roll_right(float amount);			// 

	// First Person Movement (FPM)
	void fpm_forward(float amount);				// Na direcção de proj(lookat,plano_xz)
	void fpm_back(float amount);				// Na direcção oposta a proj(lookat,plano_xz)
	void fpm_sidestep_left(float amount);		// Na direcção oposta a proj(vx,plano_xz)
	void fpm_sidestep_right(float amount);		// Na direcção de proj(vx,plano_xz)
	void fpm_up(float amount);					// Na direcção de yy (world)
	void fpm_down(float amount);				// Na direcção oposta a yy (world)
	void fpm_turn_left(float amount);			// Sobre yy (world)
	void fpm_turn_right(float amount);			// Sobre yy (world)
	void fpm_look_up(float amount);				// 
	void fpm_look_down(float amount);			// 
	void fpm_roll_left(float amount);			// 
	void fpm_roll_right(float amount);			// 

	// Non-orientation based translation
	void no_forward(float amount);
	void no_back(float amount);
	void no_sidestep_left(float amount);
	void no_sidestep_right(float amount);
	void no_up(float amount);
	void no_down(float amount);
	void no_move_to(float x,float y,float z);

	// Collision detection
	inline	void	add_bounding_sphere(ldBoundingSphere *sphere) { if (sphere) bounding_spheres.push_back(sphere); }
	inline	void	add_bounding_sphere(float x,float y,float z,float r) { ldBoundingSphere *sphere=new ldBoundingSphere(x,y,z,r); bounding_spheres.push_back(sphere); }
			void	remove_bounding_sphere(ldBoundingSphere *sphere);
			void	delete_bounding_sphere(ldBoundingSphere *sphere);
			void	delete_all_bounding_spheres();

	virtual	bool	is_colliding(ldObject3D *object);
	virtual	bool	is_colliding(ldBoundingSphere *sphere);
	virtual bool	is_intersected(Vec3f *ray_origin,Vec3f *ray_dir,float *t=NULL);

	virtual	bool	is_perfect_colliding(ldBoundingSphere *sphere,
										 unsigned long *triangle=NULL,float *s=NULL,float *t=NULL);
	virtual	bool	is_perfect_intersection(Vec3f *ray_origin,Vec3f *ray_dir,
											unsigned long *triangle=NULL,float *t=NULL);
	virtual	bool	is_perfect_colliding(ldObject3D *object,
										 unsigned long *triangle1=NULL,
										 unsigned long *triangle2=NULL);

	// Helper function
	inline	Vec3f	get_orientation() { Vec3f tmp; Vec3f vec(0.0f,0.0f,-1.0f); ori.rotate_point(tmp,vec); return tmp; }

public:
	Vec3f				pos;
	Quaternion			ori;
	ldBoundingSpheres	bounding_spheres;
};

#endif
