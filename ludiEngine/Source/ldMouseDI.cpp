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
// ldMouseDI.cpp: implementation of the ldMouseDI class.
//
//////////////////////////////////////////////////////////////////////

#include "..\Headers\ldMouseDI.h"
#include "..\Headers\ldInputDI.h"
#include "..\Headers\ldLogger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldMouseDI::ldMouseDI()
{
	_lpMouse=NULL;
}

ldMouseDI::~ldMouseDI()
{

}

bool ldMouseDI::init(void *platformParam1,void *platformParam2,void *platformParam3)
{
	if (!platformParam1)
	{
		log("Can't initialize DirectInput mouse device: HINSTANCE needed for first parameter of init!");
		return false;
	}

	if (!platformParam2)
	{
		log("Can't initialize DirectInput mouse device: window handle needed for second parameter of init!");
		return false;
	}

	if (!ldInputDIHelper.init(*((HINSTANCE*)platformParam1))) return false;

	HRESULT hRet=ldInputDIHelper._lpdi->CreateDevice(GUID_SysMouse,&_lpMouse,NULL);
	if (hRet!=DI_OK)
	{
		log("Can't initialize mouse!");
		ldInputDIHelper.shutdown();
		return false;
	}
	
	hRet=_lpMouse->SetDataFormat(&c_dfDIMouse);
	if (hRet!=DI_OK)
	{
		_lpMouse->Release();
		log("Couldn't set the data format for the mouse!");
		ldInputDIHelper.shutdown();
		return false;
	}

	hRet=_lpMouse->SetCooperativeLevel(*((HWND*)platformParam2),
									   DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

	if (hRet!=DI_OK)
	{
		_lpMouse->Release();
		ldInputDIHelper.shutdown();
		log("Couldn't set cooperative level for the mouse!");
		return false;
	}

	hRet=_lpMouse->Acquire();
	if (hRet!=DI_OK)
	{
		log("Couldn't acquire the mouse!");
	}

	_start_up=true;

	return true;
}

void ldMouseDI::shutdown()
{
	if (_lpMouse)
	{
		_lpMouse->Unacquire();
		_lpMouse->Release();
	}

	ldInputDIHelper.shutdown();
}

void ldMouseDI::poll(float time)
{
	DIMOUSESTATE	mouseState;

	HRESULT hRet=_lpMouse->GetDeviceState(sizeof(DIMOUSESTATE),(void*)&mouseState); 
	if (hRet!=DI_OK)
	{
		switch (hRet)
		{
			case DIERR_INPUTLOST:
			case DIERR_NOTACQUIRED:
				hRet=_lpMouse->Acquire();
				if (hRet!=DI_OK)
				{
					log("Couldn't poll mouse: Couldn't acquire the mouse!");
					return;
				}
				else
				{
					hRet=_lpMouse->GetDeviceState(sizeof(DIMOUSESTATE),(void*)&mouseState); 
					if (hRet!=DI_OK)
					{
						log("Couldn't poll mouse: Input lost!");
						return;
					}
				}
				break;
			case DIERR_INVALIDPARAM:
				log("Couldn't poll mouse: Invalid parameter!");
				return;
				break;
			case DIERR_NOTINITIALIZED:
				log("Couldn't poll mose: Not initialized!");
				return;
				break;
			case E_PENDING:
				log("Couldn't poll mouse: Data is not yet available!");
				return;
				break;
		}
	}

	if (_start_up)
	{
		_delta_x=0;
		_delta_y=0;
		_delta_z=0;
		_start_up=false;
	}
	else
	{
		_delta_x=mouseState.lX;
		_delta_y=mouseState.lY;
		_delta_z=mouseState.lZ;
	}

	for (int i=0; i<4; i++)
	{
		if (mouseState.rgbButtons[i] & 128)
		{
			if (!(_last_state.rgbButtons[i] & 128))
			{
				_time_of_click[i]=timeGetTime();
			}
			else
			{
				_time_of_click[i]=0;
			}
		}
		else
		{
			_time_of_click[i]=0;
		}
	}

	memcpy(&_last_state,&mouseState,sizeof(DIMOUSESTATE));
}

bool ldMouseDI::button_pressed(int n)
{
	if (_last_state.rgbButtons[n] & 128) return true;
	else return false;
}

bool ldMouseDI::button_clicked(int n)
{
	if (_time_of_click[n]==0) return false;
	if (timeGetTime()-_time_of_click[n]>500) return false;
	return true;
}

long ldMouseDI::get_delta_x()
{
	return _delta_x;
}

long ldMouseDI::get_delta_y()
{
	return _delta_y;
}

long ldMouseDI::get_delta_z()
{
	return _delta_z;
}

