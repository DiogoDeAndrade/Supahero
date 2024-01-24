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
// ldSound.cpp: implementation of the ldSound class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldSound.h"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldSound*	SND=NULL;

ldSound::ldSound()
{
	if (!SND) SND=this;
	
	can_do_sound=false;
	enabled=true;
}

ldSound::~ldSound()
{
	if (SND==this) SND=NULL;
}
