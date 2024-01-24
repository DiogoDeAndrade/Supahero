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
// ldCamera3D.h: interface for the ldCamera3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDCAMERA3D_H__88844DB7_75C8_4E60_B08F_1FCE1A4646FD__INCLUDED_)
#define AFX_LDCAMERA3D_H__88844DB7_75C8_4E60_B08F_1FCE1A4646FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldObject3d.h"

class ldCamera3D : public ldObject3D  
{
public:
	ldCamera3D();
	virtual ~ldCamera3D();

			void set();
	inline	void set_fov(float fov) { _fov=fov; }
	inline	void set_near_plane(float near_plane) { _near_plane=near_plane; }
	inline	void set_far_plane(float far_plane) { _far_plane=far_plane; }
	inline	void set_aspect_ratio(float aspect_ratio) { _aspect_ratio=aspect_ratio; }

protected:
	float	_fov;
	float	_near_plane,_far_plane;
	float	_aspect_ratio;
};

#endif // !defined(AFX_LDCAMERA3D_H__88844DB7_75C8_4E60_B08F_1FCE1A4646FD__INCLUDED_)
