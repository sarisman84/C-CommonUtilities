#pragma once
#include <chrono>
#include <functional>
#include <iostream>
#include "List.h"

using namespace std;
using namespace std::chrono;

#define TimePoint steady_clock::time_point
#define GetCount time_since_epoch().count()
#define CurrentTime high_resolution_clock::now()
#define UpdateEvent_Del function<void(Timer&)>
#define RegisteredEventPred(body) [&](TimeEvent&  anEvent) {body;}
#define UpdatePredicate(head,body) [head](Timer& aTimer) {body;}
//Returns the time it takes to go to the next frame in seconds.
#define deltaTime GetDeltaTime_Sec()
//Returns the time it takes to go to the next frame in nanoseconds.
#define rawDeltaTime GetDeltaTime()
//Returns the total time since this timer has started.
#define totalTime GetTotalTime()

namespace CommonUtilities
{
	class Timer
	{
	public:
		class TimeEvent
		{
		public:
			TimeEvent();
			TimeEvent(UpdateEvent_Del aPredicate);
			void Invoke(Timer& aTimer);
			const bool operator==(const TimeEvent aRhs);
		private:
			UpdateEvent_Del myUpdateEvent;

		};
	private:
		TimePoint myStartTime;
		TimePoint myLastTime; //Used to refresh update
		TimePoint myCurrentTime;
		float myDeltaTime;
		float myRefreshRate;
		bool myRuntimeFlag;
		const double mySecondInMillisecond = 1000.0f;
		const double mySecondInNanosecond = 1000000000.0f;
		List<TimeEvent> myUpdateEvent;

		void DoUpdateLoop();
		void CalculateDeltaTime();
	public:

		Timer();
		Timer(const Timer& aTimer) = delete;
		Timer& operator=(const Timer& aTimer) = delete;
		void Update();
		float FromNanoToSeconds(const float aValue) const;
		float GetDeltaTime_Sec() ;
		float GetDeltaTime() ;
		double GetTotalTime() const;
		//Stops the timer
		void StopTimer();
		//Starts the timer
		void StartTimer(const float aRefreshRate = 10.0f, const bool aCreateLoop = false);

		//Adds an event to an event buffer that gets called every frame.
		void AddListener(TimeEvent aListenerToAdd);

		//Removes an event from an event buffer.
		void RemoveListener(TimeEvent aListenerToRemove);

		//Removes all events registered in an event buffer.
		void RemoveAllListeners();
	};
}