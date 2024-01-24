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
// ldParticleSystem.h: interface for the ldParticleSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDPARTICLESYSTEM_H__95F40D54_E976_4D32_A419_C3056EFCD5A6__INCLUDED_)
#define AFX_LDPARTICLESYSTEM_H__95F40D54_E976_4D32_A419_C3056EFCD5A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldObject3d.h"
#include "ldGfx.h"

class ldTexture;

typedef void (*ldEmitterFunction)(Vec3f *emitter_pos,Vec3f *emitter_orientation,
								  Vec3f *particle_pos,Vec3f *particle_orientation);

typedef void (*ldPhysicsFunction)(float t,
								  Vec3f *pos,Vec3f *orientation,float *speed);

#include <list>

struct ldColorCycleElem {
	Vec4f	color;
	float	t;
};

typedef std::list<ldColorCycleElem*>	ldColorCycle;

struct ldParticle {
	bool  dead;
	float starting_life;
	float current_life;

	Vec3f pos;
	Vec3f ori;
	float speed;

	float size;
	Vec4f color;
};

typedef std::list<ldParticle*> ldParticles;

class ldParticleSystem : public ldObject3D  
{
public:
	ldParticleSystem();
	virtual ~ldParticleSystem();

	inline	void	set_life(unsigned long life) { _life=life; }
	inline	void	set_rate(unsigned long rate) { _rate=rate; }
	inline	void	set_particle_size(float initial_size,float final_size) { _initial_size=initial_size; _final_size=final_size; }
	inline	void	set_initial_life(float base,float variance) { _base_life=base; _variance_life=variance; }
	inline	void	set_initial_speed(float base,float variance) { _base_speed=base; _variance_speed=variance; }
			void	set_life_cycle_color(float t,float r,float g,float b,float a);
	inline	void	set_emitter_function(ldEmitterFunction function) { _emitter=function; }
	inline	void	set_physics_function(ldPhysicsFunction function) { _physics=function; }
	inline	void	set_texture(ldTexture *tex) { _texture=tex; }
	inline	void	set_blend_mode(LDBlendMode mode) { _blend_mode; }

	inline	unsigned long	get_particle_count() { return _particles.size(); }

	virtual void	anim(unsigned long t);
	virtual	void	generate(unsigned long n_part);
	virtual	void	draw() = 0;

	virtual bool	is_alive();

	inline	void	change_bounding_sphere_radius(float r) { (*(bounding_spheres.begin()))->set_radius(r); }

protected:
			void	get_life_cycle_color(float t,ldColorCycleElem **c1,ldColorCycleElem **c2);
			void	interpolate_color(float t,Vec4f *c1,Vec4f *c2,Vec4f *c);

protected:
	float				_initial_size,_final_size;
	ldTexture*			_texture;
	float				_base_life,_variance_life;
	float				_base_speed,_variance_speed;
	ldEmitterFunction	_emitter;
	ldPhysicsFunction	_physics;
	ldColorCycle		_color_cycle;
	ldParticles			_particles;
	ldParticles			_deleted;
	LDBlendMode			_blend_mode;
	unsigned long		_life;
	unsigned long		_rate;
};

// Some standart helper functions

// Omni-directional point emitter function
void part_func_omni_point_emitter(Vec3f *emitter_pos,Vec3f *emitter_orientation,
								  Vec3f *particle_pos,Vec3f *particle_orientation);

// Gravity-type physics function
void gravity_physics(float t,
					 Vec3f *pos,Vec3f *orientation,float *speed);

#endif // !defined(AFX_LDPARTICLESYSTEM_H__95F40D54_E976_4D32_A419_C3056EFCD5A6__INCLUDED_)
