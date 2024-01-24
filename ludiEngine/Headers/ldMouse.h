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
// ldMouse.h: interface for the ldMouse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ldMouse_H__D5FD2A06_3D72_48DA_866F_CF48D76D6984__INCLUDED_)
#define AFX_ldMouse_H__D5FD2A06_3D72_48DA_866F_CF48D76D6984__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>

class ldMouse
{
public:
	ldMouse();
	virtual ~ldMouse();

	virtual bool init(void *platformParam1=NULL,void *platformParam2=NULL,void *platformParam3=NULL) = 0;
	virtual void shutdown() = 0;

	virtual void poll(float time) = 0;

	// Querying methods
	virtual bool button_pressed(int n) = 0;
	virtual bool button_clicked(int n) = 0;
	virtual long get_delta_x() = 0;
	virtual long get_delta_y() = 0;
	virtual long get_delta_z() = 0;

};

extern ldMouse*		MOUSE;

#endif // !defined(AFX_ldMouse_H__D5FD2A06_3D72_48DA_866F_CF48D76D6984__INCLUDED_)
