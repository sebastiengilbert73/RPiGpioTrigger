#include "RPiGpioTrigger.h"

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
