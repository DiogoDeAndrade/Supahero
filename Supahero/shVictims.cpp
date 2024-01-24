// shVictims.cpp: implementation of the shVictims class.
//
//////////////////////////////////////////////////////////////////////

#include "shVictims.h"
#include "shLevel.h"

#define VICTIM_SPEED	0.0005f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

shVictim::shVictim()
{
	vel.set(0.0f,0.0f,0.0f);
	accel.set(0.0f,0.0f,0.0f);
	active=true;
	time_to_die=0;
}

shVictim::~shVictim()
{

}

void shVictim::update(unsigned long t,shLevel *lvl)
{
	if (time_to_die==0.0f) 
	{
		vel=vel+accel;
		
		pos=pos+(t*vel*VICTIM_SPEED);
		pos.y=lvl->cast_ray_down_scenario(pos.x,200.0f,pos.z);

		Vec3f vec=-vel;
		if ((vec.x!=0.0f) || (vec.z!=0.0f)) 
		{
			ori.identity_quaternion();
		}

		// Rotate character
		if ((vec.x>0.0f) && (vec.z<0.0f)) fpm_turn_right(45.0f); else	// NE
		if ((vec.x==0.0f) && (vec.z<0.0f)) fpm_turn_right(0.0f); else	// N
		if ((vec.x<0.0f) && (vec.z<0.0f)) fpm_turn_left(45.0f);	 else	// NW
		if ((vec.x<0.0f) && (vec.z==0.0f)) fpm_turn_left(90.0f); else	// E
		if ((vec.x>0.0f) && (vec.z==0.0f)) fpm_turn_right(90.0f); else	// W
		if ((vec.x>0.0f) && (vec.z>0.0f)) fpm_turn_right(135.0f); else	// SE
		if ((vec.x==0.0f) && (vec.z>0.0f)) fpm_turn_right(180.0f); else	// S
		if ((vec.x<0.0f) && (vec.z>0.0f)) fpm_turn_left(135.0f);		// SW
	}

	if (time_to_die>0)
	{
		if (t>time_to_die)
		{
			// Character is dead
			for (unsigned long i=0; i<lvl->characters.size(); i++)
			{
				if (lvl->characters[i]==this)
				{
					delete lvl->characters[i];
					lvl->characters[i]=NULL;
					return;
				}
			}
		}
		else
		{
			fpm_down(0.002f*t);
		}
	}
}
