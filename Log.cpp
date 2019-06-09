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

	if (init_)
	{
		file_.close();
	}
}

Log& Log::getInstance()
{
	static Log logger;

	return logger;
}

void Log::init(std::string logname, std::string filepath)
{
	logname_ = logname;

	openlog(logname_.c_str(), LOG_CONS, LOG_LOCAL4);

	file_.open(filepath, fstream::out | fstream::app);

	if (file_.is_open())
	{
		init_ = true;
	}
	else
	{
		std::cout << "Could not open log file: " << filepath << std::endl;
	}
}

void Log::log( const char * format, ... )
{
  char buffer[2048];
  va_list args;

  if (!init_)
  {
	  return;
  }

  va_start (args, format);
  vsprintf (buffer,format, args);

  printf("%s\n", buffer);
  syslog(LOG_INFO | LOG_LOCAL4, "%s", buffer);

  file_ << buffer << std::endl;

  va_end (args);
}
