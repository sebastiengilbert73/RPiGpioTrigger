#include <iostream>
#include <tinyxml.h>
#include <string>
#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>


using namespace std;
using namespace log4cxx;
using namespace log4cxx::xml;
using namespace log4cxx::helpers;

// Define static logger variable
LoggerPtr loggerPtr(Logger::getLogger("RPiGpioTriggerMain"));

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		cout << "Usage: RPiGpioTriggerMain [configFilename]" << endl;
		return -1;
	}
	
	DOMConfigurator::configure("RPiGpioTriggerMainLog4cxxConfig.xml");
	LOG4CXX_DEBUG(loggerPtr, "main()");
	
	// Read config file
	string configFilename = argv[1];
	
	cout << "Done!" << endl;
}
