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
// ldKeyboardDI.cpp: implementation of the ldKeyboardDI class.
//
//////////////////////////////////////////////////////////////////////

#include "..\\Headers\\ldKeyboardDI.h"
#include "..\\Headers\\ldInputDI.h"
#include "..\Headers\ldLogger.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldKeyboardDI::ldKeyboardDI()
{
	memset((void*)&_last_state,0,sizeof(_last_state));
	memset((void*)&_virt_key,0,sizeof(_virt_key));
	memset((void*)&_shift_key,0,sizeof(_shift_key));
	
	_virt_key[DIK_1]='1'; _virt_key[DIK_2]='2'; _virt_key[DIK_3]='3'; _virt_key[DIK_4]='4'; 
	_virt_key[DIK_5]='5'; _virt_key[DIK_6]='6'; _virt_key[DIK_7]='7'; _virt_key[DIK_8]='8'; 
	_virt_key[DIK_9]='9'; _virt_key[DIK_0]='0'; _virt_key[DIK_MINUS]='-'; _virt_key[DIK_EQUALS]='='; 
	_virt_key[DIK_Q]='q'; _virt_key[DIK_W]='w'; _virt_key[DIK_E]='e'; _virt_key[DIK_R]='r'; 
	_virt_key[DIK_T]='t'; _virt_key[DIK_Y]='y'; _virt_key[DIK_U]='u'; _virt_key[DIK_I]='i'; 
	_virt_key[DIK_O]='o'; _virt_key[DIK_P]='p'; _virt_key[DIK_LBRACKET]='['; _virt_key[DIK_RBRACKET]=']'; 
	_virt_key[DIK_RETURN]=13; _virt_key[DIK_A]='a'; _virt_key[DIK_S]='s'; _virt_key[DIK_D]='d'; 
	_virt_key[DIK_F]='f'; _virt_key[DIK_G]='g'; _virt_key[DIK_H]='h'; _virt_key[DIK_J]='j'; 
	_virt_key[DIK_K]='k'; _virt_key[DIK_L]='l'; _virt_key[DIK_SEMICOLON]=';'; _virt_key[DIK_APOSTROPHE]='\''; 
	_virt_key[DIK_GRAVE]='`'; _virt_key[DIK_BACKSLASH]='\\'; _virt_key[DIK_Z]='z'; _virt_key[DIK_X]='x'; 
	_virt_key[DIK_C]='c'; _virt_key[DIK_V]='v'; _virt_key[DIK_B]='b'; _virt_key[DIK_N]='n'; 
	_virt_key[DIK_M]='m'; _virt_key[DIK_COMMA]=','; _virt_key[DIK_PERIOD]='.'; _virt_key[DIK_SLASH]='/'; 
	_virt_key[DIK_SPACE]=' ';
	_virt_key[DIK_ADD]='+'; _virt_key[DIK_SUBTRACT]='-';
	_virt_key[DIK_MINUS]='-';

	_shift_key[DIK_1]='!'; _shift_key[DIK_2]='"'; _shift_key[DIK_3]='#'; _shift_key[DIK_4]='$'; 
	_shift_key[DIK_5]='%'; _shift_key[DIK_6]='&'; _shift_key[DIK_7]='/'; _shift_key[DIK_8]='('; 
	_shift_key[DIK_9]=')'; _shift_key[DIK_0]='='; _shift_key[DIK_MINUS]='_'; _shift_key[DIK_EQUALS]='='; 
	_shift_key[DIK_Q]='Q'; _shift_key[DIK_W]='W'; _shift_key[DIK_E]='E'; _shift_key[DIK_R]='R'; 
	_shift_key[DIK_T]='T'; _shift_key[DIK_Y]='Y'; _shift_key[DIK_U]='U'; _shift_key[DIK_I]='I'; 
	_shift_key[DIK_O]='O'; _shift_key[DIK_P]='P'; _shift_key[DIK_LBRACKET]='['; _shift_key[DIK_RBRACKET]=']'; 
	_shift_key[DIK_RETURN]=13; _shift_key[DIK_A]='A'; _shift_key[DIK_S]='S'; _shift_key[DIK_D]='D'; 
	_shift_key[DIK_F]='F'; _shift_key[DIK_G]='G'; _shift_key[DIK_H]='H'; _shift_key[DIK_J]='J'; 
	_shift_key[DIK_K]='K'; _shift_key[DIK_L]='L'; _shift_key[DIK_SEMICOLON]=';'; _shift_key[DIK_APOSTROPHE]='\''; 
	_shift_key[DIK_GRAVE]='`'; _shift_key[DIK_BACKSLASH]='\\'; _shift_key[DIK_Z]='Z'; _shift_key[DIK_X]='X'; 
	_shift_key[DIK_C]='C'; _shift_key[DIK_V]='V'; _shift_key[DIK_B]='b'; _shift_key[DIK_N]='n'; 
	_shift_key[DIK_M]='M'; _shift_key[DIK_COMMA]=';'; _shift_key[DIK_PERIOD]=':'; _shift_key[DIK_SLASH]='/'; 
	_shift_key[DIK_SPACE]=' ';
	_shift_key[DIK_EQUALS]='+';

	for (int i=0; i<255; i++) 
	{
		for (int j=0; j<255; j++)
		{
			if (_virt_key[j]==i) _inv_virt_key[i]=j;
			if (_shift_key[j]==i) _inv_shift_key[i]=j;
		}
	}
}

