// shSupahero.h: interface for the shSupahero class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHSUPAHERO_H__BAD17584_D919_45C5_A451_74DB5FDB4532__INCLUDED_)
#define AFX_SHSUPAHERO_H__BAD17584_D919_45C5_A451_74DB5FDB4532__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../LudiEngine/Headers/ldDrawObj3D.h"

#include "shLevel.h"

#define	SUPA_MOVE_SPEED	0.05f

class shSupahero : public ldDrawObj3D  
{
public:
	shSupahero();
	virtual ~shSupahero();

	virtual void	anim(unsigned long t,shLevel *level);

			void	fire(shLevel *level);

public:
	Vec3f			vec;
	unsigned long	time_since_last_shot;
	long			energy;
	long			time_to_die;
	long			invulnerable;
};

#endif // !defined(AFX_SHSUPAHERO_H__BAD17584_D919_45C5_A451_74DB5FDB4532__INCLUDED_)
