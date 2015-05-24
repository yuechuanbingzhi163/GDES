#pragma once

/**
* 来源: http://blog.csdn.net/fksec/article/details/41959005
*/

#ifdef CRUISE_LOG4CPLUS

#include <log4cplus/logger.h>
#include <log4cplus/helpers/snprintf.h>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

class cruise_log4cplus
{
private:
	cruise_log4cplus(){}
	cruise_log4cplus(const cruise_log4cplus& obj)
		: m_logger(obj.m_logger){}
	cruise_log4cplus& operator=(const cruise_log4cplus& obj)
	{ m_logger = obj.m_logger; }
	//virtual ~cruise_log4cplus(){}

private:
#define DO_LOGGER(log_level/*, file, line, function, format_msg, __VA_ARGS__*/)    \
	int retval = -1;                                                      \
	try                                                                   \
	{                                                                     \
	if (m_logger.isEnabledFor(log_level))                             \
	{                                                                 \
	const log4cplus::tchar * msg = NULL;                          \
	log4cplus::helpers::snprintf_buf buf;                         \
	std::va_list args;                                            \
	do                                                            \
	{                                                             \
	va_start(args, format_msg);                               \
	retval = buf.print_va_list(msg, format_msg, args);        \
	va_end(args);                                             \
	   } while (retval == -1);                                       \
	   m_logger.forcedLog(log_level, msg, file, line, function);     \
	   }                                                                 \
	   retval = 0;                                                       \
	   }                                                                     \
	   catch (...) {}                                                          

public:
	void Fatal(const char* file, const int line, const char* function,
		const log4cplus::tchar* format_msg, ...)
	{   DO_LOGGER(log4cplus::FATAL_LOG_LEVEL);   }

	void Error(const char* file, const int line, const char* function,
		const log4cplus::tchar* format_msg, ...)
	{   DO_LOGGER(log4cplus::ERROR_LOG_LEVEL);   }

	void Warn(const char* file, const int line, const char* function,
		const log4cplus::tchar* format_msg, ...)
	{   DO_LOGGER(log4cplus::WARN_LOG_LEVEL);   }

	void Info(const char* file, const int line, const char* function,
		const log4cplus::tchar* format_msg, ...)
	{   DO_LOGGER(log4cplus::INFO_LOG_LEVEL);   }

	void Debug(const char* file, const int line, const char* function,
		const log4cplus::tchar* format_msg, ...)
	{   DO_LOGGER(log4cplus::DEBUG_LOG_LEVEL);   }

	void Trace(const char* file, const int line, const char* function,
		const log4cplus::tchar* format_msg, ...)
	{   DO_LOGGER(log4cplus::TRACE_LOG_LEVEL);   }

public:
	void log_init(log4cplus::tchar* prop_file, 
		log4cplus::tchar* prefix_log_flie = NULL,
		log4cplus::tchar* sub_logger_name = NULL)
	{
		log4cplus::initialize();
		m_logger = log4cplus::Logger::getRoot();

		if (NULL == prop_file) return;

		try
		{
			if (NULL != prefix_log_flie && '\0' != prefix_log_flie[0])
				change_log_file_name(prop_file, prefix_log_flie);
			else
				log4cplus::PropertyConfigurator::doConfigure(prop_file);
			//log4cplus::ConfigureAndWatchThread cfg_thread(prop_file, 24*60*60*1000);

			if (NULL != sub_logger_name && '\0' != sub_logger_name[0]
			&& log4cplus::Logger::exists(sub_logger_name))
				m_logger = log4cplus::Logger::getInstance(sub_logger_name);
			//else
			//    m_logger = log4cplus::Logger::getRoot();

			LOG4CPLUS_INFO(m_logger, LOG4CPLUS_TEXT("Log System Start."));
		}
		catch (...)
		{
			LOG4CPLUS_FATAL(m_logger, LOG4CPLUS_TEXT("Exception occured..."));
			abort();
		}
	}

