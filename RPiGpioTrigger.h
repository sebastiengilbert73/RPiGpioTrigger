#ifndef RPiGpioTrigger_h
#define RPiGpioTrigger_h

#include <iostream>
#include <string>
#include <sstream>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h> // usleep()
#include <log4cxx/logger.h>
#include <stdexcept> // runtime_error
#include <wiringPi.h>
#include <cstdlib>


class RPiGpioTrigger
{
public:
	RPiGpioTrigger();
	void Create();
	RPiGpioTrigger(int physicalPin, std::string systemCallOnEvent, bool triggerOnHigh, double sleepTimeInLoopInSeconds,
	   double minimumDelayBetweenTriggersInSeconds, double delayToLogInactivityInSeconds, log4cxx::LoggerPtr loggerPtr);
	void Create(int physicalPin, std::string systemCallOnEvent, bool triggerOnHigh, double sleepTimeInLoopInSeconds,
	   double minimumDelayBetweenTriggersInSeconds, double delayToLogInactivityInSeconds, log4cxx::LoggerPtr loggerPtr);
	void StartWatching();
	void StopWatching();
	int PhysicalPin() { return _physicalPin; }
	std::string SystemCallOnEvent() { return _systemCallOnEvent; }
	bool TriggerOnHigh() { return _triggerState; }
	double SleepTimeInLoopInSeconds() { return _sleepTimeInLoopInSeconds; }
	double MinimumDelayBetweenTriggersInSeconds() { return _minimumDelayBetweenTriggersInSeconds; }
	double DelayToLogInactivityInSeconds() { return _delayToLogInactivityInSeconds; }
	log4cxx::LoggerPtr LoggerPtr() {  return _loggerPtr; }
	bool WatchThreadMustRun() { return _watchThreadMustRun; }
	
private:
	int _physicalPin;
	std::string _systemCallOnEvent;
	bool _triggerState;
	double _sleepTimeInLoopInSeconds;
	double _minimumDelayBetweenTriggersInSeconds;
	double _delayToLogInactivityInSeconds;
	log4cxx::LoggerPtr _loggerPtr;
	pthread_t _watchThread;
	bool _watchThreadMustRun;

};

void* GpioWatchThread(void* castRPiGpioTriggerPtr);

#endif
