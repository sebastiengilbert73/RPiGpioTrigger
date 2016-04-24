#include "RPiGpioTrigger.h"

using namespace std;

RPiGpioTrigger::RPiGpioTrigger()
{
	Create();
}

void RPiGpioTrigger::Create()
{
	_physicalPin = 0;
	_systemCallOnEvent = "";
	_triggerState = true;
	_sleepTimeInLoopInSeconds = 0;
	_minimumDelayBetweenTriggersInSeconds = 0;
	_delayToLogInactivityInSeconds = 0;
	_watchThread = 0;

}

RPiGpioTrigger::RPiGpioTrigger(int physicalPin, std::string systemCallOnEvent, bool triggerOnHigh, double sleepTimeInLoopInSeconds,
   double minimumDelayBetweenTriggersInSeconds, double delayToLogInactivityInSeconds, log4cxx::LoggerPtr loggerPtr)
{
	Create(physicalPin, systemCallOnEvent, triggerOnHigh, sleepTimeInLoopInSeconds,
		minimumDelayBetweenTriggersInSeconds, delayToLogInactivityInSeconds, loggerPtr);
}

void RPiGpioTrigger::Create(int physicalPin, std::string systemCallOnEvent, bool triggerOnHigh, double sleepTimeInLoopInSeconds,
	   double minimumDelayBetweenTriggersInSeconds, double delayToLogInactivityInSeconds, log4cxx::LoggerPtr loggerPtr)
{
	Create();
	_physicalPin = physicalPin;
	_systemCallOnEvent = systemCallOnEvent;
	_triggerState = triggerOnHigh;
	_sleepTimeInLoopInSeconds = sleepTimeInLoopInSeconds;
	_minimumDelayBetweenTriggersInSeconds = minimumDelayBetweenTriggersInSeconds;
	_delayToLogInactivityInSeconds = delayToLogInactivityInSeconds;
	_loggerPtr = loggerPtr;
}

void RPiGpioTrigger::StartWatching()
{
	if (_watchThread == 0)
		pthread_create(&_watchThread, NULL, GpioWatchThread, this);
	else
	{
		stringstream msg;
		msg << "RPiGpioTrigger::StartWatching(): Thread is already started";
		LOG4CXX_ERROR(_loggerPtr, msg.str());
		throw runtime_error(msg.str().c_str());
	}
}

void RPiGpioTrigger::StopWatching()
{
}


void* GpioWatchThread(void* castRPiGpioTriggerPtr)
{
	RPiGpioTrigger* triggerPtr = (RPiGpioTrigger*) castRPiGpioTriggerPtr;
	while(true)
	{
		usleep(1000000 * triggerPtr->SleepTimeInLoopInSeconds());
	}
	return NULL;
}