	void log_uinit()
	{
		LOG4CPLUS_INFO(m_logger, LOG4CPLUS_TEXT("Log System Stop."));

		log4cplus::Logger::shutdown();
	}

private:
	void change_log_file_name(log4cplus::tchar* prop_file,
		log4cplus::tchar* prefix_log_flie)
	{
		log4cplus::PropertyConfigurator prop_config(prop_file);
		log4cplus::helpers::Properties& prop
			= const_cast<log4cplus::helpers::Properties &>(
			prop_config.getProperties());
		log4cplus::helpers::Properties appender_properties
			= prop.getPropertySubset(LOG4CPLUS_TEXT("appender."));
		std::vector<log4cplus::tstring> appenders_props
			= appender_properties.propertyNames();

		for (std::vector<log4cplus::tstring>::iterator
			it = appenders_props.begin(); it != appenders_props.end(); ++it)
		{
			if (log4cplus::tstring::npos != it->find(
				LOG4CPLUS_TEXT(".File")))
			{
				log4cplus::tstring file_name_old
					= appender_properties.getProperty(*it);
				log4cplus::tstring file_name_new
					= file_name_add_prefix(file_name_old, prefix_log_flie);

				prop.setProperty(LOG4CPLUS_TEXT("appender.") + *it, file_name_new);
			}
		}

		prop_config.configure();
	}

	log4cplus::tstring file_name_add_prefix(log4cplus::tstring& old_name,
		log4cplus::tstring prefix_flie)
	{
		size_t pos = 0;
		if (log4cplus::tstring::npos != (
			pos = old_name.find_last_of(LOG4CPLUS_TEXT('\\')))
			|| log4cplus::tstring::npos != (
			pos = old_name.find_last_of(LOG4CPLUS_TEXT('/'))))
		{
			++pos;
		}

		prefix_flie.append(LOG4CPLUS_TEXT("_"));
		return old_name.insert(pos, prefix_flie);
	}

public:
	static inline cruise_log4cplus& get_singleton()
	{
		static cruise_log4cplus _instance;
		return _instance;
	}

	static inline cruise_log4cplus* get_singleton_ptr()
	{   return &get_singleton();   }

	static inline log4cplus::Logger& get_logger()
	{   return get_singleton().m_logger;   }

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


#else // !CRUISE_LOG4CPLUS

#include <cstdio>   // printf
#include <cstdlib>
#include <iostream> // std::cout, std::cerr, std::endl
#pragma warning(disable:4996)


/************************************************************************/
/* MACRO: _T, _TEXT, TEXT                                                                     */
/* TYPE: tchar , tstring                                                                            */
/************************************************************************/
#ifndef _T
#if UNICODE
#define _T(x)   L ## x
#else
#define _T(x)   x
#endif
#endif

#ifndef _TEXT
#define _TEXT(x) _T(x)
#endif

#ifndef TEXT
#define TEXT(x) _T(x)
#endif

#if UNICODE
typedef wchar_t tchar;
typedef std::wstring tstring;
typedef std::wistream tistream;
typedef std::wostream tostream;
//static tostream& tcout = std::wcout;
//static tostream& tcerr = std::wcerr;
#define tprintf wprintf
#else
typedef char tchar;
typedef std::string tstring;
typedef std::istream tistream;
typedef std::ostream tostream;
//static tostream& tcout = std::cout;
//static tostream& tcerr = std::cerr;
#define tprintf printf
#endif
//typedef std::basic_string<tchar, std::char_traits<tchar>, std::allocator<tchar>> tstring;
//typedef std::basic_ostream<tchar, std::char_traits<tchar> > tostream;
//typedef std::basic_istream<tchar, std::char_traits<tchar> > tistream;

static tostream& get_tcout()
{
#if UNICODE
	static tostream& s_tcout = std::wcout;
#else
	static tostream& s_tcout = std::cout;
#endif
	return s_tcout;
}
#define tcout   get_tcout()

static tostream& get_tcerr()
{
#if UNICODE
	static tostream& s_tcerr = std::wcerr;
#else
	static tostream& s_tcerr = std::cerr;
#endif
	return s_tcerr;
}
#define tcerr   get_tcerr()

static std::string ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
	setlocale(LC_ALL, "");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs(_Dest, _Source, _Dsize);
	std::string result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

static std::wstring s2ws(const std::string& s)
{
	setlocale(LC_ALL, "");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	std::wstring result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, "C");
	return result;
}

