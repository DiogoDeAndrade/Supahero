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
// ldMouseDI.h: interface for the ldMouseDI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ldMouseDI_H__2C208785_AE38_40FB_8695_8EDB139C68F8__INCLUDED_)
#define AFX_ldMouseDI_H__2C208785_AE38_40FB_8695_8EDB139C68F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldMouse.h"
#include <dinput.h>

class ldMouseDI : public ldMouse
{
public:
	ldMouseDI();
	virtual ~ldMouseDI();

	virtual bool init(void *platformParam1=NULL,void *platformParam2=NULL,void *platformParam3=NULL);
	virtual void shutdown();

	virtual void poll(float time);

	// Querying methods
	virtual bool button_pressed(int n);
	virtual bool button_clicked(int n);
	virtual long get_delta_x();
	virtual long get_delta_y();
	virtual long get_delta_z();

protected:
	LPDIRECTINPUTDEVICE8	_lpMouse;
	DIMOUSESTATE			_last_state;
	long					_delta_x,_delta_y,_delta_z;
	bool					_start_up;
	unsigned long			_time_of_click[4];
};

#endif // !defined(AFX_ldMouseDI_H__2C208785_AE38_40FB_8695_8EDB139C68F8__INCLUDED_)
