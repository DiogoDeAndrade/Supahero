// shSupahero.cpp: implementation of the shSupahero class.
//
//////////////////////////////////////////////////////////////////////

#include "shSupahero.h"
#include "shUtils.h"
#include "../LudiEngine/Headers/ldUtils.h"

void part_func_supahero_die_emitter(Vec3f *emitter_pos,Vec3f *emitter_orientation,
								    Vec3f *particle_pos,Vec3f *particle_orientation)
{
	*particle_pos=*emitter_pos;

	particle_orientation->x=get_random_number(-1.0f,1.0f);
	particle_orientation->y=1.0f;
	particle_orientation->z=get_random_number(-1.0f,1.0f);

	particle_orientation->normalize();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

shSupahero::shSupahero()
{
	_mesh=supa_model[0];
	vec.set(0.0f,0.0f,0.0f);
	time_since_last_shot=0;
	energy=1000;
	time_to_die=0;
	invulnerable=0;
	add_bounding_sphere(0.0f,0.0f,0.0f,5.0f);
}

shSupahero::~shSupahero()
{

}

void shSupahero::anim(unsigned long t,shLevel *level)
{
	if (time_to_die==0)
	{
		if (invulnerable>t) invulnerable-=t;
		else invulnerable=0;

		if ((vec.x!=0.0f) || (vec.z!=0.0f)) 
		{
			ori.identity_quaternion();
			vec.normalize();
		}

		vec=vec*t*SUPA_MOVE_SPEED;

		pos=pos+vec;

		pos.y=level->cast_ray_down_scenario(pos.x,200.0f,pos.z);

		// Rotate character
		if ((vec.x>0.0f) && (vec.z<0.0f)) fpm_turn_right(45.0f); else	// NE
		if ((vec.x==0.0f) && (vec.z<0.0f)) fpm_turn_right(0.0f); else	// N
		if ((vec.x<0.0f) && (vec.z<0.0f)) fpm_turn_left(45.0f);	 else	// NW
		if ((vec.x<0.0f) && (vec.z==0.0f)) fpm_turn_left(90.0f); else	// E
		if ((vec.x>0.0f) && (vec.z==0.0f)) fpm_turn_right(90.0f); else	// W
		if ((vec.x>0.0f) && (vec.z>0.0f)) fpm_turn_right(135.0f); else	// SE
		if ((vec.x==0.0f) && (vec.z>0.0f)) fpm_turn_right(180.0f); else	// S
		if ((vec.x<0.0f) && (vec.z>0.0f)) fpm_turn_left(135.0f);		// SW

		if (invulnerable==0)
		{
			// Check for life drain or something
			// Test for killings
			for (unsigned long i=0; i<level->particles.size(); i++)
			{
				if (level->particles[i]) 
				{
					if (level->ps_infos[i].destroyer)
					{
						if (is_colliding(level->particles[i]))
						{
							// Victim dies an horrible death
							energy-=250*(level->level);
							invulnerable=2000;

							if (energy<=0) 
							{
								// Dying
								time_to_die=5000.0f;
								// The particle system
								ldParticleSystem *particle_system=GFX->new_particle_system();
								particle_system->set_emitter_function(part_func_supahero_die_emitter);
								particle_system->set_life(2500);
								particle_system->set_rate(2000);
								particle_system->set_initial_life(3000,500);
								particle_system->set_initial_speed(4.0f,0.5f);
								particle_system->set_life_cycle_color(0.0f,1.0f,1.0f,1.0f,1.0f);
								particle_system->set_life_cycle_color(1.0f,1.0f,1.0f,1.0f,0.0f);
								particle_system->set_particle_size(0.0f,4.0f);
								particle_system->set_texture(get_texture("Textures/Particle.png"));
								particle_system->set_blend_mode(LDBLENDMODE_ADD);
								particle_system->pos=pos;

								level->particles.push_back(particle_system);

								shPSInfo info;
								info.start_time=timeGetTime();
								info.destroyer=false;
								info.power=false;
								info.spawner=this;
								level->ps_infos.push_back(info);

								SND->play(sounds[10]);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		if (time_to_die<t) time_to_die=0;
		else time_to_die-=t;
	}
}

void shSupahero::fire(shLevel *level)
{
	if ((time_since_last_shot!=0) &&
		(timeGetTime()-time_since_last_shot<1000)) return;
	
	Vec3f	vec(0.0f,0.0f,-1.0f);
	Vec3f	tmp;

	ori.rotate_point(tmp,vec);

	// The particle system
	ldParticleSystem *particle_system=GFX->new_particle_system();
	particle_system->set_emitter_function(part_func_omni_point_emitter);
	particle_system->set_life(3000);
	particle_system->set_rate(500);
	particle_system->set_initial_life(500,250);
	particle_system->set_initial_speed(1.0f,0.5f);
	particle_system->set_life_cycle_color(0.0f,1.0f,0.1f,0.1f,1.0f);
	particle_system->set_life_cycle_color(1.0f,0.1f,0.1f,0.1f,0.0f);
	particle_system->set_particle_size(0.0f,1.0f);
	particle_system->set_texture(get_texture("Textures/Particle.png"));
	particle_system->set_blend_mode(LDBLENDMODE_ADD);
	particle_system->add_bounding_sphere(0.0f,0.0f,0.0f,2.0f);
	particle_system->pos=pos; particle_system->pos.y+=7.5f;

	level->particles.push_back(particle_system);
	
	shPSInfo info;
	info.start_time=timeGetTime();
	info.destroyer=false;
	info.power=true;
	info.spawner=this;
	info.dir=tmp;
	level->ps_infos.push_back(info);

	time_since_last_shot=timeGetTime();

	SND->play(sounds[9]);
}
