/*
 * Log.cpp
 *
 *  Created on: Jun 7, 2019
 *      Author: jim
 */

#include "Log.h"
#include <iostream>
#include <syslog.h>
#include <stdarg.h>
#include <stdio.h>


Log::Log():
	init_(false),
	logname_("")
{
}

Log::~Log()
{
	closelog();
}

Log& Log::getInstance()
{
	static Log logger;

	return logger;
}

void Log::init(std::string logname)
{
	logname_ = logname;
	init_ = true;

	openlog(logname_.c_str(), LOG_CONS, LOG_LOCAL4);
}


//template<typename... Args> void Log::log(const char * f, Args... args)
//{
//if (!init)
//{
//	return;
//}
//  printf(f, args...);
//  syslog(LOG_INFO | LOG_LOCAL4, f, args...);
//}

//void Log::log( const char * format, ... )
//{
//  char buffer[2048];
//  va_list args;
//
//  if (!init_)
//  {
//	  return;
//  }
//
//  va_start (args, format);
//  vsprintf (buffer,format, args);
//
//  syslog(LOG_INFO | LOG_LOCAL4, format, args...);
//  va_end (args);
//}