static tstring ws2t(const std::wstring ws)
{
#if UNICODE
	return ws;
#else
	return ws2s(ws);
#endif
}

static tstring s2t(const std::string s)
{
#if UNICODE
	return s2ws(s);
#else
	return s;
#endif
}

static std::string t2s(const tstring s)
{
#if UNICODE
	return ws2s(s);
#else
	return s;
#endif
}

#define CRUISE_C_STR_TO_TSTRING(STRING)  s2t(STRING)
#define CRUISE_STRING_TO_TSTRING(STRING) s2t(STRING)
#define CRUISE_TSTRING_TO_STRING(STRING) t2s(STRING)

/************************************************************************/
/* CRUISE_MACRO_FUNCTION()                                                                */
/* __FILE__, __LINE__                                                                              */
/************************************************************************/
//#define CRUISE_MACRO_FILE()     __FILE__
//#define CRUISE_MACRO_LINE()     __LINE__

#define CRUISE_MACRO_FUNCTION()     0
#if defined(_MSC_VER) && _MSC_VER > 1400
#       undef CRUISE_MACRO_FUNCTION
#       define CRUISE_MACRO_FUNCTION() __FUNCSIG__
#elif defined(__GNUC__)                                         \
	&& __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)
#       undef CRUISE_MACRO_FUNCTION
#       define CRUISE_MACRO_FUNCTION() __PRETTY_FUNCTION__
#elif defined(__BORLANDC__) && __BORLANDC__ >= 0x0650
#       undef CRUISE_MACRO_FUNCTION
#       define CRUISE_MACRO_FUNCTION() __FUNCTION__
//#       define CRUISE_MACRO_FUNCTION() __FUNC__ //C++ BuilderX
#elif (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 19901L) \
	|| (defined(__GNUC__) && __GNUC__ > 3                       \
	|| (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#       undef CRUISE_MACRO_FUNCTION
#       define CRUISE_MACRO_FUNCTION() __func__
#endif

#if defined (__GNUC__)                                          \
	&& (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8))
#   define CRUISE_CALLER_FILE()     __builtin_FILE ()
#   define CRUISE_CALLER_LINE()     __builtin_LINE ()
#   define CRUISE_CALLER_FUNCTION() __builtin_FUNCTION ()
#else
#   define CRUISE_CALLER_FILE()     (NULL)
#   define CRUISE_CALLER_LINE()     (-1)
#   define CRUISE_CALLER_FUNCTION() (NULL)
#endif


class cruise_trace_logger
{
public:
	cruise_trace_logger(const tstring& _msg,
		const char* _file = CRUISE_CALLER_FILE(),
		int _line = CRUISE_CALLER_LINE(),
		char const * _function = CRUISE_CALLER_FUNCTION())
		: msg(_msg), file(_file), function(_function), line(_line)
	{
		tcout << "ENTER: " << msg.c_str()
			<< " [" << file << ":" << line << ":" << function << "]"
			<< std::endl;
	}

	~cruise_trace_logger()
	{
		tcout << "EXIT:  " << msg.c_str()
			<< " [" << file << ":" << line << ":" << function << "]"
			<< std::endl;
	}

private:
	cruise_trace_logger(cruise_trace_logger const &);
	cruise_trace_logger & operator = (cruise_trace_logger const &);

	tstring msg;
	const char* file;
	const char* function;
	int line;
};


/************************************************************************/
/*  Log out  macro                                                                                 */
/************************************************************************/
#define LOG_METHOD(logEvent)                        \
	cruise_trace_logger _trace_logger(logEvent, \
	__FILE__, __LINE__, CRUISE_MACRO_FUNCTION())

#define CRUISE_DEBUG_COUT(logEvent)                 \
	tcout << logEvent << " ["                       \
	<< __FILE__ << ":" << __LINE__<< ":"        \
	<< CRUISE_MACRO_FUNCTION() << "]" << std::endl
#define CRUISE_DEBUG_CERR(logEvent)                 \
	tcerr << logEvent << " ["                       \
	<< __FILE__ << ":" << __LINE__<< ":"        \
	<< CRUISE_MACRO_FUNCTION() << "]" << std::endl

#define LOG_FATAL(logEvent)  CRUISE_DEBUG_CERR(logEvent)
#define LOG_ERROR(logEvent)  CRUISE_DEBUG_CERR(logEvent)
#define LOG_WARN(logEvent)   CRUISE_DEBUG_COUT(logEvent)
#define LOG_INFO(logEvent)   CRUISE_DEBUG_COUT(logEvent)
#define LOG_DEBUG(logEvent)  CRUISE_DEBUG_COUT(logEvent)
#define LOG_TRACE(logEvent)  CRUISE_DEBUG_COUT(logEvent)

#if  _MSC_VER > 1400
#define CRUISE_DEBUG_PRINTF(logFmt, ...)                                \
	tprintf(logFmt, __VA_ARGS__);                                       \
	tprintf(_T(" [%s:%d:%s]\n"),                                        \
	CRUISE_C_STR_TO_TSTRING(__FILE__).c_str(), __LINE__,            \
	CRUISE_C_STR_TO_TSTRING(CRUISE_MACRO_FUNCTION()).c_str());

#define LOG_FATAL_FMT(logFmt, ...) CRUISE_DEBUG_PRINTF(logFmt, __VA_ARGS__)
#define LOG_ERROR_FMT(logFmt, ...) CRUISE_DEBUG_PRINTF(logFmt, __VA_ARGS__)
#define LOG_WARN_FMT(logFmt, ...)  CRUISE_DEBUG_PRINTF(logFmt, __VA_ARGS__)
#define LOG_INFO_FMT(logFmt, ...)  CRUISE_DEBUG_PRINTF(logFmt, __VA_ARGS__)
#define LOG_DEBUG_FMT(logFmt, ...) CRUISE_DEBUG_PRINTF(logFmt, __VA_ARGS__)
#define LOG_TRACE_FMT(logFmt, ...) CRUISE_DEBUG_PRINTF(logFmt, __VA_ARGS__)
#endif


#if defined (__GNUC__) && __GNUC__ >= 3
#  define CRUISE_BUILTIN_EXPECT(exp, c) __builtin_expect ((exp), (c))
#else
#  define CRUISE_BUILTIN_EXPECT(exp, c) (exp)
#endif
#define CRUISE_LIKELY(cond) CRUISE_BUILTIN_EXPECT(!! (cond), 1)
#define CRUISE_UNLIKELY(cond) CRUISE_BUILTIN_EXPECT(!! (cond), 0)

#define LOG_ASSERT(cond)                          \
	if (CRUISE_UNLIKELY(!cond))                   \
	LOG_FATAL("failed condition: "#cond);

#endif

static void cruise_set_loc()
{
#if (defined(__linux__) || defined(__linux)        \
	|| defined(linux) || defined(__gnu_linux__))   \
	&& defined(__GNUC_)
	std::ios::sync_with_stdio(false);   // Linux gcc.
#endif
	std::locale::global(std::locale(""));
#if defined(__MINGW32__)           // Windows32 by mingw compiler 
	setlocale(LC_CTYPE, "");       // MinGW gcc.
#endif
	tcout.imbue(std::locale(""));
}

static void log_init(tchar* prop_file,
					 tchar* prefix_log_flie = NULL, tchar* sub_logger_name = NULL)
{
#ifdef CRUISE_LOG4CPLUS
	g_Log4cplus.log_init(prop_file, prefix_log_flie, sub_logger_name);
#else
	cruise_set_loc();
#endif
}

static void log_uinit()
{
#ifdef CRUISE_LOG4CPLUS
	g_Log4cplus.log_uinit();
#endif
}
