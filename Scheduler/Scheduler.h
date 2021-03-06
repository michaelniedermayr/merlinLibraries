#ifndef Scheduler_h
#define Scheduler_h


#include <stdint-gcc.h>
#include <Timer.h>
#include <Schedulable.h>
#include <Loggable.h>

/**
 * Scheduler provides the heartbeat for the Merlin framework. Its update-function is continuously called by the main
 * while-loop. Every cyclePeriod seconds it calls all update-functions of the tasks that were added (addTask()).
 * Thereby, the tasks are update regularly:
 */
class Scheduler: public Loggable {
///////////////////////////////////////////////////////////////////////////
// Loggable
///////////////////////////////////////////////////////////////////////////
public:
	bool printData(Logger *logger, bool printHeader) override {
		if(loggingActive) {
			logger->printLoggable("counter", loopCounter, printHeader);
			logger->printLoggable("sysTime", cycleTimer.getSystemTime(), printHeader);
			logger->printLoggable("dT", deltaTime, printHeader);
			return true;
		}
		return false;
	}

///////////////////////////////////////////////////////////////////////////
// Custom
///////////////////////////////////////////////////////////////////////////
public:

    /**
     * @param hardwareTimer
     * @param cyclePeriod defines the heartbeat period in s of the Merlin framework
     */
	Scheduler(Hardware_timer *hardwareTimer, float cyclePeriod) : cycleTimer(Timer(hardwareTimer)), taskTimer(Timer(hardwareTimer)) {
		Scheduler::cyclePeriod = cyclePeriod;
		cycleTimer.startClock();
	}

    /**
     * Schedulable task are added
     * @param schedulable
     */
    void addTask(Schedulable *schedulable);

	void update();

	float getSystemTime(){ return (cycleTimer.getSystemTime()); }
	uint32_t getLoopCounter(){ return loopCounter; };
	float getdeltaTime(){ return deltaTime; };

protected:

	static const uint8_t SCHEDULABLES_MAX = 20;

    void runTask(uint16_t i);

	bool firstRun = true;

	Timer cycleTimer;
	Timer taskTimer;
	float cyclePeriod; // in s

    Schedulable *schedulables[SCHEDULABLES_MAX] = {}; // array of elements that are executed when update is called
    float taskDeltaTimes[SCHEDULABLES_MAX] = {}; // holds the delta time for each task
    float taskExecutionTimes[SCHEDULABLES_MAX] = {}; // array storing the execution time of each task

    uint16_t numTasks = 0;
	uint32_t loopCounter = 0;
	float deltaTime = 0.f;
	float deltaTimeOffset = 0.f;

};

#endif //Scheduler_h


