// shLevel.cpp: implementation of the shLevel class.
//
//////////////////////////////////////////////////////////////////////

#include "shLevel.h"
#include "shUtils.h"
#include "shGeoFactory.h"
#include "../LudiEngine/Headers/ldGfx.h"
#include "../LudiEngine/Headers/ldGeoFilter.h"
#include "../LudiEngine/Headers/ldUtils.h"
#include "../LudiEngine/Headers/ldGeometry.h"
#include "../LudiEngine/Headers/ldCamera3d.h"

extern ldCamera3D* camera;

void part_func_vertical_point_emitter(Vec3f *emitter_pos,Vec3f *emitter_orientation,
								      Vec3f *particle_pos,Vec3f *particle_orientation)
{
	*particle_pos=*emitter_pos;

	particle_orientation->x=get_random_number(-1.0f,1.0f);
	particle_orientation->y=1.0f;
	particle_orientation->z=get_random_number(-1.0f,1.0f);

	particle_orientation->normalize();
}

void part_func_death_emitter(Vec3f *emitter_pos,Vec3f *emitter_orientation,
							 Vec3f *particle_pos,Vec3f *particle_orientation)
{
	particle_pos->x=emitter_pos->x+get_random_number(-4.0f,4.0f);
	particle_pos->y=emitter_pos->y+get_random_number(0.0f,8.0f);
	particle_pos->z=emitter_pos->z+get_random_number(-4.0f,4.0f);

	float ang=get_random_number(0.0f,PI_TIMES_2);
	particle_orientation->x=cos(0.37)*cos(ang);
	particle_orientation->y=sin(0.37);
	particle_orientation->z=cos(0.37)*sin(ang);
}

