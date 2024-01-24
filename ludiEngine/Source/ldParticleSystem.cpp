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
// ldParticleSystem.cpp: implementation of the ldParticleSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldParticleSystem.h"
#include "..\HEADERS\ldUtils.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldParticleSystem::ldParticleSystem()
{
	_initial_size=0.5f; _final_size=0.0f;
	_texture=NULL;
	_base_life=1000.0f; _variance_life=0.0f;
	_base_speed=1.0f; _variance_speed=0.0f;
	_emitter=NULL;
	_physics=NULL;
	_life=0x00FFFFFFFF;
	_rate=0;
}

ldParticleSystem::~ldParticleSystem()
{
	_particles.clear();
	
	ldColorCycle::iterator	i;
	for (i=_color_cycle.begin(); i!=_color_cycle.end(); ++i)
	{
		ldColorCycleElem	*tmp=*i;
		if (tmp)
		{
			delete tmp;
		}
	}
	_color_cycle.clear();
}

void ldParticleSystem::set_life_cycle_color(float t,float r,float g,float b,float a)
{
	ldColorCycleElem		*elem;
	ldColorCycle::iterator	i;

	for (i=_color_cycle.begin(); i!=_color_cycle.end(); ++i)
	{
		if ((*i)->t==t) return;
		if ((*i)->t>t)
		{
			elem=new ldColorCycleElem;
			elem->t=t;
			elem->color.x=r; 
			elem->color.y=g; 
			elem->color.z=b; 
			elem->color.w=a; 
			_color_cycle.insert(i,elem);
			return;
		}
	}

	elem=new ldColorCycleElem;
	elem->t=t;
	elem->color.x=r; 
	elem->color.y=g; 
	elem->color.z=b; 
	elem->color.w=a; 
	_color_cycle.push_back(elem);
}

void ldParticleSystem::generate(unsigned long n_part)
{
	if (!_emitter) return;
	if (_life==0) return;

	ldParticle	*part;
	
	Vec3f		source(0.0f,0.0f,1.0f);
	Vec3f		vec;

	ori.rotate_point(vec,source);

	for (unsigned long i=0; i<n_part; i++)
	{
		// Get a new particle
		if (_deleted.size()>0)
		{
			part=*(_deleted.begin());
			_deleted.erase(_deleted.begin());
		}
		else
		{
			part=new ldParticle;
		}

		_emitter(&pos,&vec,&(part->pos),&(part->ori));

		ldColorCycleElem	*c1;
		get_life_cycle_color(0.0f,&c1,NULL);

		if (c1->t!=0.0f)
		{
			part->color.set(1.0f,1.0f,1.0f,1.0f);
		}
		part->current_life=part->starting_life=get_random_number(_base_life-_variance_life,_base_life+_variance_life);
		part->speed=get_random_number(_base_speed-_variance_speed,_base_speed+_variance_speed);
		part->size=_initial_size;

		_particles.push_back(part);
	}
}

void ldParticleSystem::get_life_cycle_color(float t,ldColorCycleElem **c1,ldColorCycleElem **c2)
{
	ldColorCycleElem		elem;
	ldColorCycle::iterator	i;

	if (t==0.0f)
	{
		if (c1) *c1=*(_color_cycle.begin());
		if (c2) *c2=*c1;
		return;
	}

	for (i=_color_cycle.begin(); i!=_color_cycle.end(); ++i)
	{
		ldColorCycleElem	*tmp=*i;

		if (tmp->t==t)
		{
			if (c1) *c1=(*i);
			if (c2) *c2=(*i);
			return;
		}
		if (tmp->t>t)
		{
			if (c2) *c2=(*i);
			--i;
			if (c1) *c1=(*i);
			return;
		}
	}
}

void ldParticleSystem::anim(unsigned long t)
{
	ldParticles::iterator	i;
	ldParticles::iterator	end=_particles.end();
	float					time=t/1000.0f;

	if (_life<t)
	{
		// System dies
		_life=0;
	}
	else
	{
		if (_life!=0x00FFFFFFFF)
		{
			_life-=t;
		}
		// Generate particles
		if (_rate!=0) generate((t*_rate)/1000);
	}

	for (i=_particles.begin(); i!=end; ++i)
	{
		ldParticle *tmp=*i;
		// Update life and delete particle if necessary
		tmp->current_life-=t;
		if ((tmp->current_life)<=0) 
		{
			i=_particles.erase(i);
			_deleted.push_back(tmp);
		}
		else
		{
			// Get phase of life cycle
			float t2=1.0f-(tmp->current_life)/(tmp->starting_life);
			// Use physics (more usefull to change orientation and speed)
			if (_physics) _physics(t,&(tmp->pos),&(tmp->ori),&(tmp->speed));
			// Update particle position
			tmp->pos.x+=(tmp->ori.x)*(tmp->speed)*time;
			tmp->pos.y+=(tmp->ori.y)*(tmp->speed)*time;
			tmp->pos.z+=(tmp->ori.z)*(tmp->speed)*time;
			// Update color
			ldColorCycleElem	*c1,*c2;

			get_life_cycle_color(t2,&c1,&c2);

			float t3=t2-c1->t+(t2*(c2->t-c1->t));

			interpolate_color(t3,&(c1->color),&(c2->color),&(tmp->color));

			tmp->size=(_initial_size)+t2*(_final_size-_initial_size);
		}
	}

/*	if (bounding_spheres.size()!=0)
	{
		// Recalc bounding sphere
		Vec3f	sum(0.0f,0.0f,0.0f);

		// Get the center
		for (i=_particles.begin(); i!=end; ++i)
		{
			sum=sum+((*i)->pos);
		}
		sum=sum/_particles.size();

		// Get the radius
		Vec3f	vec;
		float	d=99999999999.9f;
		float	d2;
		for (i=_particles.begin(); i!=end; ++i)
		{
			vec=(*i)->pos-sum;
			d2=vec.norm_squared();
			if (d2>d) d=d2;
		}

		ldBoundingSphere *tmp=*(bounding_spheres.begin());
		tmp->set_center(&sum);
		tmp->set_radius(d);
	}*/
}

void ldParticleSystem::interpolate_color(float t,Vec4f *c1,Vec4f *c2,Vec4f *c)
{
	c->x=(c1->x)+t*((c2->x)-(c1->x));
	c->y=(c1->y)+t*((c2->y)-(c1->y));
	c->z=(c1->z)+t*((c2->z)-(c1->z));
	c->w=(c1->w)+t*((c2->w)-(c1->w));
}

bool ldParticleSystem::is_alive()
{
	if ((_life==0) && (_particles.size()==0)) return false;
	
	return true;
}

// Omni-directional point emitter function
void part_func_omni_point_emitter(Vec3f *emitter_pos,Vec3f *emitter_orientation,
								  Vec3f *particle_pos,Vec3f *particle_orientation)
{
	*particle_pos=*emitter_pos;

	particle_orientation->x=get_random_number(-1.0f,1.0f);
	particle_orientation->y=get_random_number(-1.0f,1.0f);
	particle_orientation->z=get_random_number(-1.0f,1.0f);

	particle_orientation->normalize();
}

// Gravity-type physics function
void gravity_physics(float t,
					 Vec3f *pos,Vec3f *orientation,float *speed)
{
	Vec3f	gravity;

	gravity.set(0.0f,-0.005f*t,0.0f);

	*orientation=*orientation*(*speed)+gravity;

	*speed=orientation->norm();

	*orientation=(*orientation)/(*speed);
}

