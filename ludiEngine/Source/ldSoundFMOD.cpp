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
// ldSoundFMOD.cpp: implementation of the ldSoundFMOD class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldSoundFMOD.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldSoundFMOD::ldSoundFMOD()
{

}

ldSoundFMOD::~ldSoundFMOD()
{
#ifndef _NO_FMOD
	shutdown();
#endif
}

bool ldSoundFMOD::init(long playback_rate,long max_channels)
{
#ifndef _NO_FMOD
	unsigned long pr;
	unsigned long mc;

	if (playback_rate==0) pr=44100;
	else pr=playback_rate;

	if (max_channels==0) mc=32;
	else mc=max_channels;

	if (FSOUND_Init(pr,mc,0)) 
	{
		can_do_sound=true;
		return true;
	}
	
	can_do_sound=false;
#endif
	return false;
}

void ldSoundFMOD::shutdown()
{
#ifndef _NO_FMOD
	if (can_do_sound) FSOUND_Close();

	clear_all();
#endif
}

ldSoundID ldSoundFMOD::load(ldSoundType type,char *filename,bool loop,bool stream)
{
#ifndef _NO_FMOD
	if (!can_do_sound) return LDSOUND_ERROR;

	ldSoundArrayElem	elem;

	switch (type)
	{
		case LDSOUND_MOD:
		case LDSOUND_S3M:
		case LDSOUND_XM:
		case LDSOUND_IT:
		case LDSOUND_MID:
			elem.type=type;
			elem.streaming=false;
			elem.handle.mod=FMUSIC_LoadSong(filename);
			if (!elem.handle.mod) return LDSOUND_ERROR;
			
			_sounds.push_back(elem);
			return _sounds.size()-1;
			break;
		case LDSOUND_WAV:
		case LDSOUND_MP2:
		case LDSOUND_MP3:
		case LDSOUND_OGG:
		case LDSOUND_RAW:
			elem.type=type;
			elem.streaming=stream;
			if (stream)
			{
				elem.handle.stream=FSOUND_Stream_OpenFile(filename,(loop)?(FSOUND_LOOP_NORMAL):(FSOUND_LOOP_OFF),0);
				if (!elem.handle.stream) return LDSOUND_ERROR;
			}
			else
			{
				elem.handle.sample=FSOUND_Sample_Load(FSOUND_FREE,filename,(loop)?(FSOUND_LOOP_NORMAL):(FSOUND_LOOP_OFF),0);
				if (!elem.handle.sample) return LDSOUND_ERROR;
			}

			_sounds.push_back(elem);
			return _sounds.size()-1;
			break;
		default:
			return LDSOUND_ERROR;
			break;
	}
#endif
	return LDSOUND_ERROR;
}

long ldSoundFMOD::play(ldSoundID id)
{
	if (!is_sound_enabled()) return -1;

	long chan=-1;

#ifndef _NO_FMOD
	if ((_sounds.size()-1)<id) return -1;

	switch (_sounds[id].type)
	{
		case LDSOUND_MOD:
		case LDSOUND_S3M:
		case LDSOUND_XM:
		case LDSOUND_IT:
		case LDSOUND_MID:
			FMUSIC_PlaySong(_sounds[id].handle.mod);
			break;
		case LDSOUND_WAV:
		case LDSOUND_MP2:
		case LDSOUND_MP3:
		case LDSOUND_OGG:
		case LDSOUND_RAW:
			if (_sounds[id].streaming)
			{
				chan=_sounds[id].channel=FSOUND_Stream_Play(FSOUND_FREE,_sounds[id].handle.stream);
			}
			else
			{
				chan=_sounds[id].channel=FSOUND_PlaySound(FSOUND_FREE,_sounds[id].handle.sample);
			}
			break;
		default:
			break;
	}
#endif

	return chan;
}

void ldSoundFMOD::stop(ldSoundID id,long channel)
{
#ifndef _NO_FMOD
	if (!can_do_sound) return;

	switch (_sounds[id].type)
	{
		case LDSOUND_MOD:
		case LDSOUND_S3M:
		case LDSOUND_XM:
		case LDSOUND_IT:
		case LDSOUND_MID:
			FMUSIC_StopSong(_sounds[id].handle.mod);
			break;
		case LDSOUND_WAV:
		case LDSOUND_MP2:
		case LDSOUND_MP3:
		case LDSOUND_OGG:
		case LDSOUND_RAW:
			if (_sounds[id].streaming)
			{
				FSOUND_Stream_Stop(_sounds[id].handle.stream);
			}
			else
			{
				if (channel==-1)
					FSOUND_StopSound(_sounds[id].channel);
				else 
					FSOUND_StopSound(channel);
			}
			break;
		default:
			break;
	}
#endif
}

void ldSoundFMOD::clear(ldSoundID id)
{
#ifndef _NO_FMOD
	if (!can_do_sound) return;

	if ((_sounds.size()-1)<id) return;

	stop(id);

	switch (_sounds[id].type)
	{
		case LDSOUND_MOD:
		case LDSOUND_S3M:
		case LDSOUND_XM:
		case LDSOUND_IT:
		case LDSOUND_MID:
			FMUSIC_FreeSong(_sounds[id].handle.mod);
			_sounds[id].type=LDSOUND_NONE;
			break;
		case LDSOUND_WAV:
		case LDSOUND_MP2:
		case LDSOUND_MP3:
		case LDSOUND_OGG:
		case LDSOUND_RAW:
			if (_sounds[id].streaming)
			{
				FSOUND_Stream_Close(_sounds[id].handle.stream);
			}
			else
			{
				FSOUND_Sample_Free(_sounds[id].handle.sample);
			}
			_sounds[id].type=LDSOUND_NONE;
			break;
		default:
			break;
	}
#endif
}

void ldSoundFMOD::clear_all()
{
#ifndef _NO_FMOD
	for (unsigned long i=0; i<_sounds.size(); i++)
	{
		clear(i);
	}
	_sounds.clear();
#endif
}

bool ldSoundFMOD::is_playing(ldSoundID id,long channel)
{
#ifndef _NO_FMOD
	if (!can_do_sound) return false;

	if ((_sounds.size()-1)<id) return false;

	switch (_sounds[id].type)
	{
		case LDSOUND_MOD:
		case LDSOUND_S3M:
		case LDSOUND_XM:
		case LDSOUND_IT:
		case LDSOUND_MID:
			if (FMUSIC_IsPlaying(_sounds[id].handle.mod)) return true;
			return false;
			break;
		case LDSOUND_WAV:
		case LDSOUND_MP2:
		case LDSOUND_MP3:
		case LDSOUND_OGG:
		case LDSOUND_RAW:
			if (channel==-1)
				if (FSOUND_IsPlaying(_sounds[id].channel)) return true;
			else 
				if (FSOUND_IsPlaying(channel)) return true;
			return false;
			break;
		default:
			break;
	}

#endif

	return false;
}
