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
// ldMouse.cpp: implementation of the ldMouse class.
//
//////////////////////////////////////////////////////////////////////

#include "..\Headers\ldMouse.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldMouse*	MOUSE;

ldMouse::ldMouse()
{
	MOUSE=this;
}

ldMouse::~ldMouse()
{
	if (MOUSE==this) MOUSE=NULL;
}

