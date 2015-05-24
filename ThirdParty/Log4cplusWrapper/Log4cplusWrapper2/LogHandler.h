#pragma once

#include "log4cplus/logger.h"
#include "log4cplus/loglevel.h"
#include "log4cplus/layout.h"
#include "log4cplus/ndc.h"
#include "log4cplus/loggingmacros.h"

// appenders
#include "log4cplus/consoleappender.h"
#include "log4cplus/fileappender.h"
#include "log4cplus/socketappender.h"

#define MAX_LOG_LENGTH 1024

#define LOG_FATAL(...) { LogHandler::LogFunction( LogHandler::FATAL_LOG, LogHandler::g_Logger, __VA_ARGS__); }
#define LOG_ERROR(...) { LogHandler::LogFunction( LogHandler::ERROR_LOG, LogHandler::g_Logger, __VA_ARGS__); }
#define LOG_WARN(...) { LogHandler::LogFunction( LogHandler::WARN_LOG, LogHandler::g_Logger, __VA_ARGS__); }
#define LOG_INFO(...) { LogHandler::LogFunction( LogHandler::INFO_LOG, LogHandler::g_Logger, __VA_ARGS__); }
#define LOG_DEBUG(...) { LogHandler::LogFunction( LogHandler::DEBUG_LOG, LogHandler::g_Logger, __VA_ARGS__); }

namespace LogHandler
{
	enum LOG_TYPE
	{
		FATAL_LOG,
		ERROR_LOG,
		WARN_LOG,
		INFO_LOG,
		DEBUG_LOG,
	};

	enum APPENDER_TYPE
	{
		APPENDER_CONSOLE,
		APPENDER_FILE,
		APPENDER_SOCKET,
	};

	enum LAYOUT_TYPE
	{
		SIMPLE_LAYOUT,
		TTCC_LAYOUT,
		PATTERN_LAYOUT,
	};
	
	static log4cplus::Logger	g_Logger;
	static char					g_LogBuffer[MAX_LOG_LENGTH];
	static wchar_t				g_LogBufferW[MAX_LOG_LENGTH];

	static void Init()
	{
		log4cplus::initialize();
		g_Logger = log4cplus::Logger::getInstance( LOG4CPLUS_TEXT( "log" ) );

		memset( g_LogBuffer, 0, sizeof( g_LogBuffer ) );
		memset( g_LogBufferW, 0, sizeof( g_LogBufferW ) );
	}

	static void SetAppenderProperties( 
		log4cplus::SharedAppenderPtr appender,
		const log4cplus::tstring& name,
		LAYOUT_TYPE layoutType = SIMPLE_LAYOUT,
		log4cplus::LogLevel logLevel = log4cplus::ALL_LOG_LEVEL,
		const log4cplus::tstring & pattern = L"" )
	{
		// set name
		appender->setName( name );

		// set layout
		std::auto_ptr<log4cplus::Layout> layout;
		switch ( layoutType )
		{
		case LogHandler::SIMPLE_LAYOUT:
			layout = std::auto_ptr<log4cplus::Layout>( new log4cplus::SimpleLayout( ) );
			break;
		case LogHandler::TTCC_LAYOUT:
			layout = std::auto_ptr<log4cplus::Layout>( new log4cplus::TTCCLayout( ) );
			break;
		case LogHandler::PATTERN_LAYOUT:
			layout = std::auto_ptr<log4cplus::Layout>( new log4cplus::PatternLayout( pattern ) );
			break;
		default:
			break;
		}
		appender->setLayout( layout );

		// set loglevel
		appender->setThreshold( logLevel );
	}

	// manual addition
	static void AddAppender( log4cplus::SharedAppenderPtr newAppender )
	{
		g_Logger.addAppender( newAppender );
	}

	// auto generation
	static void AddConsoleAppender( 
		const log4cplus::tstring& appenderName,
		LAYOUT_TYPE layoutType = SIMPLE_LAYOUT,
		log4cplus::LogLevel logLevel = log4cplus::ALL_LOG_LEVEL
		 )
	{
		// create appender
		log4cplus::SharedAppenderPtr newAppender( new log4cplus::ConsoleAppender() );

		// set properties
		SetAppenderProperties( newAppender, appenderName, layoutType, logLevel );

		// add appender
		AddAppender( newAppender );
	}

	static void AddFileAppender(
		const log4cplus::tstring& appenderName,
		const log4cplus::tstring& fileName,
		LAYOUT_TYPE layoutType = SIMPLE_LAYOUT,
		log4cplus::LogLevel logLevel = log4cplus::ALL_LOG_LEVEL
		)
	{
		// create appender
		log4cplus::SharedAppenderPtr newAppender( new log4cplus::FileAppender( fileName ) );

		// set properties
		SetAppenderProperties( newAppender, appenderName, layoutType, logLevel );

		// add appender
		AddAppender( newAppender );
	}

	static void LogFunction( LOG_TYPE type, log4cplus::Logger inputLogger, char* fmt, ... )
	{
		va_list ap;

		va_start( ap, fmt );
		vsprintf_s( g_LogBuffer, fmt, ap );
		va_end( ap );

		switch ( type )
		{
		case FATAL_LOG:
			LOG4CPLUS_FATAL( inputLogger, g_LogBuffer );
			break;
		case ERROR_LOG:
			LOG4CPLUS_ERROR( inputLogger, g_LogBuffer );
			break;
		case WARN_LOG:
			LOG4CPLUS_WARN( inputLogger, g_LogBuffer );
			break;
		case INFO_LOG:
			LOG4CPLUS_INFO( inputLogger, g_LogBuffer );
			break;
		case DEBUG_LOG:
			LOG4CPLUS_DEBUG( inputLogger, g_LogBuffer );
			break;
		default:
			break;
		}
	}

	static void LogFunction( LOG_TYPE type, log4cplus::Logger inputLogger, wchar_t* fmt, ... )
	{
		va_list ap;

		va_start( ap, fmt );
		swprintf_s( g_LogBufferW, fmt, ap );
		va_end( ap );

		switch ( type )
		{
		case FATAL_LOG:
			LOG4CPLUS_FATAL( inputLogger, g_LogBufferW );
			break;
		case ERROR_LOG:
			LOG4CPLUS_ERROR( inputLogger, g_LogBufferW );
			break;
		case WARN_LOG:
			LOG4CPLUS_WARN( inputLogger, g_LogBufferW );
			break;
		case INFO_LOG:
			LOG4CPLUS_INFO( inputLogger, g_LogBufferW );
			break;
		case DEBUG_LOG:
			LOG4CPLUS_DEBUG( inputLogger, g_LogBufferW );
			break;
		default:
			break;
		}
	}


}