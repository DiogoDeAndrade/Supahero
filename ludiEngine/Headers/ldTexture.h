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
// ldTexture.h: interface for the ldTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDTEXTURE_H__87F6144F_9454_4A12_B746_700BE440A25C__INCLUDED_)
#define AFX_LDTEXTURE_H__87F6144F_9454_4A12_B746_700BE440A25C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LDTEXTURE_MAX_NUMBER	8

class ldTexture  
{
public:
	ldTexture();
	virtual ~ldTexture();

	inline	unsigned long	get_width() { return _sx; }
	inline	unsigned long	get_height() { return _sy; }

	inline	char*			get_name() { return (char*)&_name; }

	inline	void	set_wrap(bool u,bool v) { _wrap_u=u; _wrap_v=v; }

	virtual bool	load(char *filename) = 0;

	virtual void	clear() = 0;

protected:
	unsigned long	_sx;
	unsigned long	_sy;
	bool			_wrap_u,_wrap_v;
	char			_name[512];
};

ldTexture *get_texture(char *name);
void delete_all_textures();


#endif // !defined(AFX_LDTEXTURE_H__87F6144F_9454_4A12_B746_700BE440A25C__INCLUDED_)
