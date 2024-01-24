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
// ldMaterialD3D.h: interface for the ldMaterialD3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDMATERIALD3D_H__42E596D1_7997_4A1B_BE1E_F8A0994309A9__INCLUDED_)
#define AFX_LDMATERIALD3D_H__42E596D1_7997_4A1B_BE1E_F8A0994309A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldMaterial.h"
#include <d3d8.h>

class ldMaterialD3D : public ldMaterial  
{
public:
	ldMaterialD3D();
	virtual ~ldMaterialD3D();

	virtual	void	set();

protected:
	D3DTEXTUREOP	ld_to_dx(ldBlendOp op);
	unsigned long	ld_to_dx(ldBlendArgument arg);
};

#endif // !defined(AFX_LDMATERIALD3D_H__42E596D1_7997_4A1B_BE1E_F8A0994309A9__INCLUDED_)
