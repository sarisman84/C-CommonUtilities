#include "Timer.h"
#include "pch.h"
#include <Windows.h>
void CommonUtilities::Timer::DoUpdateLoop()
{

	while (myRuntimeFlag)
	{
		float sleepDur = (1.0f / myRefreshRate) * mySecondInMillisecond;
		Sleep(sleepDur);
		Update();


	}
}

void CommonUtilities::Timer::CalculateDeltaTime()
{
	myCurrentTime = CurrentTime;
	myDeltaTime = (myCurrentTime.GetCount - myLastTime.GetCount);
	myLastTime = CurrentTime;
}

CommonUtilities::Timer::Timer()
{
	myRefreshRate = 10;
	myRuntimeFlag = false;
}

void CommonUtilities::Timer::Update()
{
	myUpdateEvent.ForEach(RegisteredEventPred(anEvent.Invoke(*this)));
}



float CommonUtilities::Timer::FromNanoToSeconds(const float aValue) const
{
	return aValue / mySecondInNanosecond;
}




float CommonUtilities::Timer::GetDeltaTime_Sec()
{
	return FromNanoToSeconds(GetDeltaTime());
}



float CommonUtilities::Timer::GetDeltaTime()
{
	CalculateDeltaTime();

	return myDeltaTime;
}

double CommonUtilities::Timer::GetTotalTime() const
{
	return FromNanoToSeconds(CurrentTime.GetCount - myStartTime.GetCount);
}

void CommonUtilities::Timer::StopTimer()
{
	myRuntimeFlag = false;
}

void CommonUtilities::Timer::StartTimer(const float aRefreshRate, const bool aCreateLoop)
{
	myRuntimeFlag = true;
	myRefreshRate = aRefreshRate;
	myStartTime = CurrentTime;
	myLastTime = myStartTime;
	if (aCreateLoop)
		DoUpdateLoop();
}

void CommonUtilities::Timer::AddListener(TimeEvent aListenerToAdd)
{
	myUpdateEvent.Add(aListenerToAdd);
}

void CommonUtilities::Timer::RemoveListener(TimeEvent aListenerToRemove)
{
	myUpdateEvent.Remove(aListenerToRemove);
}

void CommonUtilities::Timer::RemoveAllListeners()
{
	myUpdateEvent.Clear();
}

CommonUtilities::Timer::TimeEvent::TimeEvent()
{
}

CommonUtilities::Timer::TimeEvent::TimeEvent(UpdateEvent_Del aPredicate)
{
	myUpdateEvent = aPredicate;
}

void CommonUtilities::Timer::TimeEvent::Invoke(Timer& aTimer)
{
	if (myUpdateEvent != nullptr)
		myUpdateEvent(aTimer);
}

const bool CommonUtilities::Timer::TimeEvent::operator==(const TimeEvent aRhs)
{
	return myUpdateEvent.target_type() == aRhs.myUpdateEvent.target_type();
}