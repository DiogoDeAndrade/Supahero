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
// ldLight3DD3D.h: interface for the ldLight3DD3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDLIGHT3DD3D_H__4488356C_263E_40F2_94A4_E16B0743D1F1__INCLUDED_)
#define AFX_LDLIGHT3DD3D_H__4488356C_263E_40F2_94A4_E16B0743D1F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldLight3d.h"

class ldLight3DD3D : public ldLight3D  
{
public:
	ldLight3DD3D();
	virtual ~ldLight3DD3D();

		void	set(unsigned long index);
};

#endif // !defined(AFX_LDLIGHT3DD3D_H__4488356C_263E_40F2_94A4_E16B0743D1F1__INCLUDED_)
