
#ifndef __STONEUTILS
#define __STONEUTILS

#include "../LudiEngine/Headers/ldSound.h"

#define MAX_HEADS				4
#define MAX_BODIES				4
#define MAX_GAME_CHARS_MODELS	MAX_HEADS*MAX_BODIES
#define MAX_ENEMIES_MODELS		4
#define MAX_SUPA_MODELS			1
#define MAX_SOUNDS				11

extern ldMesh *game_char[MAX_GAME_CHARS_MODELS];
extern ldMesh *enemy_model[MAX_ENEMIES_MODELS];
extern ldMesh *supa_model[MAX_SUPA_MODELS];
extern ldSoundID sounds[MAX_SOUNDS];

bool load_textures();
bool create_models();
void delete_models();
bool load_sounds();

#endif