ldKeyboardDI::~ldKeyboardDI()
{

}

bool ldKeyboardDI::init(void *platformParam1,void *platformParam2,void *platformParam3)
{
	if (!platformParam1)
	{
		log("Can't initialize DirectInput keyboard device: HINSTANCE needed for first parameter of init!");
		return false;
	}

	if (!platformParam2)
	{
		log("Can't initialize DirectInput keyboard device: window handler needed for second parameter of init!");
		return false;
	}

	if (!ldInputDIHelper.init(*((HINSTANCE*)platformParam1))) return false;

	HRESULT hRet=ldInputDIHelper._lpdi->CreateDevice(GUID_SysKeyboard,&_lpKeyboard,NULL);
	if (hRet!=DI_OK)
	{
		log("Can't initialize keyboard!");
		ldInputDIHelper.shutdown();
		return false;
	}
	
	hRet=_lpKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (hRet!=DI_OK)
	{
		_lpKeyboard->Release();
		log("Couldn't set the data format for the keyboard!");
		ldInputDIHelper.shutdown();
		return false;
	}

	hRet=_lpKeyboard->SetCooperativeLevel(*((HWND*)platformParam2),
										  DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

	if (hRet!=DI_OK)
	{
		_lpKeyboard->Release();
		ldInputDIHelper.shutdown();
		log("Couldn't set cooperative level for the keyboard!");
		return false;
	}

	hRet=_lpKeyboard->Acquire();
	if (hRet!=DI_OK)
	{
		log("Couldn't acquire the keyboard!");
	}

	return true;
}

void ldKeyboardDI::shutdown()
{
	_lpKeyboard->Unacquire();
	_lpKeyboard->Release();

	ldInputDIHelper.shutdown();
}

void ldKeyboardDI::poll(float time)
{
	unsigned char buffer[256];
	
	HRESULT hRet=_lpKeyboard->GetDeviceState(sizeof(buffer),(void*)&buffer); 
	if (hRet!=DI_OK)
	{
		switch (hRet)
		{
			case DIERR_INPUTLOST:
			case DIERR_NOTACQUIRED:
				hRet=_lpKeyboard->Acquire();
				if (hRet!=DI_OK)
				{
					log("Couldn't poll keyboard: Couldn't acquire the keyboard!");
					return;
				}
				else
				{
					hRet=_lpKeyboard->GetDeviceState(sizeof(buffer),(void*)&buffer); 
					if (hRet!=DI_OK)
					{
						log("Couldn't poll keyboard: Input lost!");
						return;
					}
				}
				break;
			case DIERR_INVALIDPARAM:
				log("Couldn't poll keyboard: Invalid parameter!");
				return;
				break;
			case DIERR_NOTINITIALIZED:
				log("Couldn't poll keyboard: Not initialized!");
				return;
				break;
			case E_PENDING:
				log("Couldn't poll keyboard: Data is not yet available!");
				return;
				break;
		}
	}

	memcpy(_last_state,buffer,256);
}

bool ldKeyboardDI::is_any_key_pressed()
{
	for (int i=0; i<256; i++) if (_last_state[i] && 128) return true;
	
	return false;
}

bool ldKeyboardDI::is_key_pressed(char c)
{
	if (_last_state[_inv_virt_key[c]] & 128) return true;
	
	if (_last_state[_inv_shift_key[c]] & 128) return true;
	
	return false;
}

bool ldKeyboardDI::f_key_pressed(long n)
{
	switch (n)
	{
		case 1: return bool(_last_state[DIK_F1] && 128);
				break;
		case 2: return bool(_last_state[DIK_F2] && 128);
				break;
		case 3: return bool(_last_state[DIK_F3] && 128);
				break;
		case 4: return bool(_last_state[DIK_F4] && 128);
				break;
		case 5: return bool(_last_state[DIK_F5] && 128);
				break;
		case 6: return bool(_last_state[DIK_F6] && 128);
				break;
		case 7: return bool(_last_state[DIK_F7] && 128);
				break;
		case 8: return bool(_last_state[DIK_F8] && 128);
				break;
		case 9: return bool(_last_state[DIK_F9] && 128);
				break;
		case 10: return bool(_last_state[DIK_F10] && 128);
				break;
		case 11: return bool(_last_state[DIK_F11] && 128);
				break;
		case 12: return bool(_last_state[DIK_F12] && 128);
				break;
	}

	return false;
}

char ldKeyboardDI::get_key_pressed()
{
	for (int i=0; i<256; i++)
	{
		if (_last_state[i] & 128) 
		{
			if ((left_shift_pressed()) || (right_shift_pressed()))
				return _shift_key[i];
			else
				return _virt_key[i];
		}
	}

	return 0;
}

bool ldKeyboardDI::left_shift_pressed()
{ 
	return bool(_last_state[DIK_LSHIFT] && 128); 
}

bool ldKeyboardDI::right_shift_pressed() 
{ 
	return bool(_last_state[DIK_RSHIFT] && 128); 
}

bool ldKeyboardDI::left_ctrl_pressed() 
{ 
	return bool(_last_state[DIK_LCONTROL] && 128); 
}

bool ldKeyboardDI::right_ctrl_pressed() 
{ 
	return bool(_last_state[DIK_RCONTROL] && 128); 
}

bool ldKeyboardDI::esc_pressed() 
{ 
	return bool(_last_state[DIK_ESCAPE] && 128); 
}

bool ldKeyboardDI::space_pressed() 
{ 
	return bool(_last_state[DIK_SPACE] && 128); 
}

bool ldKeyboardDI::up_pressed() 
{ 
	return bool(_last_state[DIK_UP] && 128); 
}

bool ldKeyboardDI::down_pressed() 
{ 
	return bool(_last_state[DIK_DOWN] && 128); 
}

bool ldKeyboardDI::left_pressed() 
{ 
	return bool(_last_state[DIK_LEFT] && 128); 
}

bool ldKeyboardDI::right_pressed() 
{ 
	return bool(_last_state[DIK_RIGHT] && 128); 
}

bool ldKeyboardDI::page_up_pressed() 
{ 
	return bool(_last_state[DIK_PRIOR] && 128); 
}

bool ldKeyboardDI::page_down_pressed() 
{ 
	return bool(_last_state[DIK_NEXT] && 128); 
}

bool ldKeyboardDI::return_pressed() 
{ 
	return bool(_last_state[DIK_RETURN] && 128); 
}

bool ldKeyboardDI::backspace_pressed() 
{ 
	return bool(_last_state[DIK_BACK] && 128); 
}
