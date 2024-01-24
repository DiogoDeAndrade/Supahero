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

#include "..\HEADERS\ldUtils.h"

float get_angle_betwen_vectors(float x1,float y1,float x2,float y2)
{
	Vec2f	vec(x2-x1,y2-y1);
	
	float hip=vec.norm();
	float tmp;

	tmp=rad_to_deg(acos((x2-x1)/hip));

	if ((y2-y1)<0) tmp=360.0f-tmp;
	
	return tmp;
}
