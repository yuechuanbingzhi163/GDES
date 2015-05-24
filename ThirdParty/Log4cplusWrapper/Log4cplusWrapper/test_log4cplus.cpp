
#include <log4cplus/logger.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/ndc.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/property.h>
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;

/**
* 参考：http://pyhcx.blog.51cto.com/713166/143549/
*/
int test_logcplus()
{    
	/* step 1: Instantiate an appender object */
	SharedAppenderPtr _append(new FileAppender(LOG4CPLUS_TEXT("Test.log")));
	_append->setName(LOG4CPLUS_TEXT("file log test"));

	/* step 2: Instantiate a layout object */
	std::auto_ptr<Layout> _layout(new TTCCLayout());

	/* step 3: Attach the layout object to the appender */
	_append->setLayout( _layout );

	/* step 4: Instantiate a logger object */
	Logger _logger = Logger::getInstance(LOG4CPLUS_TEXT("test"));

	/* step 5: Attach the appender object to the logger  */
	_logger.addAppender(_append);

	/* log activity */
	int i;
	for( i = 0; i < 5; ++i )
	{
		LOG4CPLUS_DEBUG(_logger, LOG4CPLUS_TEXT("Entering loop #") << i << LOG4CPLUS_TEXT("End line #"));
	}
	return 0;
}

int test_logcplus2()
{
	// 定义1个控制台的Appender,3个文件Appender 
	SharedAppenderPtr pConsoleAppender(new ConsoleAppender()); 
	SharedAppenderPtr pFileAppender1(new FileAppender(LOG4CPLUS_TEXT("c:\\testlog1.log"))); 
	SharedAppenderPtr pFileAppender2(new FileAppender(LOG4CPLUS_TEXT("c:\\testlog2.log"))); 
	SharedAppenderPtr pFileAppender3(new FileAppender(LOG4CPLUS_TEXT("c:\\testlog3.log"))); 

	// 定义一个简单的Layout,并绑定到 pFileAppender1 
	std::auto_ptr<Layout> pSimpleLayout(new SimpleLayout()); 
	pFileAppender1->setLayout(pSimpleLayout); 

	// 定义一个TTCLayout,并绑定到 pFileAppender2 
	std::auto_ptr<Layout> pTTCLayout(new TTCCLayout()); 
	pFileAppender2->setLayout(pTTCLayout); 

	// 定义一个PatternLayout,并绑定到 pFileAppender3 
	std::auto_ptr<Layout> pPatternLayout(new PatternLayout(LOG4CPLUS_TEXT("%d{%m/%d/%y %H:%M:%S}    - %m [%l]%n"))); 
	pFileAppender3->setLayout(pPatternLayout); 

	// 定义Logger 
	Logger pTestLogger = Logger::getInstance(LOG4CPLUS_TEXT("LoggerName"));    

	// 将需要关联Logger的Appender添加到Logger上 
	pTestLogger.addAppender(pConsoleAppender); 
	pTestLogger.addAppender(pFileAppender1); 
	pTestLogger.addAppender(pFileAppender2); 
	pTestLogger.addAppender(pFileAppender3); 

	// 输出日志信息 
	LOG4CPLUS_WARN(pTestLogger, LOG4CPLUS_TEXT("This is a <Warn> log message...")); 

	return 0; 
}


#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;
using namespace log4cplus::helpers;
using namespace log4cplus::thread;

static Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("log"));

void printDebug()
{
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("This is a DEBUG message"));
	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("This is a INFO message"));
	LOG4CPLUS_WARN(logger, LOG4CPLUS_TEXT("This is a WARN message"));
	LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("This is a ERROR message"));
	LOG4CPLUS_FATAL(logger, LOG4CPLUS_TEXT("This is a FATAL message"));
}

int test_logcplus3()
{
	//log4cplus::initialize();

	Logger logger = Logger::getRoot();
	PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));

	LOG4CPLUS_TRACE_METHOD(logger, LOG4CPLUS_TEXT("::printDebug()"));

	//log4cplus::Logger::shutdown();

	return 0;
}

#include "log.h"

int test_logcplus4()
{  
	log_init(LOG4CPLUS_TEXT("log4cplus.properties"));

	//for (int i = 0; i < 1000; ++i)
	{
		//LOG_METHOD(_TEXT("打开"));
		//LOG_ASSERT(true);
		//LOG_ASSERT(false);

		LOG_TRACE(_TEXT("Log4cplus library!") << 2);
		LOG_TRACE_FMT(_TEXT("%d, Log4cplus library!"), 3);
	}

	log_uinit();  
	return 0;  
}