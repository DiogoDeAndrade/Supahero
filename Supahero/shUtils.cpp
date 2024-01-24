
#include "../LudiEngine/Headers/ldTexture.h"
#include "../LudiEngine/Headers/ldGfx.h"
#include "../LudiEngine/Headers/ldUtils.h"
#include "shGeoFactory.h"
#include "shUtils.h"

ldMesh *game_char[MAX_GAME_CHARS_MODELS];
ldMesh *enemy_model[MAX_ENEMIES_MODELS];
ldMesh *supa_model[MAX_SUPA_MODELS];

ldSoundID sounds[MAX_SOUNDS];

bool load_textures()
{
	ldTexture	*texture;

	for (unsigned long i=0; i<MAX_HEADS; i++)
	{
		texture=GFX->new_texture();
		char buffer[512];
		sprintf((char*)&buffer,"Textures/Head%02li.png",i+1);
		if (!texture->load((char*)&buffer)) return false;
	}

	for (i=0; i<MAX_BODIES; i++)
	{
		texture=GFX->new_texture();
		char buffer[512];
		sprintf((char*)&buffer,"Textures/Body%02li.png",i+1);
		if (!texture->load((char*)&buffer)) return false;
	}

	texture=GFX->new_texture();
	if (!texture->load("Textures/Grass.png")) return false;
	texture->set_wrap(true,true);

	texture=GFX->new_texture();
	if (!texture->load("Textures/Sand.png")) return false;
	texture->set_wrap(true,true);

	texture=GFX->new_texture();
	if (!texture->load("Textures/Ice.png")) return false;
	texture->set_wrap(true,true);

	texture=GFX->new_texture();
	if (!texture->load("Textures/Roads.png")) return false;
	texture->set_wrap(true,true);

	texture=GFX->new_texture();
	if (!texture->load("Textures/Particle.png")) return false;
	texture->set_wrap(true,true);

	texture=GFX->new_texture();
	if (!texture->load("Textures/map_background.png")) return false;

	texture=GFX->new_texture();
	if (!texture->load("Textures/map_enemy.png")) return false;

	texture=GFX->new_texture();
	if (!texture->load("Textures/map_supahero.png")) return false;

	texture=GFX->new_texture();
	if (!texture->load("Textures/supa_head.png")) return false;

	texture=GFX->new_texture();
	if (!texture->load("Textures/supa_body.png")) return false;

	return true;
}

bool create_models()
{
	shGeoFactory	factory;

	for (unsigned long i=0; i<MAX_HEADS; i++)
	{
		for (unsigned long j=0; j<MAX_BODIES; j++)
		{
			float	size=get_random_number(1.5f,2.0f);
			game_char[i*MAX_BODIES+j]=factory.game_char(size,i+1,j+1);
		}
	}

	for (i=0; i<MAX_ENEMIES_MODELS; i++)
	{
		enemy_model[i]=factory.enemy(i);
	}

	// Supahero!
	for (i=0; i<MAX_SUPA_MODELS; i++)
	{
		supa_model[i]=factory.supa(i);
	}

	return true;
}

void delete_models()
{
	for (unsigned long i=0; i<MAX_GAME_CHARS_MODELS; i++)
	{
		if (game_char[i]) delete game_char[i];
		game_char[i]=NULL;
	}
	for (i=0; i<MAX_ENEMIES_MODELS; i++)
	{
		if (enemy_model[i]) delete enemy_model[i];
		enemy_model[i]=NULL;
	}
	for (i=0; i<MAX_SUPA_MODELS; i++)
	{
		if (supa_model[i]) delete supa_model[i];
		supa_model[i]=NULL;
	}
}

bool load_sounds()
{
	sounds[0]=SND->load(LDSOUND_MP3,"Sounds/CharDying01.mp3",false,false);
	if (sounds[0]==LDSOUND_ERROR) return false;
	sounds[1]=SND->load(LDSOUND_MP3,"Sounds/CharDying02.mp3",false,false);
	if (sounds[1]==LDSOUND_ERROR) return false;
	sounds[2]=SND->load(LDSOUND_MP3,"Sounds/CharDying03.mp3",false,false);
	if (sounds[2]==LDSOUND_ERROR) return false;
	sounds[3]=SND->load(LDSOUND_MP3,"Sounds/CharScreaming01.mp3",false,false);
	if (sounds[3]==LDSOUND_ERROR) return false;
	sounds[4]=SND->load(LDSOUND_MP3,"Sounds/CharScreaming02.mp3",false,false);
	if (sounds[4]==LDSOUND_ERROR) return false;
	sounds[5]=SND->load(LDSOUND_MP3,"Sounds/EnemiesRising.mp3",false,false);
	if (sounds[5]==LDSOUND_ERROR) return false;
	sounds[6]=SND->load(LDSOUND_MP3,"Sounds/EnemiesFiring.mp3",false,false);
	if (sounds[6]==LDSOUND_ERROR) return false;
	sounds[7]=SND->load(LDSOUND_MP3,"Sounds/EnemiesDying.mp3",false,false);
	if (sounds[7]==LDSOUND_ERROR) return false;
	sounds[8]=SND->load(LDSOUND_MP3,"Sounds/Music.mp3",true,true);
	if (sounds[8]==LDSOUND_ERROR) return false;
	sounds[9]=SND->load(LDSOUND_MP3,"Sounds/PlayerFiring.mp3",false,false);
	if (sounds[9]==LDSOUND_ERROR) return false;
	sounds[10]=SND->load(LDSOUND_MP3,"Sounds/PlayerDying.mp3",false,false);
	if (sounds[10]==LDSOUND_ERROR) return false;

	return true;
}
