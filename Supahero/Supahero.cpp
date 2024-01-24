
#include <windows.h>

// Init stuff
#include "../ludiEngine/Headers/ldLogger.h"
#include "../ludiEngine/Headers/ldGfxD3D.h"
#include "../ludiEngine/Headers/ldKeyboardDI.h"
#include "../ludiEngine/Headers/ldMouseDI.h"
#include "../ludiEngine/Headers/ldSoundFMod.h"
#include "../ludiEngine/Headers/LudiEngine.h"

// Standart stuff
#include "../ludiEngine/Headers/ldCamera3D.h"
#include "../ludiEngine/Headers/ldLight3D.h"
#include "../ludiEngine/Headers/ldFont.h"
#include "../ludiEngine/Headers/ldTexture.h"
#include "../ludiEngine/Headers/ldUtils.h"

// More specific stuff
#include "../ludiEngine/Headers/ldDrawObj3D.h"

// Game stuff
#include "shGeoFactory.h"
#include "shUtils.h"
#include "shLevel.h"
#include "shGameOptions.h"
#include "shSupahero.h"

ldCamera3D	*camera;
shSupahero	*supahero;
ldLight3D	*light;
ldFont		*font;

int Init(HINSTANCE hInstance)
{
	///////////////////////////////////////////////////////////////////////////////////////
	// Init graphic modules
	///////////////////////////////////////////////////////////////////////////////////////
	new ldGfxD3D;

	if (!GFX->init(800,600,32,false,"Supahero!",&hInstance))
		return -1;

	///////////////////////////////////////////////////////////////////////////////////////
	// Init input devices
	///////////////////////////////////////////////////////////////////////////////////////
	new ldKeyboardDI;
	new ldMouseDI;

	KEYBOARD->init(&hInstance,&(((ldGfxD3D*)GFX)->_mainWindow));
	MOUSE->init(&hInstance,&(((ldGfxD3D*)GFX)->_mainWindow));

	///////////////////////////////////////////////////////////////////////////////////////
	// Init sound
	///////////////////////////////////////////////////////////////////////////////////////
	new ldSoundFMOD;

	SND->init(0,0);

	///////////////////////////////////////////////////////////////////////////////////////
	// Init light
	///////////////////////////////////////////////////////////////////////////////////////
	light=GFX->new_light();
	light->set_diffuse_color(1.0f,1.0f,1.0f,1.0f);
	light->fm_look_down(30.0f);
	GFX->add_light(light);

	///////////////////////////////////////////////////////////////////////////////////////
	// Init font
	///////////////////////////////////////////////////////////////////////////////////////
	font=GFX->new_font();
	font->load("Comic Sans MS",20);

	///////////////////////////////////////////////////////////////////////////////////////
	// Init sounds
	///////////////////////////////////////////////////////////////////////////////////////
	if (!load_sounds())
	{
		return -1;
	}

	///////////////////////////////////////////////////////////////////////////////////////
	// Init textures
	///////////////////////////////////////////////////////////////////////////////////////
	if (!load_textures())
	{
		return -1;
	}

	///////////////////////////////////////////////////////////////////////////////////////
	// Init models
	///////////////////////////////////////////////////////////////////////////////////////
	if (!create_models())
	{
		return -1;
	}

	// Play music
	SND->play(sounds[8]);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// General stuff
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
typedef enum { SUPA_NONE,SUPA_INTRO, SUPA_MAIN_MENU, SUPA_GAME_OVER, SUPA_EXIT,SUPA_GAME, SUPA_NEXT_LEVEL } SupaState;

shGameOptions	options;
SupaState		current_state=SUPA_INTRO;
float			fps=0.0f;

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Single screen
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
ldTexture	*single_screen;
long		fade_in=0;
long		fade_out=0;
long		timeout=-1;
long		counter=0;
long		fade_time=0;

void single_screen_loop_function(unsigned long t)
{
	if (counter>=0) counter+=t;

	// Animation
	KEYBOARD->poll(t);
	MOUSE->poll(t);
	
	if (KEYBOARD->is_any_key_pressed())
	{
		if (KEYBOARD->esc_pressed()) 
		{
			LD_set_exit(true);
			current_state=SUPA_EXIT;
		}
		else
		{
			if ((fade_out==0) && (fade_in==0))
			{
				current_state=SUPA_NONE;
				fade_out=fade_time;
				fade_in=0;
				counter=0;
			}
		}
	}
	if ((MOUSE->button_pressed(0)) || (MOUSE->button_pressed(1)))
	{
		if ((fade_out==0) && (fade_in==0))
		{
			current_state=SUPA_NONE;
			fade_out=fade_time;
			fade_in=0;
			counter=0;
		}
	}

	// Rendering
	GFX->clear(LD_FRAME_BUFFER | LD_Z_BUFFER,0.0f,0.0f,0.0f,1.0f,1.0f);

	GFX->begin_render();

	Vec2f	tmp(0.0f,0.0f);
	Vec4f	col;
	if (fade_in>0)
	{
		col.set(1.0f,1.0f,1.0f,(counter>0)?(float(counter)/float(fade_in)):(1.0f));
		if (counter>=fade_in) fade_in=0;
	} else
	if (fade_out>0)
	{
		col.set(1.0f,1.0f,1.0f,1.0f-((counter>=0)?(float(counter)/float(fade_out)):(1.0f)));
		if (counter>=fade_out) 
		{
			LD_set_exit(true);
			current_state=SUPA_NONE;
			fade_out=0;
		}
	} else
	{
		col.set(1.0f,1.0f,1.0f,1.0f);
	}
	GFX->draw_sprite(&tmp,800,600,&col,0.0f,0.0f,1.0f,1.0f,single_screen);

	GFX->end_render();

	GFX->swap_buffers();
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// Game stuff
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
shLevel *current_level=NULL;
float	zoom=80.0f;

void init_level(int level)
{
	// Initialize the levels
	current_level=new shLevel;
	current_level->init_enviroment(level);

	supahero=new shSupahero;
	supahero->pos.set(0.0f,0.0f,0.0f);

	///////////////////////////////////////////////////////////////////////////////////////
	// Init camera
	///////////////////////////////////////////////////////////////////////////////////////
	camera=new ldCamera3D;
	camera->set_far_plane(5000.0f);
	camera->fm_up(80.0f);
	camera->fm_back(80.0f);
	camera->fpm_look_down(-45.0f);
	camera->add_bounding_sphere(new ldBoundingSphere(1.0f));
}

void game_loop_function(unsigned long t)
{
	unsigned long startTime=timeGetTime();

	float r=get_random_number(0.0f,1.0f);

	if (r<(0.0001f*t)) SND->play(sounds[3+rand()%2]);

	// Animation
	KEYBOARD->poll(t);
	MOUSE->poll(t);

	supahero->vec.set(0.0f,0.0f,0.0f);

	if (KEYBOARD->is_any_key_pressed())
	{
		if (KEYBOARD->f_key_pressed(3)) GFX->set_render_mode(LDRENDERMODE_WIREFRAME);
		if (KEYBOARD->f_key_pressed(4)) GFX->set_render_mode(LDRENDERMODE_SOLID);

		if (KEYBOARD->esc_pressed()) 
		{
			current_state=SUPA_EXIT;
			LD_set_exit(true);			
		}

		if (KEYBOARD->is_key_pressed('z')) if (zoom<80.0f) zoom+=t*0.1;
		if (KEYBOARD->is_key_pressed('x')) if (zoom>20.0f) zoom-=t*0.1;

		// Move Supahero
		if (KEYBOARD->up_pressed()) if (supahero->pos.z>-450.0f) supahero->vec.z=-1.0f;;
		if (KEYBOARD->down_pressed()) if (supahero->pos.z<450.0f) supahero->vec.z=1.0f;
		if (KEYBOARD->left_pressed()) if (supahero->pos.x>-420.0f) supahero->vec.x=-1.0f;
		if (KEYBOARD->right_pressed()) if (supahero->pos.x<420.0f) supahero->vec.x=1.0f;
		if (KEYBOARD->space_pressed()) supahero->fire(current_level);

		if (KEYBOARD->f_key_pressed(1)) SND->stop(sounds[8]);
		if (KEYBOARD->f_key_pressed(2)) SND->stop(sounds[8]);
	}

	// Animate supahero
	supahero->anim(t,current_level);

	// Camera following
	camera->pos.x=supahero->pos.x;
	camera->pos.y=supahero->pos.y+zoom;
	camera->pos.z=supahero->pos.z+zoom;

	current_level->anim(t);

	// Rendering
	GFX->clear(LD_FRAME_BUFFER | LD_Z_BUFFER,1.0f,1.0f,1.0f,1.0f,1.0f);

	GFX->begin_render();

	camera->set();

	supahero->draw();
	current_level->draw();
	current_level->draw_map(supahero);

	GFX->end_render();

	// Get win/loose conditions
	long	victims_alive=current_level->get_victims_alive();

	// Write FPS
//	font->write(0,0,0.0f,0.0f,0.0f,1.0f,"%f FPS",fps);
	// Write FPS
	font->write(0,0,0.0f,0.0f,0.0f,1.0f,"Timer: %02li:%02li",(current_level->timer/60000),(current_level->timer % 60000)/1000);
	font->write(0,20,0.0f,0.0f,0.0f,1.0f,"Energy:%li",supahero->energy);
	font->write(0,40,0.0f,0.0f,0.0f,1.0f,"Panicking idiots:%li",victims_alive);

	GFX->swap_buffers();

	unsigned long endTime=timeGetTime();

	fps=1000.0f/float(endTime-startTime);

	// Check conditions
	if (current_level->timer==0)
	{
		// NEXT LEVEL! WEEEEEEEEEEEEEEEEEEE!
		current_state=SUPA_NEXT_LEVEL;
		LD_set_exit(true);
	}
	if ((supahero->energy<=0) && (supahero->time_to_die==0))
	{
		// DIE! :(
		current_state=SUPA_GAME_OVER;
		LD_set_exit(true);
	}
	if (victims_alive<current_level->must_survive)
	{
		// TOO MANY DEAD PEOPLE! :(
		current_state=SUPA_GAME_OVER;
		LD_set_exit(true);
	}
}

void Shutdown()
{
	delete_all_textures();
	delete_models();
	GFX->shutdown(); delete GFX;
	MOUSE->shutdown(); delete MOUSE;
	KEYBOARD->shutdown(); delete KEYBOARD;
}

// Main Function
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	ldLogger logger("Supahero","Supahero_Log.txt");

	SetCurrentDirectory("GameData/");
	
	// Control the randomness a bit
	srand(0);

	int xpto=Init(hInstance);
	if (xpto!=0) 
	{
		return xpto;
	}

	unsigned long	level=1;
	bool			exit=false;
	single_screen=GFX->new_texture();

	while (!exit)
	{
		if (current_state==SUPA_INTRO)
		{
			// Intro
			single_screen->load("Textures/Intro01.png");
			fade_time=fade_in=2000;
			fade_out=counter=0;
			LD_set_loop_function(single_screen_loop_function);
			LD_loop();
			single_screen->clear();
			single_screen->load("Textures/Intro02.png");
			fade_time=fade_in=2000;
			fade_out=counter=0;
			LD_set_loop_function(single_screen_loop_function);
			LD_loop();
			single_screen->clear();
			if (current_state!=SUPA_EXIT) current_state=SUPA_MAIN_MENU;
		} else
		if (current_state==SUPA_MAIN_MENU)
		{
			// Main menu
			single_screen->load("Textures/Instructions.png");
			fade_time=fade_in=2000;
			fade_out=counter=0;
			LD_set_loop_function(single_screen_loop_function);
			LD_loop();
			single_screen->clear();
			level=1;
			if (current_state!=SUPA_EXIT) current_state=SUPA_GAME;
		} else
		if (current_state==SUPA_GAME_OVER)
		{
			// Game over screen
			single_screen->load("Textures/GameOverScreen.png");
			fade_time=fade_in=2000;
			fade_out=counter=0;
			LD_set_loop_function(single_screen_loop_function);
			LD_loop();
			single_screen->clear();
			if (current_state!=SUPA_EXIT) current_state=SUPA_MAIN_MENU;
		} else
		if (current_state==SUPA_EXIT)
		{
			exit=true;
			break;
		} else
		if (current_state==SUPA_NEXT_LEVEL)
		{
			// Next level screen
			single_screen->load("Textures/NextLevel.png");
			fade_time=fade_in=2000;
			fade_out=counter=0;
			LD_set_loop_function(single_screen_loop_function);
			LD_loop();
			single_screen->clear();
			if (current_state!=SUPA_EXIT) 
			{
				level++;
				current_state=SUPA_GAME;
			}
		} else
		if (current_state==SUPA_GAME)
		{
			// Game
			init_level(level);
			LD_set_loop_function(game_loop_function);
			fps=0.0f;
			LD_loop();
			current_level->clear();
			delete current_level; current_level=NULL;
		}
	}

	Shutdown();

	return 0;
}





