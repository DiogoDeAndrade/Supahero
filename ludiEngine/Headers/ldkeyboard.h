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
// ldKeyboard.h: interface for the ldKeyboard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ldKeyboard_H__B57455D5_3999_429B_9995_C28507AA6A21__INCLUDED_)
#define AFX_ldKeyboard_H__B57455D5_3999_429B_9995_C28507AA6A21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>

class ldKeyboard
{
public:
	ldKeyboard();
	virtual ~ldKeyboard();

	virtual bool init(void *platformParam1=NULL,void *platformParam2=NULL,void *platformParam3=NULL) = 0;
	virtual void shutdown() = 0;

	virtual void poll(float time) = 0;

	// Querying methods
	virtual bool	is_any_key_pressed() = 0;
	virtual bool	is_key_pressed(char c) = 0;
	virtual bool	f_key_pressed(long n) = 0;

	virtual char	get_key_pressed() = 0;

	virtual	bool	left_shift_pressed() = 0;
	virtual	bool	right_shift_pressed() = 0;
	virtual	bool	left_ctrl_pressed() = 0;
	virtual	bool	right_ctrl_pressed() = 0;
	virtual	bool	esc_pressed() = 0;
	virtual	bool	space_pressed() = 0;
	virtual	bool	up_pressed() = 0;
	virtual	bool	down_pressed() = 0;
	virtual	bool	left_pressed() = 0;
	virtual	bool	right_pressed() = 0;
	virtual	bool	page_up_pressed() = 0;
	virtual	bool	page_down_pressed() = 0;
	virtual	bool	return_pressed() = 0;
	virtual	bool	backspace_pressed() = 0;

};

extern ldKeyboard*		KEYBOARD;

#endif // !defined(AFX_ldKeyboard_H__B57455D5_3999_429B_9995_C28507AA6A21__INCLUDED_)
