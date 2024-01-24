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
// ldGeoFactory.h: interface for the ldGeoFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDGEOFACTORY_H__0C70231E_AAC5_416A_9ABF_6DC6A9315764__INCLUDED_)
#define AFX_LDGEOFACTORY_H__0C70231E_AAC5_416A_9ABF_6DC6A9315764__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldMesh.h"
#include <vector>

class ldGeoFactory  
{
public:
	ldGeoFactory();
	virtual ~ldGeoFactory();

	// Generic configuration parameters
	inline	void	set_dynamic(bool b) { _dynamic=b; }
	inline	void	set_texturing(int nTex) { _n_tex=nTex; }
	inline	void	set_collidable(bool b) { _collidable=b; }
			void	set_size(float x1,float y1,float z1,float x2,float y2,float z2);
	inline	void	set_resolution(float res_x,float res_y,float res_z) { _res_x=res_x; _res_y=res_y; _res_z=res_z; }

	// Builds a quad on the xz plane (origin at 0,0,0)
			ldMesh*		xz_quad();
			ldMesh*		spheroid(float thetaStart=0.0f,float thetaEnd=PI,float phiStart=0.0f,float phiEnd=PI_TIMES_2,bool top_cap=true,bool bottom_cap=true);
			ldMesh*		prism(long n_sides,bool capped);
			ldMesh*		pyramid(long n_sides,bool capped);

protected:
	ldMesh*			new_mesh();
	unsigned long	load_quad(ldMesh *mesh,
								 float x1,float y1,float z1,float nx1,float ny1,float nz1,float u1,float v1,
								 float x2,float y2,float z2,float nx2,float ny2,float nz2,float u2,float v2,
								 float x3,float y3,float z3,float nx3,float ny3,float nz3,float u3,float v3,
								 float x4,float y4,float z4,float nx4,float ny4,float nz4,float u4,float v4);
	unsigned long	load_tri(ldMesh *mesh,
								 float x1,float y1,float z1,float nx1,float ny1,float nz1,float u1,float v1,
								 float x2,float y2,float z2,float nx2,float ny2,float nz2,float u2,float v2,
								 float x3,float y3,float z3,float nx3,float ny3,float nz3,float u3,float v3);
	void			add_cap(ldMesh *mesh,VertexBuffer *vertex,bool invert);

protected:
	bool	_dynamic;
	int		_n_tex;
	bool	_collidable;
	float	_x1,_y1,_z1;
	float	_x2,_y2,_z2;
	float	_res_x;
	float	_res_y;
	float	_res_z;
};

#endif // !defined(AFX_LDGEOFACTORY_H__0C70231E_AAC5_416A_9ABF_6DC6A9315764__INCLUDED_)
