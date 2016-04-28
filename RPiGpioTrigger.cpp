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
	_watchThreadMustRun = false;

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
	{
		_watchThreadMustRun = true;
		pthread_create(&_watchThread, NULL, GpioWatchThread, this);
	}
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
	_watchThreadMustRun = false;
}


void* GpioWatchThread(void* castRPiGpioTriggerPtr)
{
	RPiGpioTrigger* triggerPtr = (RPiGpioTrigger*) castRPiGpioTriggerPtr;
	LOG4CXX_INFO(triggerPtr->LoggerPtr(), "GpioWatchThread(): Start");
	wiringPiSetupPhys();
	pinMode(triggerPtr->PhysicalPin(), INPUT);
	pullUpDnControl(triggerPtr->PhysicalPin(), triggerPtr->TriggerOnHigh() ? PUD_DOWN : PUD_UP);
	while(triggerPtr->WatchThreadMustRun())
	{
		if ( (triggerPtr->TriggerOnHigh() && digitalRead(triggerPtr->PhysicalPin()) == HIGH)
			|| (!triggerPtr->TriggerOnHigh() && digitalRead(triggerPtr->PhysicalPin()) == LOW) )
		{
			LOG4CXX_INFO(triggerPtr->LoggerPtr(), "GpioWatchThread(): Calling 'system(" << triggerPtr->SystemCallOnEvent().c_str() << ")'");
			system(triggerPtr->SystemCallOnEvent().c_str());
		}
		usleep(1000000 * triggerPtr->SleepTimeInLoopInSeconds());
	}
	LOG4CXX_INFO(triggerPtr->LoggerPtr(), "GpioWatchThread(): Stop");
	return NULL;
}
