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
// ldInputDI.h: interface for the ldInputDI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ldInputDI_H__D00CD18F_3784_427D_B310_C9947C7F40E2__INCLUDED_)
#define AFX_ldInputDI_H__D00CD18F_3784_427D_B310_C9947C7F40E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <DInput.h>

// This is a helper object, that encapsulates the DirectInput object

class ldInputDI  
{
public:
	ldInputDI();
	virtual ~ldInputDI();

	bool	init(HINSTANCE hInst);
	void	shutdown();

public:
	LPDIRECTINPUT8	_lpdi;

protected:
	int				_reference_count;
};

extern ldInputDI	ldInputDIHelper;

#endif // !defined(AFX_ldInputDI_H__D00CD18F_3784_427D_B310_C9947C7F40E2__INCLUDED_)
