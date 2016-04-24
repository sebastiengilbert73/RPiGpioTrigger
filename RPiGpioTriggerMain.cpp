#include <iostream>
#include <tinyxml.h>
#include <string>
#include <sstream>
#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <stdexcept> // runtime_error
#include "RPiGpioTrigger.h"


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
		
		TiXmlElement* physicalPinElmPtr = rootElmPtr->FirstChildElement("PhysicalPin");
		if (physicalPinElmPtr == NULL)
		{
			stringstream msg;
			msg << "main(): Could not find XML element 'PhysicalPin' in file '" << configFilename << "'";
			LOG4CXX_ERROR(loggerPtr, msg.str());
			throw runtime_error(msg.str().c_str());
		}
		int physicalPin = atoi(physicalPinElmPtr->GetText());
		
		TiXmlElement* systemCallElmPtr = rootElmPtr->FirstChildElement("SystemCallOnEvent");
		if (systemCallElmPtr == NULL)
		{
			stringstream msg;
			msg << "main(): Could not find XML element 'SystemCallOnEvent' in file '" << configFilename << "'";
			LOG4CXX_ERROR(loggerPtr, msg.str());
			throw runtime_error(msg.str().c_str());
		}
		string systemCallOnEvent = systemCallElmPtr->GetText();
		
		TiXmlElement* triggerStateElmPtr = rootElmPtr->FirstChildElement("TriggerState");
		if (triggerStateElmPtr == NULL)
		{
			stringstream msg;
			msg << "main(): Could not find XML element 'TriggerState' in file '" << configFilename << "'";
			LOG4CXX_ERROR(loggerPtr, msg.str());
			throw runtime_error(msg.str().c_str());
		}
		string triggerState = triggerStateElmPtr->GetText();
		if (triggerState != "HIGH" && triggerState != "LOW")
		{
			stringstream msg;
			msg << "main(): 'TriggerState' element in file '" << configFilename << "' has unrecognized vale '" << triggerState << "'. Please use 'HIGH' or 'LOW'";
			LOG4CXX_ERROR(loggerPtr, msg.str());
			throw runtime_error(msg.str().c_str());
		}
		
		TiXmlElement* sleepTimeInLoopElmPtr = rootElmPtr->FirstChildElement("SleepTimeInLoop");
		if (sleepTimeInLoopElmPtr == NULL)
		{
			stringstream msg;
			msg << "main(): Could not find XML element 'SleepTimeInLoop' in file '" << configFilename << "'";
			LOG4CXX_ERROR(loggerPtr, msg.str());
			throw runtime_error(msg.str().c_str());
		}
		double sleepTimeInLoopInSeconds = atof(sleepTimeInLoopElmPtr->GetText());
		
		TiXmlElement* minimumDelayBetweenTriggersInSecondsElmPtr = rootElmPtr->FirstChildElement("MinimumDelayBetweenTriggers");
		if (minimumDelayBetweenTriggersInSecondsElmPtr == NULL)
		{
			stringstream msg;
			msg << "main(): Could not find XML element 'MinimumDelayBetweenTriggers' in file '" << configFilename << "'";
			LOG4CXX_ERROR(loggerPtr, msg.str());
			throw runtime_error(msg.str().c_str());
		}
		double minimumDelayBetweenTriggersInSeconds = atof(minimumDelayBetweenTriggersInSecondsElmPtr->GetText());
		
		TiXmlElement* delayToLogInactivityElmPtr = rootElmPtr->FirstChildElement("DelayToLogInactivity");
		if (delayToLogInactivityElmPtr == NULL)
		{
			stringstream msg;
			msg << "main(): Could not find XML element 'DelayToLogInactivity' in file '" << configFilename << "'";
			LOG4CXX_ERROR(loggerPtr, msg.str());
			throw runtime_error(msg.str().c_str());
		}
		double delayToLogInactivity = atof(delayToLogInactivityElmPtr->GetText());
		
		// Create event trigger
		RPiGpioTrigger eventTrigger(physicalPin, systemCallOnEvent, triggerState == "HIGH", sleepTimeInLoopInSeconds, minimumDelayBetweenTriggersInSeconds,
			delayToLogInactivity, loggerPtr);
	
		cout << "Done!" << endl;
	}
	catch (runtime_error e)
	{
		cout << "Caught exception: " << e.what() << endl;
	}
}
