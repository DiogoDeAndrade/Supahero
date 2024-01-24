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
// ldLogger.h: interface for the ldLogger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGGER_H__FCE7EC4D_0B36_47FE_8BF5_41C612D30E9C__INCLUDED_)
#define AFX_LOGGER_H__FCE7EC4D_0B36_47FE_8BF5_41C612D30E9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

class ldLogger  
{
public:
	public:
		ldLogger(char *t=NULL,char *f=NULL);
		virtual ~ldLogger();

		virtual void log_string(char *string);
		virtual void log(char *string,...);
				void enable(bool b);
		inline	void carriage_return(bool b)	{ cr=b; }

				void set_title(char *t) { if (title) free(title); title=strdup(t); }

protected:
	char *filename;
	char *ext;
	char *title;
	bool firstLog;
	bool enabled;
	bool cr;
};

void		log(char *string,...);
ldLogger	*set_logger(ldLogger *logger);

#endif // !defined(AFX_LOGGER_H__FCE7EC4D_0B36_47FE_8BF5_41C612D30E9C__INCLUDED_)
