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
// ldKeyboard.cpp: implementation of the ldKeyboard class.
//
//////////////////////////////////////////////////////////////////////

#include "..\\headers\\ldKeyboard.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldKeyboard *KEYBOARD;

ldKeyboard::ldKeyboard()
{
	KEYBOARD=this;
}

ldKeyboard::~ldKeyboard()
{
	if (KEYBOARD==this) KEYBOARD=NULL;
}

