
#include <windows.h>

// Init stuff
#include "../ludiEngine/Headers/ldLogger.h"
#include "../ludiEngine/Headers/ldGfxD3D.h"
#include "../ludiEngine/Headers/ldKeyboardDI.h"
#include "../ludiEngine/Headers/ldMouseDI.h"
#include "../ludiEngine/Headers/LudiEngine.h"

// Standart stuff
#include "../ludiEngine/Headers/ldCamera3D.h"
#include "../ludiEngine/Headers/ldLight3D.h"
#include "../ludiEngine/Headers/ldFont.h"

// More specific stuff
#include "../ludiEngine/Headers/ldDrawObj3D.h"

// Model generator
#include "../Supahero/shGeoFactory.h"
#include "../Supahero/shUtils.h"

ldDrawObj3D	*object=NULL;
ldCamera3D	*camera;
ldLight3D	*light;
ldFont		*font;

int Init(HINSTANCE hInstance)
{
	///////////////////////////////////////////////////////////////////////////////////////
	// Init graphic modules
	///////////////////////////////////////////////////////////////////////////////////////
	new ldGfxD3D;

	if (!GFX->init(800,600,32,true,"LudiEngine Test Framework",&hInstance))
		return -1;

	GFX->set_blend_mode(LDBLENDMODE_NORMAL);
	GFX->set_render_mode(LDRENDERMODE_SOLID);

	///////////////////////////////////////////////////////////////////////////////////////
	// Init input devices
	///////////////////////////////////////////////////////////////////////////////////////
	new ldKeyboardDI;
	new ldMouseDI;

	KEYBOARD->init(&hInstance,&(((ldGfxD3D*)GFX)->_mainWindow));
	MOUSE->init(&hInstance,&(((ldGfxD3D*)GFX)->_mainWindow));

	///////////////////////////////////////////////////////////////////////////////////////
	// Init camera
	///////////////////////////////////////////////////////////////////////////////////////
	camera=new ldCamera3D;
	camera->set_far_plane(5000.0f);
	camera->fm_back(4.0f);
	camera->fm_up(1.0f);
	camera->add_bounding_sphere(new ldBoundingSphere(1.0f));

	///////////////////////////////////////////////////////////////////////////////////////
	// Init light
	///////////////////////////////////////////////////////////////////////////////////////
	light=GFX->new_light();
	light->set_diffuse_color(1.0f,1.0f,1.0f,1.0f);
//	light->fm_look_down(45.0f);
	GFX->add_light(light);

	///////////////////////////////////////////////////////////////////////////////////////
	// Init font
	///////////////////////////////////////////////////////////////////////////////////////
	font=GFX->new_font();
	font->load("Arial",20);

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

	///////////////////////////////////////////////////////////////////////////////////////
	// Init character
	///////////////////////////////////////////////////////////////////////////////////////
	if (!object) object=new ldDrawObj3D;
//	object->set_mesh(game_char[5]);
//	object->set_mesh(enemy_model[0]);
	object->set_mesh(supa_model[0]);

	return 0;
}

long	character=0;
float	fps=0.0f;
unsigned long time_since_last_key=0;

void model_viewer_loop_function(unsigned long t)
{
	unsigned long startTime=timeGetTime();

	// Animation
	KEYBOARD->poll(t);
	MOUSE->poll(t);
	
	if (KEYBOARD->is_any_key_pressed())
	{
		// Move camera
		if (KEYBOARD->up_pressed()) camera->fm_forward(0.01f*t);
		if (KEYBOARD->down_pressed()) camera->fm_back(0.01f*t);
		if (KEYBOARD->left_pressed()) camera->fm_turn_left(0.1f*t);
		if (KEYBOARD->right_pressed()) camera->fm_turn_right(0.1f*t);
		if (KEYBOARD->page_up_pressed()) camera->fm_up(0.01f*t);
		if (KEYBOARD->page_down_pressed()) camera->fm_down(0.01f*t);

		if (object)
		{
			// Move object
			if (KEYBOARD->is_key_pressed('w')) object->no_forward(0.025f*t);
			if (KEYBOARD->is_key_pressed('s')) object->no_back(0.025f*t);
			if (KEYBOARD->is_key_pressed('a')) object->no_sidestep_left(0.025f*t);
			if (KEYBOARD->is_key_pressed('d')) object->no_sidestep_right(0.025f*t);
			if (KEYBOARD->is_key_pressed('r')) object->no_up(0.025f*t);
			if (KEYBOARD->is_key_pressed('f')) object->no_down(0.025f*t);
		}

		// Models
		if (KEYBOARD->is_key_pressed('1'))
		{
			// Mesh
			if ((time_since_last_key==0) || (timeGetTime()-time_since_last_key>500))
			{
				if (character>0) character--;
				if (!object) object=new ldDrawObj3D;
				object->set_mesh(game_char[character]);
//				object->set_mesh(enemy_model[character]);
				time_since_last_key=timeGetTime();
			}
		}
		if (KEYBOARD->is_key_pressed('2'))
		{
			// Mesh
			if ((time_since_last_key==0) || (timeGetTime()-time_since_last_key>500))
			{
				if (character<MAX_GAME_CHARS_MODELS-1) character++;
				if (!object) object=new ldDrawObj3D;
				object->set_mesh(game_char[character]);
//				object->set_mesh(enemy_model[character]);
				time_since_last_key=timeGetTime();
			}
		}

		if (KEYBOARD->f_key_pressed(3)) GFX->set_render_mode(LDRENDERMODE_WIREFRAME);
		if (KEYBOARD->f_key_pressed(4)) GFX->set_render_mode(LDRENDERMODE_SOLID);

		if (KEYBOARD->f_key_pressed(12)) 
		{
			LD_set_exit(true);			
		}
	}

	if (object)
	{
		// Rotate object
		object->fm_turn_right(0.025f*t*MOUSE->get_delta_x());
		object->fm_look_up(0.025f*t*(-MOUSE->get_delta_y()));
	}
	
	// Rendering
	GFX->clear(LD_FRAME_BUFFER | LD_Z_BUFFER,1.0f,1.0f,1.0f,1.0f,1.0f);

	GFX->begin_render();

	camera->set();

	// Draw terrain
	if (object) object->draw();

	GFX->end_render();

	// Write FPS
	font->write(0,0,0.0f,0.0f,0.0f,1.0f,"%f FPS",fps);
	// Write Poly count
	if (object)
	{
		font->write(0,20,0.0f,0.0f,0.0f,1.0f,"%li vertex,%li indexes,%li triangles",object->get_mesh()->get_vertex_count(),object->get_mesh()->get_index_count(),object->get_mesh()->get_triangle_count());
	}

	GFX->swap_buffers();

	unsigned long endTime=timeGetTime();

	fps=1000.0f/float(endTime-startTime);
}

void Shutdown()
{
	if (object) delete object;
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
	ldLogger logger("ModelViewer","ModelViewer.txt");

	SetCurrentDirectory("../GameData/");
	
	// Control the randomness a bit
	srand(0);

	int xpto=Init(hInstance);
	if (xpto!=0) 
	{
		return xpto;
	}

	LD_set_loop_function(model_viewer_loop_function);
	fps=0.0f;
	LD_loop();

	Shutdown();

	return 0;
}





