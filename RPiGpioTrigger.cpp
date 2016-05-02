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

int WiringPinFromPhysicalPin(int physicalPin)
{
	switch (physicalPin)
	{
		case 3:
			return 8;
		case 5:
			return 9;
		case 7:
			return 7;
		case 8:
			return 15;
		case 10:
			return 16;
		case 11:
			return 0;
		case 12:
			return 1;
		case 13:
			return 2;
		case 15:
			return 3;
		case 16:
			return 4;
		case 18:
			return 5;
		case 19:
			return 12;
		case 21:
			return 13;
		case 22:
			return 6;
		case 23:
			return 14;
		case 24:
			return 10;
		case 26:
			return 11;
		default:
			stringstream msg;
			msg << "WiringPinFromPhysicalPin(): Unavailable physical pin " << physicalPin << ". Use 3, 5, 7, 8, 10, 11, 12, 13, 15, 16, 18, 19, 21, 22, 23, 24 or 26";
			throw runtime_error(msg.str().c_str());
	}
	return -1;
}


void* GpioWatchThread(void* castRPiGpioTriggerPtr)
{
	RPiGpioTrigger* triggerPtr = (RPiGpioTrigger*) castRPiGpioTriggerPtr;
	LOG4CXX_INFO(triggerPtr->LoggerPtr(), "GpioWatchThread(): Start watching physical pin " << triggerPtr->PhysicalPin());
	int wiringPin = WiringPinFromPhysicalPin(triggerPtr->PhysicalPin());
	pinMode(wiringPin, INPUT);
	pullUpDnControl(wiringPin, triggerPtr->TriggerOnHigh() ? PUD_DOWN : PUD_UP);
	
	time_t lastEvent = time(NULL);
	time_t lastLog = time(NULL);
	while(triggerPtr->WatchThreadMustRun())
	{
		time_t currentTime = time(NULL);
		double delaySinceLastEventInSeconds = difftime(currentTime, lastEvent);
		if (delaySinceLastEventInSeconds >= triggerPtr->MinimumDelayBetweenTriggersInSeconds())
		{
			if ( (triggerPtr->TriggerOnHigh() && digitalRead(wiringPin) == HIGH)
				|| (!triggerPtr->TriggerOnHigh() && digitalRead(wiringPin) == LOW) )
			{
				LOG4CXX_INFO(triggerPtr->LoggerPtr(), "GpioWatchThread(): Calling 'system(" << triggerPtr->SystemCallOnEvent().c_str() << ")'");
				system(triggerPtr->SystemCallOnEvent().c_str());
				lastEvent = currentTime;
				lastLog = currentTime;
			}
		}
		if (difftime(currentTime, lastLog) >= triggerPtr->DelayToLogInactivityInSeconds())
		{
			LOG4CXX_INFO(triggerPtr->LoggerPtr(), "GpioWatchThread(): Still running!");
			lastLog = currentTime;
		}
		usleep(1000000 * triggerPtr->SleepTimeInLoopInSeconds());
	}
	LOG4CXX_INFO(triggerPtr->LoggerPtr(), "GpioWatchThread(): Stop");
	return NULL;
}

