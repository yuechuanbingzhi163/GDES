#include "stdafx.h"
#include "LogManager.h"
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/configurator.h>

#ifdef _DEBUG
#pragma comment(lib,"log4cplusUD.lib")
#else
#pragma comment(lib,"log4cplusU.lib")
#endif
using namespace log4cplus;

LogManager* GLogManager = nullptr;

LogManager::LogManager()
{
}

LogManager::~LogManager()
{
}

void LogManager::Init()
{
	log4cplus::initialize();

	// config
	//PropertyConfigurator config(L"Config.properties");
	//PropertyConfigurator::doConfigure( L"./Resources/Log4Cplus/Config.properties" );

	log4cplus::SharedAppenderPtr consoleAppender( new log4cplus::ConsoleAppender() );
	consoleAppender->setName( L"ConsoleAppender" );
	std::auto_ptr<log4cplus::Layout> consoleLayout = std::auto_ptr<log4cplus::Layout>( new log4cplus::SimpleLayout() );
	consoleAppender->setLayout( consoleLayout );

	log4cplus::SharedAppenderPtr fileAppender( new log4cplus::FileAppender( L"DebriDefragmentationServer.log" ) );
	fileAppender->setName( L"FileAppender" );
	std::auto_ptr<log4cplus::Layout> fileLayout = std::auto_ptr<log4cplus::Layout>( new log4cplus::SimpleLayout() );
	fileAppender->setLayout( fileLayout );
	
	m_Logger = log4cplus::Logger::getInstance( L"myLoggerName" );
	m_Logger.addAppender( consoleAppender );
	m_Logger.addAppender( fileAppender );

	m_Logger.setLogLevel( log4cplus::DEBUG_LOG_LEVEL );
}