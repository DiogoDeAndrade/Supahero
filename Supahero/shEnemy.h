// shEnemy.h: interface for the shEnemy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHENEMY_H__1D96B5E5_10F7_4A59_84E7_52DCF88903AC__INCLUDED_)
#define AFX_SHENEMY_H__1D96B5E5_10F7_4A59_84E7_52DCF88903AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../LudiEngine/Headers/ldDrawObj3D.h"

class shLevel;

class shEnemy : public ldDrawObj3D  
{
public:
	shEnemy();
	virtual ~shEnemy();

	virtual void draw();
	virtual void anim(unsigned long t,shLevel *level);

			void fire(shLevel *level);

public:
	unsigned long	idle;
	unsigned long	no_draw;
	unsigned long	last_shot;
	long			rise;
	unsigned long	time_to_die;
};

#include <vector>
typedef std::vector<shEnemy*>	shEnemies;

#endif // !defined(AFX_SHENEMY_H__1D96B5E5_10F7_4A59_84E7_52DCF88903AC__INCLUDED_)
