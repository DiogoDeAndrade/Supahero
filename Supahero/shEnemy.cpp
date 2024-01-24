// shEnemy.cpp: implementation of the shEnemy class.
//
//////////////////////////////////////////////////////////////////////

#include "shEnemy.h"
#include "shLevel.h"
#include "shSupahero.h"
#include "../LudiEngine/Headers/ldUtils.h"
#include "../LudiEngine/Headers/ldSound.h"
#include "shUtils.h"

extern shSupahero	*supahero;

void part_func_burst_point_emitter(Vec3f *emitter_pos,Vec3f *emitter_orientation,
								   Vec3f *particle_pos,Vec3f *particle_orientation)
{
	*particle_pos=*emitter_pos;

	float ang=get_random_number(0.0f,PI_TIMES_2);
	
	particle_orientation->x=cos(ang);
	particle_orientation->y=0.0f;
	particle_orientation->z=sin(ang);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

shEnemy::shEnemy()
{
	idle=0;
	no_draw=0;
	last_shot=0;
	rise=0;
	time_to_die=0;
}

shEnemy::~shEnemy()
{

}

void shEnemy::draw()
{
	if (no_draw==0) ldDrawObj3D::draw();
}

void shEnemy::anim(unsigned long t,shLevel *level)
{
	float y=level->cast_ray_down_scenario(pos.x,pos.y,pos.z);

	if (no_draw>0)
	{
		if (no_draw>t) no_draw-=t;
		else no_draw=0;
	} else
	if (rise>0)
	{
		fpm_up(0.002*t);
		if (rise<=t) rise=0;
		else rise-=t;
	}
	else if (rise<0)
	{
		fpm_down(0.002*t);
		if (rise>=-t) rise=0;
		else rise+=t;
	}
	if (time_to_die>0)
	{
		if (time_to_die<=t)
		{
			// Character is dead
			for (unsigned long i=0; i<level->enemies.size(); i++)
			{
				if (level->enemies[i]==this)
				{
					delete level->enemies[i];
					level->enemies[i]=NULL;
					return;
				}
			}
		}
		else time_to_die-=t;
	}

	if (idle>0)
	{
		if (idle>t) { idle-=t; return; }
		else idle=0;
	}

	// Animate
	// Search target
	shVictim	*tmp=NULL;
	float		d=99999999999999.0f;
	for (unsigned long i=0; i<level->characters.size(); i++)
	{
		Vec3f	vec;
		float	d2;

		if (level->characters[i])
		{
			if (level->characters[i]->active)
			{
				vec=pos-level->characters[i]->pos;
				d2=vec.norm_squared();
				if (d2<d) { tmp=level->characters[i]; d=d2; }
			}
		}
	}
	// Go to him
	if (tmp)
	{
		if (d<100.0f)
		{
			fire(level);

			Vec3f	vec=tmp->pos-pos;
			vec.y=0.0f;
			vec.normalize();

			float xpto=level->get_level();
			if (xpto>2) xpto=2+xpto/4;

			vec=vec*xpto*0.05f*t;

			pos=pos+vec;
			pos.y=level->cast_ray_down_scenario(pos.x,200.0f,pos.z);

			idle=5000;
		}
		else
		{
			Vec3f	vec=tmp->pos-pos;
			vec.y=0.0f;
			vec.normalize();

			vec=vec*level->get_level()*t*0.05f*2.0f/3.0f;

			pos=pos+vec;
			pos.y=level->cast_ray_down_scenario(pos.x,200.0f,pos.z);
		}
	}
	
	Vec3f	vec;
	vec=pos-supahero->pos;
	if (vec.norm_squared()<100.0f)
	{
		fire(level);
	}
}

void shEnemy::fire(shLevel *level)
{
	if (last_shot!=0)
	{
		if ((timeGetTime()-last_shot)<10000) return;
	}

	// The particle system
	ldParticleSystem *particle_system=GFX->new_particle_system();
	particle_system->set_emitter_function(part_func_burst_point_emitter);
	particle_system->set_life(1000);
	particle_system->set_rate(500);
	particle_system->set_initial_life(2000,1000);
	particle_system->set_initial_speed(10.0f,0.0f);
	particle_system->set_life_cycle_color(0.0f,1.0f,1.0f,1.0f,1.0f);
	particle_system->set_life_cycle_color(0.5f,0.0f,1.0f,1.0f,0.5f);
	particle_system->set_life_cycle_color(1.0f,0.0f,1.0f,1.0f,0.0f);
	particle_system->set_particle_size(0.0f,3.0f);
	particle_system->set_texture(get_texture("Textures/Particle.png"));
	particle_system->set_blend_mode(LDBLENDMODE_ADD);
	
	particle_system->pos.set(pos.x,pos.y+5.0f,pos.z);
	
	particle_system->add_bounding_sphere(0.0f,0.0f,0.0f,0.5f);

	level->particles.push_back(particle_system);
	shPSInfo info;
	info.start_time=timeGetTime();
	info.destroyer=true;
	info.power=false;
	info.spawner=this;
	level->ps_infos.push_back(info);

	last_shot=timeGetTime();

	SND->play(sounds[6]);
}