void part_func_vertical_square_emitter(Vec3f *emitter_pos,Vec3f *emitter_orientation,
								       Vec3f *particle_pos,Vec3f *particle_orientation)
{
	particle_pos->x=emitter_pos->x+get_random_number(-3.0f,3.0f);
	particle_pos->y=emitter_pos->y;
	particle_pos->z=emitter_pos->z+get_random_number(-3.0f,3.0f);

	particle_orientation->x=get_random_number(-1.0f,1.0f);
	particle_orientation->y=1.0f;
	particle_orientation->z=get_random_number(-1.0f,1.0f);

	particle_orientation->normalize();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

shLevel::shLevel()
{

}

shLevel::~shLevel()
{
	clear();
}

bool shLevel::init_enviroment(unsigned long lvl)
{
	level=lvl;
	
	long l=((level-1)%4)+1;
	switch (l)
	{
		case 1:
			init_grassland();
			init_characters(40);
			init_params();
			must_survive=38;
			timer=2*60*1000;
			break;
		case 2:
			init_desert();
			init_characters(100);
			init_params();
			must_survive=80;
			timer=3*60*1000;
			break;
		case 3:
			init_iceland();
			init_characters(100);
			init_params();
			must_survive=90;
			timer=4*60*1000;
			break;
		case 4:
			init_city();
			init_characters(100);
			init_params();
			must_survive=95;
			timer=5*60*1000;
			break;
	}

	return true;
}

void shLevel::init_grassland()
{
	// Material
	ldMaterial	*material;

	material=GFX->new_material();
	material->set_texture(0,get_texture("Textures/Grass.png"));
	material->set_diffuse_color(1.0f,1.0f,1.0f,1.0f);
	material->set_color_blend_op(0,LDBLEND_MODULATE,LDBLENDARG_TEXTURE,LDBLENDARG_DIFFUSE);
	material->set_alpha_blend_op(0,LDBLEND_DISABLE,LDBLENDARG_DIFFUSE,LDBLENDARG_DIFFUSE);

	add_to_materials(material);

	// Mesh
	shGeoFactory	factory;

	ldMesh *landscape=factory.landscape(1200.0f,40.0f);

	add_to_meshes(landscape);

	// Object
	ldDrawObj3D	*landmass=new ldDrawObj3D;

	landmass->set_material(material);
	landmass->set_mesh(landscape);

	add_to_scenario_objects(landmass);
}

void shLevel::init_desert()
{
	// Material
	ldMaterial	*material;

	material=GFX->new_material();
	material->set_texture(0,get_texture("Textures/Sand.png"));
	material->set_diffuse_color(1.0f,1.0f,1.0f,1.0f);
	material->set_color_blend_op(0,LDBLEND_MODULATE,LDBLENDARG_TEXTURE,LDBLENDARG_DIFFUSE);
	material->set_alpha_blend_op(0,LDBLEND_DISABLE,LDBLENDARG_DIFFUSE,LDBLENDARG_DIFFUSE);

	add_to_materials(material);

	// Mesh
	shGeoFactory	factory;

	ldMesh *landscape=factory.landscape(1200.0f,20.0f);

	add_to_meshes(landscape);

	// Object
	ldDrawObj3D	*landmass=new ldDrawObj3D;

	landmass->set_material(material);
	landmass->set_mesh(landscape);

	add_to_scenario_objects(landmass);
}

void shLevel::init_iceland()
{
	// Material
	ldMaterial	*material;

	material=GFX->new_material();
	material->set_texture(0,get_texture("Textures/Ice.png"));
	material->set_diffuse_color(1.0f,1.0f,1.0f,1.0f);
	material->set_color_blend_op(0,LDBLEND_MODULATE2X,LDBLENDARG_TEXTURE,LDBLENDARG_DIFFUSE);
	material->set_alpha_blend_op(0,LDBLEND_DISABLE,LDBLENDARG_DIFFUSE,LDBLENDARG_DIFFUSE);

	add_to_materials(material);

	// Mesh
	shGeoFactory	factory;

	ldMesh *landscape=factory.landscape(1200.0f,60.0f);

	add_to_meshes(landscape);

	// Object
	ldDrawObj3D	*landmass=new ldDrawObj3D;

	landmass->set_material(material);
	landmass->set_mesh(landscape);

	add_to_scenario_objects(landmass);
}

void shLevel::init_city()
{
	// Material
	ldMaterial	*material;

	material=GFX->new_material();
	material->set_texture(0,get_texture("Textures/Roads.png"));
	material->set_diffuse_color(1.0f,1.0f,1.0f,1.0f);
	material->set_color_blend_op(0,LDBLEND_MODULATE2X,LDBLENDARG_TEXTURE,LDBLENDARG_DIFFUSE);
	material->set_alpha_blend_op(0,LDBLEND_DISABLE,LDBLENDARG_DIFFUSE,LDBLENDARG_DIFFUSE);

	add_to_materials(material);

	// Mesh
	shGeoFactory	factory;

	ldMesh *landscape=factory.landscape(1200.0f,5.0f);

	add_to_meshes(landscape);

	// Object
	ldDrawObj3D	*landmass=new ldDrawObj3D;

	landmass->set_material(material);
	landmass->set_mesh(landscape);

	add_to_scenario_objects(landmass);
}

void shLevel::add_to_materials(ldMaterial *material)
{
	materials.push_back(material);
}

void shLevel::add_to_meshes(ldMesh *mesh)
{
	meshes.push_back(mesh);
}

void shLevel::add_to_scenario_objects(ldDrawObj3D *obj)
{
	scenario_objects.push_back(obj);
}

void shLevel::add_to_characters(shVictim *obj)
{
	characters.push_back(obj);
}

void shLevel::add_to_enemies(shEnemy *obj)
{
	enemies.push_back(obj);
}

void shLevel::clear()
{
	for (unsigned long i=0; i<materials.size(); i++)
	{
		if (materials[i]) delete materials[i];
	}
	materials.clear();

	for (i=0; i<meshes.size(); i++)
	{
		if (meshes[i]) delete meshes[i];
	}
	meshes.clear();

	for (i=0; i<scenario_objects.size(); i++)
	{
		if (scenario_objects[i]) delete scenario_objects[i];
	}
	scenario_objects.clear();

	for (i=0; i<characters.size(); i++)
	{
		if (characters[i]) delete characters[i];
	}
	characters.clear();

	for (i=0; i<particles.size(); i++)
	{
		if (particles[i]) delete particles[i];
	}
	particles.clear();
	ps_infos.clear();

	for (i=0; i<enemies.size(); i++)
	{
		if (enemies[i]) delete enemies[i];
	}
	enemies.clear();
}

void shLevel::draw()
{
	for (unsigned long i=0; i<scenario_objects.size(); i++)
	{
		scenario_objects[i]->draw();
	}
	for (i=0; i<characters.size(); i++)
	{
		if (characters[i]) characters[i]->draw();
	}
	for (i=0; i<enemies.size(); i++)
	{
		if (enemies[i]) enemies[i]->draw();
	}
	
	// Sort particle systems in respect to camera distance
/*	ldParticleSystems	temp;
	shPSInfos			temp2;

	for (i=0; i<particles.size(); i++)
	{
		// Calc distances
		if (particles[i]) 
		{
			Vec3f	vec=particles[i]->pos;
			vec=vec-camera->pos;
			float d=vec.norm_squared();
			
			if (temp.size()>0)
			{
				bool ok=false;
				ldParticleSystems::iterator	j;
				shPSInfos::iterator			k=temp2.begin();
				for (j=temp.begin(); j!=temp.end(); j++);
				{
					if ((*k).d<d)
					{
						j=temp.insert(j,particles[i]);
						shPSInfo	inf=*k;
						inf.d=d;
						k=temp2.insert(k,inf);
						ok=true;
						break;
					}
					k++;
				}
				if (!ok)
				{
					temp.push_back(particles[i]);
					temp2.push_back(ps_infos[i]);
				}
			}
			else
			{
				temp.push_back(particles[i]);
				temp2.push_back(ps_infos[i]);
			}
		}
	}
	for (i=0; i<temp.size(); i++)
	{
		if (temp[i]) temp[i]->draw();
	}

	temp.clear();
	temp2.clear();*/

	for (i=0; i<particles.size(); i++)
	{
		if (particles[i]) particles[i]->draw();
	}
}	

void shLevel::init_characters(long n)
{
	for (unsigned long i=0; i<n; i++)
	{
		// Create creatures
		long id=rand() % MAX_GAME_CHARS_MODELS;
		float x=get_random_number(-450.0f,450.0f);
		float z=get_random_number(-450.0f,450.0f);
		float y=cast_ray_down_scenario(x,200.0f,z);
		
		// Object
		shVictim	*character=new shVictim;

		character->set_mesh(game_char[id]);
		character->pos.set(x,y,z);
		character->add_bounding_sphere(0.0f,0.0f,0.0f,5.0f);
		
		add_to_characters(character);
	}
}

float shLevel::cast_ray_down_scenario(float x,float y,float z)
{
	for (unsigned long i=0; i<scenario_objects.size(); i++)
	{
		ldDrawObj3D *tmp=scenario_objects[i];
		Vec3f		origin(x,y,z);
		Vec3f		dir(0.0f,-1.0f,0.0f);
		Vec3f		pt;

		if (tmp->is_perfect_intersection(&origin,&dir,NULL,&pt))
		{
			return pt.y;
		}
	}

	return 0.0f;
}

void shLevel::anim(unsigned long t)
{
	if (timer<t) timer=0;
	else timer-=t;

	for (unsigned long i=0; i<characters.size(); i++)
	{
		if (characters[i])
		{
			Vec3f	accel(0.0f,0.0f,0.0f);
			
			// Select random escape path
			float a=get_random_number(0.0f,PI_TIMES_2);
			
			accel.set(cos(a),0.0f,sin(a));

			// Stay away from the borders of the map
			Vec3f	vec=characters[i]->pos;
			float	d=vec.norm_squared();
			if (d>176000.0f)
			{
				vec=-vec;
				d=d-176000.0f; d=sqrt(d);
				vec=vec*d;
				accel=accel+vec;
			}

			// Finally, calc the acceleration
			accel.normalize();

			vec.set(accel.x,0.0f,accel.z);

			characters[i]->accel=vec;
		}
	}

	for (i=0; i<characters.size(); i++)
	{
		if (characters[i]) characters[i]->update(t,this);
	}

	// Spawn enemies
	if (time_to_next_enemy<=t)
	{
		time_to_next_enemy=unsigned long(get_random_number(10000.0f/(level*0.5f),15000.0f/(level*0.5f)));
		spawn();
	}
	else
	{
		time_to_next_enemy-=t;
	}

	for (i=0; i<enemies.size(); i++)
	{
		if (enemies[i]) enemies[i]->anim(t,this);
	}

	for (i=0; i<particles.size(); i++)
	{
		if (particles[i]) 
		{
			particles[i]->anim(t);
			if (!particles[i]->is_alive())
			{
				delete particles[i];
				particles[i]=NULL;
			}
			else
			{
				if (ps_infos[i].destroyer)
				{
					unsigned long zt=timeGetTime()-ps_infos[i].start_time;
					if (zt<4000) particles[i]->change_bounding_sphere_radius(10.0f*zt/1000.0f);
				}
				if (ps_infos[i].power)
				{
					ldParticleSystem *part=particles[i];
					shPSInfo		*info=&(ps_infos[i]);
					particles[i]->pos=particles[i]->pos+(ps_infos[i].dir*t*0.05f);
				}
			}
		}
	}

	// Test for killings
	for (i=0; i<particles.size(); i++)
	{
		if (particles[i]) 
		{
			if (ps_infos[i].destroyer)
			{
				for (unsigned long j=0; j<characters.size(); j++)
				{
					shVictim *tmp=characters[j];
					
					if (tmp)
					{
						if (tmp->active)
						{
							if (tmp->is_colliding(particles[i]))
							{
								// Victim dies an horrible death
								tmp->time_to_die=5000.0f;
								die(tmp);
								tmp->active=false;
								((shEnemy*)ps_infos[i].spawner)->idle=7500/level;
								SND->play(sounds[rand()%3]);
							}
						}
					}
				}
			}
			else if (ps_infos[i].power)
			{
				for (unsigned long j=0; j<enemies.size(); j++)
				{
					shEnemy *tmp=enemies[j];
					
					if (tmp)
					{
						if (tmp->time_to_die==0)
						{
							if (tmp->is_colliding(particles[i]))
							{
								// Enemy dies an horrible death
								tmp->time_to_die=5000.0f;
								tmp->rise=-5000;
								die_enemy(tmp);
								tmp->idle=10000;
							}
						}
					}
				}
			}
		}
	}

}

void shLevel::init_params()
{
	time_to_next_enemy=unsigned long(get_random_number(1000.0f,1500.0f));
}

void shLevel::spawn()
{
	float x=get_random_number(-450.0f,450.0f);
	float z=get_random_number(-450.0f,450.0f);
	float y=cast_ray_down_scenario(x,200.0f,z);

	// The particle system
	ldParticleSystem *particle_system=GFX->new_particle_system();
	particle_system->set_emitter_function(part_func_vertical_point_emitter);
	particle_system->set_life(10000);
	particle_system->set_rate(1000);
	particle_system->set_initial_life(4000,2000);
	particle_system->set_initial_speed(2.0f,0.5f);
	particle_system->set_life_cycle_color(0.0f,1.0f,1.0f,1.0f,1.0f);
	particle_system->set_life_cycle_color(0.1f,1.0f,1.0f,0.0f,0.9f);
	particle_system->set_life_cycle_color(0.5f,1.0f,0.0f,0.0f,0.5f);
	particle_system->set_life_cycle_color(1.0f,0.0f,0.0f,0.0f,0.0f);
	particle_system->set_particle_size(0.0f,4.0f);
	particle_system->set_texture(get_texture("Textures/Particle.png"));
	particle_system->set_blend_mode(LDBLENDMODE_ADD);
	
	particle_system->pos.set(x,y,z);

	particles.push_back(particle_system);
	
	shPSInfo info;
	info.start_time=timeGetTime();
	info.destroyer=false;
	info.power=false;
	ps_infos.push_back(info);

	// The enemy itself
	shEnemy	*enemy=new shEnemy;

	unsigned long id=rand() % MAX_ENEMIES_MODELS;
	enemy->set_mesh(enemy_model[id]);
	enemy->idle=10000;
	enemy->no_draw=5000;
	enemy->rise=5000;
	enemy->pos.set(x,y-10.0f,z);
	enemy->add_bounding_sphere(0.0f,5.0f,0.0f,4.0f);

	add_to_enemies(enemy);

	SND->play(sounds[5]);
}

void shLevel::die(shVictim *victim)
{
	// The particle system
	ldParticleSystem *particle_system=GFX->new_particle_system();
	particle_system->set_emitter_function(part_func_death_emitter);
	particle_system->set_life(5000);
	particle_system->set_rate(500);
	particle_system->set_initial_life(2500,1000);
	particle_system->set_initial_speed(2.0f,1.0f);
	particle_system->set_life_cycle_color(0.0f,1.0f,1.0f,1.0f,1.0f);
	particle_system->set_life_cycle_color(0.2f,1.0f,1.0f,0.0f,0.8f);
	particle_system->set_life_cycle_color(0.35f,1.0f,0.0f,0.0f,0.6f);
	particle_system->set_life_cycle_color(0.55f,1.0f,0.0f,1.0f,0.4f);
	particle_system->set_life_cycle_color(0.70f,0.0f,0.0f,1.0f,0.3f);
	particle_system->set_life_cycle_color(0.8f,0.0f,1.0f,1.0f,0.2f);
	particle_system->set_life_cycle_color(1.0f,0.0f,0.0f,0.0f,0.0f);
	particle_system->set_particle_size(0.0f,4.0f);
	particle_system->set_texture(get_texture("Textures/Particle.png"));
	particle_system->set_blend_mode(LDBLENDMODE_ADD);
	
	particle_system->pos=victim->pos;

	particles.push_back(particle_system);
	
	shPSInfo info;
	info.start_time=timeGetTime();
	info.destroyer=false;
	info.power=false;
	ps_infos.push_back(info);
}

void shLevel::draw_map(ldDrawObj3D *obj)
{
	// Map background
	Vec2f	pos(800.0f-128.0f,0.0f);
	Vec4f	color(1.0f,1.0f,1.0f,0.75f);
	GFX->draw_sprite(&pos,128.0f,128.0f,&color,0.0f,0.0f,1.0f,1.0f,get_texture("Textures/map_background.png"));

	// Enemies
	for (unsigned long i=0; i<enemies.size(); i++)
	{
		if (enemies[i])
		{
			pos.set(enemies[i]->pos.x,enemies[i]->pos.z);
			pos.x+=600.0f; pos.y+=600.0f;
			pos.x=pos.x/1200.0f; pos.y=pos.y/1200.0f;
			pos.x=pos.x*128.0f+(800.0f-128.0f)-4.0f; pos.y=pos.y*128.0f-4.0f;
			GFX->draw_sprite(&pos,8.0f,8.0f,&color,0.0f,0.0f,1.0f,1.0f,get_texture("Textures/map_enemy.png"));
		}
	}

	if (obj)
	{
		pos.set(obj->pos.x,obj->pos.z);
		pos.x+=600.0f; pos.y+=600.0f;
		pos.x=pos.x/1200.0f; pos.y=pos.y/1200.0f;
		pos.x=pos.x*128.0f+(800.0f-128.0f)-4.0f; pos.y=pos.y*128.0f-4.0f;
		GFX->draw_sprite(&pos,8.0f,8.0f,&color,0.0f,0.0f,1.0f,1.0f,get_texture("Textures/map_supahero.png"));
	}
}

void shLevel::die_enemy(shEnemy *enemy)
{
	// The particle system
	ldParticleSystem *particle_system=GFX->new_particle_system();
	particle_system->set_emitter_function(part_func_vertical_square_emitter);
	particle_system->set_life(5000);
	particle_system->set_rate(1000);
	particle_system->set_initial_life(2500,1000);
	particle_system->set_initial_speed(2.0f,1.0f);
	particle_system->set_life_cycle_color(0.0f,0.0f,0.0f,0.5f,1.0f);
	particle_system->set_life_cycle_color(1.0f,0.0f,0.0f,0.0f,0.0f);
	particle_system->set_particle_size(0.0f,4.0f);
	particle_system->set_texture(get_texture("Textures/Particle.png"));
	particle_system->set_blend_mode(LDBLENDMODE_ADD);
	
	particle_system->pos=enemy->pos;

	particles.push_back(particle_system);
	
	shPSInfo info;
	info.start_time=timeGetTime();
	info.destroyer=false;
	info.power=false;
	ps_infos.push_back(info);

	SND->play(sounds[7]);
}

long shLevel::get_victims_alive()
{
	long counter=0;

	for (unsigned long i=0; i<characters.size(); i++)
	{
		if (characters[i]) 
		{
			if (characters[i]->active) counter++;
		}
	}

	return counter;
}
