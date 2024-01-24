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
// ldInputDI.cpp: implementation of the ldInputDI class.
//
//////////////////////////////////////////////////////////////////////

#include "..\\headers\\ldInputDI.h"
#include "..\Headers\ldLogger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldInputDI	ldInputDIHelper;

ldInputDI::ldInputDI()
{
	_reference_count=0;
}

ldInputDI::~ldInputDI()
{

}

bool ldInputDI::init(HINSTANCE hInst)
{
	if (_reference_count==0)
	{
		HRESULT hRet;

		hRet=DirectInput8Create(hInst,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&_lpdi,NULL);
		if (hRet!=DI_OK)
		{
			log("Couldn't initialize DirectInput!");
			return false;
		}
	}
	
	_reference_count++;

	return true;
}

void ldInputDI::shutdown()
{
	_reference_count--;

	if (_reference_count==0)
	{
		_lpdi->Release();
	}
}
