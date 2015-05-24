#pragma once

/**
* 来源: http://blog.csdn.net/fksec/article/details/41959005
*/

#include <log4cplus/logger.h>
#include <log4cplus/helpers/snprintf.h>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

class cruise_log4cplus
{
public:
	static cruise_log4cplus& get_singleton();
	static cruise_log4cplus* get_singleton_ptr();
	static log4cplus::Logger& get_logger();

public:
	void log_init(log4cplus::tchar* prop_file, log4cplus::tchar* prefix_log_flie = NULL, log4cplus::tchar* sub_logger_name = NULL);
	void log_uinit();

//public:
//	void Fatal(const char* file, const int line, const char* function, const log4cplus::tchar* format_msg, ...);
//	void Error(const char* file, const int line, const char* function, const log4cplus::tchar* format_msg, ...);
//	void Warn(const char* file, const int line, const char* function, const log4cplus::tchar* format_msg, ...);
//	void Info(const char* file, const int line, const char* function, const log4cplus::tchar* format_msg, ...);
//	void Debug(const char* file, const int line, const char* function, const log4cplus::tchar* format_msg, ...);
//	void Trace(const char* file, const int line, const char* function, const log4cplus::tchar* format_msg, ...);

private:
	cruise_log4cplus();
	cruise_log4cplus(const cruise_log4cplus& obj);
	cruise_log4cplus& operator=(const cruise_log4cplus& obj);
	//virtual ~cruise_log4cplus(){}
private:
	log4cplus::Logger m_logger;
};

#define g_Log4cplus cruise_log4cplus::get_singleton()
#define g_pLog4cplus cruise_log4cplus::get_singleton_ptr()
#define g_Logger cruise_log4cplus::get_logger()

/************************************************************************/
/*  Log out  macro                                                                                 */
/************************************************************************/
#define LOG_ASSERT(cond)     LOG4CPLUS_ASSERT(g_Logger, cond)
#define LOG_METHOD(logEvent) LOG4CPLUS_TRACE_METHOD(g_Logger, logEvent)

#define LOG_FATAL(logEvent)  LOG4CPLUS_FATAL(g_Logger, logEvent)
#define LOG_ERROR(logEvent)  LOG4CPLUS_ERROR(g_Logger, logEvent)
#define LOG_WARN(logEvent)   LOG4CPLUS_WARN(g_Logger, logEvent)
#define LOG_INFO(logEvent)   LOG4CPLUS_INFO(g_Logger, logEvent)
#define LOG_DEBUG(logEvent)  LOG4CPLUS_DEBUG(g_Logger, logEvent)
#define LOG_TRACE(logEvent)  LOG4CPLUS_TRACE(g_Logger, logEvent)

#if  _MSC_VER > 1400
#define LOG_FATAL_FMT(logFmt, ...) LOG4CPLUS_FATAL_FMT(g_Logger, logFmt, __VA_ARGS__)
#define LOG_ERROR_FMT(logFmt, ...) LOG4CPLUS_ERROR_FMT(g_Logger, logFmt, __VA_ARGS__)
#define LOG_WARN_FMT(logFmt, ...)  LOG4CPLUS_WARN_FMT(g_Logger, logFmt, __VA_ARGS__)
#define LOG_INFO_FMT(logFmt, ...)  LOG4CPLUS_INFO_FMT(g_Logger, logFmt, __VA_ARGS__)
#define LOG_DEBUG_FMT(logFmt, ...) LOG4CPLUS_DEBUG_FMT(g_Logger, logFmt, __VA_ARGS__)
#define LOG_TRACE_FMT(logFmt, ...) LOG4CPLUS_TRACE_FMT(g_Logger, logFmt, __VA_ARGS__)
#endif


/************************************************************************/
/* MACRO: _T, _TEXT, TEXT                                                                     */
/* TYPE: tchar , tstring                                                                            */
/************************************************************************/
#ifndef _T
#define _T(x)   LOG4CPLUS_TEXT(x)
#endif

#ifndef _TEXT
#define _TEXT(x) _T(x)
#endif

#ifndef TEXT
#define TEXT(x) _T(x)
#endif

using log4cplus::tchar;
using log4cplus::tstring;
using log4cplus::tistream;
using log4cplus::tostream;
using log4cplus::tcout;
using log4cplus::tcerr;

#define CRUISE_C_STR_TO_TSTRING(STRING)  LOG4CPLUS_C_STR_TO_TSTRING(STRING)
#define CRUISE_STRING_TO_TSTRING(STRING) LOG4CPLUS_STRING_TO_TSTRING(STRING)
#define CRUISE_TSTRING_TO_STRING(STRING) LOG4CPLUS_TSTRING_TO_STRING(STRING)

#define CRUISE_BUILTIN_EXPECT(exp, c)   LOG4CPLUS_BUILTIN_EXPECT(exp, c)
#define CRUISE_LIKELY(cond)             LOG4CPLUS_LIKELY(cond)
#define CRUISE_UNLIKELY(cond)           LOG4CPLUS_UNLIKELY(cond)

//#define CRUISE_MACRO_FILE()             __FILE__
//#define CRUISE_MACRO_LINE()             __LINE__
#define CRUISE_MACRO_FUNCTION()         LOG4CPLUS_MACRO_FUNCTION()
#define CRUISE_CALLER_FILE()            LOG4CPLUS_CALLER_FILE()
#define CRUISE_CALLER_LINE()            LOG4CPLUS_CALLER_LINE()
#define CRUISE_CALLER_FUNCTION()        LOG4CPLUS_CALLER_FUNCTION()

extern void log_init(tchar* prop_file, tchar* prefix_log_flie = NULL, tchar* sub_logger_name = NULL);
extern void log_uinit();