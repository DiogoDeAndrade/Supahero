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
// ldKeyboardDI.h: interface for the ldKeyboardDI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ldKeyboardDI_H__8FF13776_DF63_48CE_BB5C_F3925AEE2E68__INCLUDED_)
#define AFX_ldKeyboardDI_H__8FF13776_DF63_48CE_BB5C_F3925AEE2E68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldKeyboard.h"
#include <DInput.h>

class ldKeyboardDI : public ldKeyboard  
{
public:
	ldKeyboardDI();
	virtual ~ldKeyboardDI();

	virtual bool init(void *platformParam1=NULL,void *platformParam2=NULL,void *platformParam3=NULL);
	virtual void shutdown();

	virtual void poll(float time);

	// Keyboard querying rotines
	virtual bool	is_any_key_pressed();
	virtual bool	is_key_pressed(char c);
	virtual bool	f_key_pressed(long n);

	virtual char	get_key_pressed();

	virtual	bool	left_shift_pressed();
	virtual	bool	right_shift_pressed();
	virtual	bool	left_ctrl_pressed();
	virtual	bool	right_ctrl_pressed();
	virtual	bool	esc_pressed();
	virtual	bool	space_pressed();
	virtual	bool	up_pressed();
	virtual	bool	down_pressed();
	virtual	bool	left_pressed();
	virtual	bool	right_pressed();
	virtual	bool	page_up_pressed();
	virtual	bool	page_down_pressed();
	virtual	bool	return_pressed();
	virtual	bool	backspace_pressed();


protected:
	LPDIRECTINPUTDEVICE8	_lpKeyboard;
	unsigned char			_last_state[256];
	char					_virt_key[256];
	char					_shift_key[256];
	char					_inv_virt_key[256];
	char					_inv_shift_key[256];
};

#endif // !defined(AFX_ldKeyboardDI_H__8FF13776_DF63_48CE_BB5C_F3925AEE2E68__INCLUDED_)
