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
// ldSoundFMOD.h: interface for the ldSoundFMOD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDSOUNDFMOD_H__8B678A5E_4D38_432D_BD73_2059D9BB3FBD__INCLUDED_)
#define AFX_LDSOUNDFMOD_H__8B678A5E_4D38_432D_BD73_2059D9BB3FBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldSound.h"
#ifndef _NO_FMOD
#include <fmod.h>
#endif
#include <vector>

class ldSoundFMOD : public ldSound  
{
#ifndef _NO_FMOD
protected:
	typedef std::vector<long>	ldChannels;
	
	struct ldSoundArrayElem
	{
		ldSoundType	type;
		bool		streaming;
		long		channel;
		union {
			FMUSIC_MODULE	*mod;
			FSOUND_SAMPLE	*sample;
			FSOUND_STREAM	*stream;
		} handle;
	};

	typedef std::vector<ldSoundArrayElem>	ldSoundArray;
#endif
public:
	ldSoundFMOD();
	virtual ~ldSoundFMOD();

	virtual	bool		init(long playback_rate,long max_channels);
	virtual void		shutdown();

	virtual	ldSoundID	load(ldSoundType type,char *filename,bool loop=false,bool stream=false);
	virtual long		play(ldSoundID id);
	virtual void		stop(ldSoundID id,long channel=-1);
	virtual void		clear(ldSoundID id);
	virtual void		clear_all();

	virtual bool		is_playing(ldSoundID id,long channel=-1);

#ifndef _NO_FMOD
protected:
	ldSoundArray		_sounds;
#endif // _NO_FMOD
};
#endif // !defined(AFX_LDSOUNDFMOD_H__8B678A5E_4D38_432D_BD73_2059D9BB3FBD__INCLUDED_)
