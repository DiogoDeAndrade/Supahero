// shVictims.h: interface for the shVictims class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHVICTIMS_H__F9717366_D486_424C_BD80_793E53E6E404__INCLUDED_)
#define AFX_SHVICTIMS_H__F9717366_D486_424C_BD80_793E53E6E404__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../LudiEngine/Headers/ldDrawObj3D.h"

class shLevel;

class shVictim : public ldDrawObj3D  
{
public:
	shVictim();
	virtual ~shVictim();

			void	update(unsigned long t,shLevel *lvl);

public:
	Vec3f	vel;
	Vec3f	accel;
	bool	active;
	long	time_to_die;
};

#include <vector>
typedef std::vector<shVictim*> shVictims;

#endif // !defined(AFX_SHVICTIMS_H__F9717366_D486_424C_BD80_793E53E6E404__INCLUDED_)
