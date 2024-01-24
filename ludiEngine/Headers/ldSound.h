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
// ldSound.h: interface for the ldSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDSOUND_H__B7B99AE9_089E_413B_9983_C39940116F2F__INCLUDED_)
#define AFX_LDSOUND_H__B7B99AE9_089E_413B_9983_C39940116F2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef unsigned long	ldSoundID;

typedef enum { LDSOUND_NONE,
			   LDSOUND_MOD, LDSOUND_S3M, LDSOUND_XM, LDSOUND_IT, LDSOUND_MID,
			   LDSOUND_WAV, LDSOUND_MP2, LDSOUND_MP3, LDSOUND_OGG, LDSOUND_RAW } ldSoundType;

#define LDSOUND_ERROR	0x00FFFFFFFF

class ldSound  
{
public:
	ldSound();
	virtual ~ldSound();

	virtual	bool		init(long playback_rate,long max_channels) = 0;
	virtual void		shutdown() = 0;

	inline	bool		is_sound_enabled() { return enabled && can_do_sound; }
	inline	bool		enable_sound(bool b=true) { enabled=b; }

	virtual	ldSoundID	load(ldSoundType type,char *filename,bool loop=false,bool stream=false) = 0;
	virtual long		play(ldSoundID id) = 0;
	virtual void		stop(ldSoundID id,long channel=-1) = 0;
	virtual void		clear(ldSoundID id) = 0;
	virtual void		clear_all() = 0;

	virtual bool		is_playing(ldSoundID id,long channel=-1) = 0;

protected:
	bool	enabled;
	bool	can_do_sound;
};

extern ldSound*	SND;

#endif // !defined(AFX_LDSOUND_H__B7B99AE9_089E_413B_9983_C39940116F2F__INCLUDED_)
