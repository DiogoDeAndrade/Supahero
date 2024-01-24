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
// ldFontD3D.cpp: implementation of the ldFontD3D class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldFontD3D.h"
#include "..\HEADERS\ldGfxD3D.h"
#include "..\HEADERS\ldUtils.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldFontD3D::ldFontD3D()
{
	font=NULL;
}

ldFontD3D::~ldFontD3D()
{
	if (font)
	{
		DeleteObject(font);
		font_d3d->Release();
	}
}

void ldFontD3D::load(char *name,long height,bool bold,bool italic,bool underline,bool strikeout)
{
	if (font) 
	{
		font_d3d->Release();
		DeleteObject(font);
	}

	font=CreateFont(height,0,0,0,
					(bold)?(FW_BOLD):(FW_DONTCARE),
					(italic)?(TRUE):(FALSE),
					(underline)?(TRUE):(FALSE),
					(strikeout)?(TRUE):(FALSE),
					ANSI_CHARSET,
					OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY,
					DEFAULT_PITCH | FF_DONTCARE,
					name);

	if (font)
	{
		HRESULT hRet=D3DXCreateFont(((ldGfxD3D*)GFX)->_g_pd3dDevice,font,&font_d3d);
		if (hRet!=D3D_OK)
		{
			font_d3d=NULL;
			DeleteObject(font); font=NULL;
		}
	}
}

void ldFontD3D::write(float x,float y,float r,float g,float b,float a,char *string,...)
{
	static	char	buffer[8192];
	va_list			argptr;

	va_start(argptr,string);
	vsprintf(buffer,string,argptr);
	va_end(argptr);

	write_text(x,y,r,g,b,a,(char*)&buffer);
}

void ldFontD3D::write_text(float x,float y,float r,float g,float b,float a,char *string)
{
	if (!font_d3d) return;

	if (font_d3d->Begin()==D3D_OK)
	{
		RECT		rect;
		D3DCOLOR	color;

		color=float_to_colorref(r,g,b,a);

		rect.left=x;
		rect.top=y;
		rect.bottom=y+1;
		rect.right=x+1;

		font_d3d->DrawTextA(string,-1,&rect,DT_CALCRECT,color);
		font_d3d->DrawTextA(string,-1,&rect,DT_LEFT,color);
		font_d3d->End();
	}
}
