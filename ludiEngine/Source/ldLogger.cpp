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
// ldLogger.cpp: implementation of the ldLogger class.
//
//////////////////////////////////////////////////////////////////////

#include <afx.h>
#include <stdio.h>
#include <time.h>
#include <io.h>
#include "../Headers/ldLogger.h"

void CleanString(char *string)
{
	for (char *tmp=string; *tmp!=0; tmp++)
	{
		if (*tmp==10) *tmp=32;
		if (*tmp==13) *tmp=32;
	}
}

ldLogger		*currentLogger=NULL;
bool			firstLog=true;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldLogger::ldLogger(char *t,char *f)
{
	cr=true;
	firstLog=true;
	if (t) title=strdup(t); else title=NULL;
	if (f) 
	{
		char tmp1[512];
		char tmp2[512];
		char tmp3[512]; char *tmp4=f;
		long index=0;

		while (*tmp4!='.') { tmp3[index]=*tmp4; index++; tmp4++; }
		tmp3[index]=0;
		ext=strdup(tmp4);

		GetCurrentDirectory(512,(char*)&tmp1);

		sprintf((char*)&tmp2,"%s\\Logs",tmp1);
		CreateDirectory((char*)&tmp2,NULL);

		sprintf((char*)&tmp2,"%s\\Logs\\%s",tmp1,tmp3);

		filename=strdup(tmp2); 
	}
	else { filename=NULL; ext=NULL; }

	enabled=true;

	if (!currentLogger) currentLogger=this;
}

ldLogger::~ldLogger()
{
	if (title) free(title); title=NULL;
	if (filename) free(filename); filename=NULL;
	if (ext) free(ext); ext=NULL;
}

void ldLogger::log(char *string,...)
{
	static	char	buffer[8192];
	va_list			argptr;

	va_start(argptr,string);
	vsprintf(buffer,string,argptr);
	va_end(argptr);

	log_string((char*)&buffer);
}

void ldLogger::log_string(char *string)
{
	FILE	*logFile;
	time_t	t;
	char	buffer[8192];

	if (!enabled) return;

	if (filename) 
	{		
		sprintf((char*)&buffer,"%s%s",filename,ext);

		logFile=fopen((char*)&buffer,"at");
	}
	else logFile=fopen("LogFile.txt","at");
	if (!logFile) return;

	if (firstLog)
	{
		t=time(NULL);
		fprintf(logFile,"-------------------------------------------------------------------------------------\n");
		if (title) fprintf(logFile,"%s log session started %s",title,ctime(&t));
		else fprintf(logFile,"Log session started %s",ctime(&t));
		fprintf(logFile,"-------------------------------------------------------------------------------------\n");
		firstLog=false;
	}

	time_t tt;
	time(&tt);

	if (title)
		sprintf((char*)&buffer,"%s%s: %s",ctime(&tt),title,string);
	else 
		sprintf((char*)&buffer,"%s - %s\n",ctime(&tt),string);
	CleanString((char*)&buffer);
	if (cr) fprintf(logFile,"%s\n",(char*)&buffer);
	else fprintf(logFile,"%s",(char*)&buffer);

	fclose(logFile);
}

void ldLogger::enable(bool b) 
{ 
	enabled=b; 
}

void log(char *string,...)
{
	if (currentLogger) 
	{
		static	char	buffer[8192];
		va_list			argptr;

		va_start(argptr,string);
		vsprintf(buffer,string,argptr);
		va_end(argptr);

		currentLogger->log_string((char*)&buffer);
	}
}

ldLogger *set_logger(ldLogger *logger)
{
	ldLogger *tmp=currentLogger;

	currentLogger=logger;

	return tmp;
}
