#include <iostream>
#include <tinyxml.h>
#include <string>
#include <sstream>
#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <stdexcept> // runtime_error


using namespace std;
using namespace log4cxx;
using namespace log4cxx::xml;
using namespace log4cxx::helpers;

// Define static logger variable
LoggerPtr loggerPtr(Logger::getLogger("RPiGpioTriggerMain"));

int main(int argc, char **argv)
{
	try
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
		TiXmlDocument configDoc(configFilename);
		if (!configDoc.LoadFile())
		{
			stringstream msg;
			msg << "main(): Could not load configuration file '" << configFilename << "'";
			LOG4CXX_ERROR(loggerPtr, msg.str());
			throw runtime_error(msg.str().c_str());
		}
		TiXmlElement* rootElmPtr = configDoc.FirstChildElement("RPiGPioTriggerMain");
		if (rootElmPtr == NULL)
		{
			stringstream msg;
			msg << "main(): Could not find XML element 'RPiGPioTriggerMain' at the root of file '" << configFilename << "'";
			LOG4CXX_ERROR(loggerPtr, msg.str());
			throw runtime_error(msg.str().c_str());
		}
	
		cout << "Done!" << endl;
	}
	catch (runtime_error e)
	{
		cout << "Caught exception: " << e.what() << endl;
	}
}
