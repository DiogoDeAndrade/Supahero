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
// ldFont.h: interface for the ldFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDFONT_H__FB379FF9_619A_4FCC_8D76_630521463F28__INCLUDED_)
#define AFX_LDFONT_H__FB379FF9_619A_4FCC_8D76_630521463F28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ldFont  
{
public:
	ldFont();
	virtual ~ldFont();

	virtual void	load(char *name,long height,bool bold=false,bool italic=false,bool underline=false,bool strikeout=false) = 0;
	virtual void	write(float x,float y,float r,float g,float b,float a,char *string,...) = 0;
};

#endif // !defined(AFX_LDFONT_H__FB379FF9_619A_4FCC_8D76_630521463F28__INCLUDED_)
