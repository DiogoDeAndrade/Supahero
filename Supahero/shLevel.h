// shLevel.h: interface for the shLevel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHLEVEL_H__821A859C_A723_4C92_8E62_748B2636DB43__INCLUDED_)
#define AFX_SHLEVEL_H__821A859C_A723_4C92_8E62_748B2636DB43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../LudiEngine/Headers/ldMaterial.h"
#include "../LudiEngine/Headers/ldMesh.h"
#include "../LudiEngine/Headers/ldDrawObj3D.h"
#include "../LudiEngine/Headers/ldParticleSystem.h"

#include "shVictims.h"
#include "shEnemy.h"

#include <vector>
typedef std::vector<ldMaterial*>		ldMaterials;
typedef std::vector<ldMesh*>			ldMeshes;
typedef std::vector<ldDrawObj3D*>		ldObjects;
typedef std::vector<ldParticleSystem*>	ldParticleSystems;

struct shPSInfo {
	unsigned long	start_time;
	bool			destroyer;
	bool			power;
	Vec3f			dir;
	ldDrawObj3D		*spawner;
	float			d;
};

typedef std::vector<shPSInfo>	shPSInfos;

class shLevel  
{
public:
	shLevel();
	virtual ~shLevel();

	bool	init_enviroment(unsigned long lvl);
	void	anim(unsigned long t);
	void	draw();
	void	clear();

public:
	inline	long	get_level() { return level; }

			long	get_victims_alive();

protected:
	void	init_grassland();
	void	init_desert();
	void	init_iceland();
	void	init_city();
	void	init_characters(long n);
	void	init_params();

protected:
	void	add_to_materials(ldMaterial *material);
	void	add_to_meshes(ldMesh *mesh);
	void	add_to_scenario_objects(ldDrawObj3D *obj);
	void	add_to_characters(shVictim *obj);
	void	add_to_enemies(shEnemy *obj);

public:
	float	cast_ray_down_scenario(float x,float y,float z);
	void	spawn();
	void	die(shVictim *victim);
	void	die_enemy(shEnemy *enemy);
	void	draw_map(ldDrawObj3D *obj);

public:
	ldMaterials			materials;
	ldMeshes			meshes;
	ldObjects			scenario_objects;
	shVictims			characters;
	ldParticleSystems	particles;
	shEnemies			enemies;
	long				level;
	unsigned long		time_to_next_enemy;
	shPSInfos			ps_infos;
	long				must_survive;
	unsigned long		timer;
};

#endif // !defined(AFX_SHLEVEL_H__821A859C_A723_4C92_8E62_748B2636DB43__INCLUDED_)
