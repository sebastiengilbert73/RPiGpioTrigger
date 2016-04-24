#ifndef RPiGpioTrigger_h
#define RPiGpioTrigger_h

#include <iostream>
#include <string>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h> // usleep()
#include <log4cxx/logger.h>

class RPiGpioTrigger
{
public:
	RPiGpioTrigger();
	void Create();
	RPiGpioTrigger(int physicalPin, std::string systemCallOnEvent, bool triggerOnHigh, double sleepTimeInLoopInSeconds,
	   double minimumDelayBetweenTriggersInSeconds, double delayToLogInactivityInSeconds, log4cxx::LoggerPtr loggerPtr);
	void Create(int physicalPin, std::string systemCallOnEvent, bool triggerOnHigh, double sleepTimeInLoopInSeconds,
	   double minimumDelayBetweenTriggersInSeconds, double delayToLogInactivityInSeconds, log4cxx::LoggerPtr loggerPtr);
	
private:
	int _physicalPin;
	std::string _systemCallOnEvent;
	bool _triggerState;
	double _sleepTimeInLoopInSeconds;
	double _minimumDelayBetweenTriggersInSeconds;
	double _delayToLogInactivityInSeconds;
	log4cxx::LoggerPtr _loggerPtr;
};

#endif
