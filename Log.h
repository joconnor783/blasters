/*
 * Log.h
 *
 *  Created on: Jun 7, 2019
 *      Author: jim
 */

#ifndef LOG_H_
#define LOG_H_

#include <string>

#include <syslog.h>
#include <stdarg.h>
#include <stdio.h>

class Log {
public:



	virtual ~Log();


    static Log& getInstance();



    void init(std::string logname);

    //void log( const char * format, ... );

    //template<typename... Args> void log(const char * f, Args... args);

    template<typename... Args> void log(const char * f, Args... args)
    {
    	if (!init_)
    	{
    		return;
    	}
      printf(f, args...);
      syslog(LOG_INFO | LOG_LOCAL4, f, args...);
    }


    Log (Log const&)            = delete;
    void operator=(Log const&)  = delete;

private:
	Log();

	bool init_;
	std::string logname_;
};

#endif /* LOG_H_ */
