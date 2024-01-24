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

#include "../HEADERS/LudiEngine.h"
#include <windows.h>
#include <mmsystem.h>

bool			ldExit=false;
ldLoopFunction	loopFunc=NULL;

void LD_loop()
{
	MSG				msg;
	unsigned long	t;

	unsigned long	lastTime;

	ldExit=false;

	lastTime=timeGetTime();

	while (!ldExit)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Stop app going more than a given frames per second
			t=timeGetTime();
			if (loopFunc) loopFunc(t-lastTime);
			lastTime=t;
		}
	}
}

void LD_set_loop_function(ldLoopFunction pt)
{
	loopFunc=pt;
}

void LD_set_exit(bool exit)
{
	ldExit=exit;
}