#include "StdAfx.h"
#include "LogHelper.h"

/**
* 参考来源: http://blog.csdn.net/fksec/article/details/41959005
*/

static log4cplus::tstring file_name_add_prefix(log4cplus::tstring& old_name,log4cplus::tstring prefix_flie)
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

static void change_log_file_name(log4cplus::tchar* prop_file, log4cplus::tchar* prefix_log_flie)
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

//#define DO_LOGGER(log_level/*, file, line, function, format_msg, __VA_ARGS__*/)    \
//	int retval = -1;                                                      \
//	try                                                                   \
//{                                                                     \
//	if (m_logger.isEnabledFor(log_level))                             \
//{                                                                 \
//	const log4cplus::tchar * msg = NULL;                          \
//	log4cplus::helpers::snprintf_buf buf;                         \
//	std::va_list args;                                            \
//	do                                                            \
//{                                                             \
//	va_start(args, format_msg);                               \
//	retval = buf.print_va_list(msg, format_msg, args);        \
//	va_end(args);                                             \
//} while (retval == -1);                                       \
//	m_logger.forcedLog(log_level, msg, file, line, function);     \
//}                                                                 \
//	retval = 0;                                                       \
//}                                                                     \
//	catch (...) {}                                                          
//
//void cruise_log4cplus::Fatal(const char* file, const int line, const char* function,
//		   const log4cplus::tchar* format_msg, ...)
//{   DO_LOGGER(log4cplus::FATAL_LOG_LEVEL);   }
//
//void cruise_log4cplus::Error(const char* file, const int line, const char* function,
//		   const log4cplus::tchar* format_msg, ...)
//{   DO_LOGGER(log4cplus::ERROR_LOG_LEVEL);   }
//
//void cruise_log4cplus::Warn(const char* file, const int line, const char* function,
//		  const log4cplus::tchar* format_msg, ...)
//{   DO_LOGGER(log4cplus::WARN_LOG_LEVEL);   }
//
//void cruise_log4cplus::Info(const char* file, const int line, const char* function,
//		  const log4cplus::tchar* format_msg, ...)
//{   DO_LOGGER(log4cplus::INFO_LOG_LEVEL);   }
//
//void cruise_log4cplus::Debug(const char* file, const int line, const char* function,
//		   const log4cplus::tchar* format_msg, ...)
//{   DO_LOGGER(log4cplus::DEBUG_LOG_LEVEL);   }
//
//void cruise_log4cplus::Trace(const char* file, const int line, const char* function,
//		   const log4cplus::tchar* format_msg, ...)
//{   DO_LOGGER(log4cplus::TRACE_LOG_LEVEL);   }

void cruise_log4cplus::log_init(log4cplus::tchar* prop_file, 
			  log4cplus::tchar* prefix_log_flie,
			  log4cplus::tchar* sub_logger_name)
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

void cruise_log4cplus::log_uinit()
{
	LOG4CPLUS_INFO(m_logger, LOG4CPLUS_TEXT("Log System Stop."));

	log4cplus::Logger::shutdown();
}

cruise_log4cplus::cruise_log4cplus()
{
	m_origin_locale = std::locale::global(std::locale("chs"));
}

cruise_log4cplus::~cruise_log4cplus()
{
	std::locale::global(m_origin_locale);
}

cruise_log4cplus::cruise_log4cplus(const cruise_log4cplus& obj) : m_logger(obj.m_logger)
{

}

cruise_log4cplus& cruise_log4cplus::operator=(const cruise_log4cplus& obj)
{ 
	m_logger = obj.m_logger;
	return *this;
}

cruise_log4cplus& cruise_log4cplus::get_singleton()
{
	static cruise_log4cplus _instance;
	return _instance;
}

cruise_log4cplus* cruise_log4cplus::get_singleton_ptr()
{   return &get_singleton();   }

log4cplus::Logger& cruise_log4cplus::get_logger()
{   return get_singleton().m_logger;   }


void log_init(tchar* prop_file, tchar* prefix_log_flie, tchar* sub_logger_name)
{
	g_Log4cplus.log_init(prop_file, prefix_log_flie, sub_logger_name);
}

void log_uinit()
{
	g_Log4cplus.log_uinit();
}
