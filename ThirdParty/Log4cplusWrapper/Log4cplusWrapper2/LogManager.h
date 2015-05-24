#pragma once

#include "LoggerProperties.h"

#define DDLOG_FATAL(MESSAGE) LOG4CPLUS_FATAL( GLogManager->m_Logger, MESSAGE )
#define DDLOG_ERROR(MESSAGE) LOG4CPLUS_ERROR( GLogManager->m_Logger, MESSAGE )
#define DDLOG_WARN(MESSAGE) LOG4CPLUS_WARN( GLogManager->m_Logger, MESSAGE )
#define DDLOG_INFO(MESSAGE) LOG4CPLUS_INFO( GLogManager->m_Logger, MESSAGE )
#define DDLOG_DEBUG(MESSAGE) LOG4CPLUS_DEBUG( GLogManager->m_Logger, MESSAGE )

class LogManager
{
public:
	LogManager();
	~LogManager();

	void Init();
	log4cplus::Logger* GetLogger() { return &m_Logger; }

	log4cplus::Logger	m_Logger;
};

extern LogManager* GLogManager;