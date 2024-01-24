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
// ldFontD3D.h: interface for the ldFontD3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDFONTD3D_H__0C225AD7_497B_4F8D_8A65_E1BADFFF52D7__INCLUDED_)
#define AFX_LDFONTD3D_H__0C225AD7_497B_4F8D_8A65_E1BADFFF52D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldFont.h"
#include <d3dx8.h>

class ldFontD3D : public ldFont  
{
public:
	ldFontD3D();
	virtual ~ldFontD3D();

	virtual void	load(char *name,long height,bool bold=false,bool italic=false,bool underline=false,bool strikeout=false);
	virtual void	write(float x,float y,float r,float g,float b,float a,char *string,...);


protected:
			void	write_text(float x,float y,float r,float g,float b,float a,char *string);

protected:
	HFONT		font;
	LPD3DXFONT	font_d3d;
};

#endif // !defined(AFX_LDFONTD3D_H__0C225AD7_497B_4F8D_8A65_E1BADFFF52D7__INCLUDED_)
