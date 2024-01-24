// shGeoFactory.h: interface for the shGeoFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_shGeoFactory_H__CAF14826_402B_4BA1_88AC_1424F40FE5F2__INCLUDED_)
#define AFX_shGeoFactory_H__CAF14826_402B_4BA1_88AC_1424F40FE5F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../LudiEngine/Headers/ldGeoFactory.h"

class shGeoFactory : public ldGeoFactory  
{
public:
	shGeoFactory();
	virtual ~shGeoFactory();

	ldMesh* game_char(float height,long text_head,long text_body);
	ldMesh*	game_char_body(float size);
	ldMesh*	game_char_head(float size);

	ldMesh *landscape(float size,float bumps);

	ldMesh* enemy(long i);

	ldMesh* supa(long i);
};

#endif // !defined(AFX_shGeoFactory_H__CAF14826_402B_4BA1_88AC_1424F40FE5F2__INCLUDED_)
