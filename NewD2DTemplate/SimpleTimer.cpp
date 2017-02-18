#include "SimpleTimer.h"

SimpleTimer::SimpleTimer()
{
	INT64 frequency;


	// Get the cycles per second speed for this system.
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	// Store it in floating point.
	m_frequency = (float)frequency;

	// Get the initial start time.
	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
}


SimpleTimer::SimpleTimer(const SimpleTimer& other)
{
}


SimpleTimer::~SimpleTimer()
{
}


void SimpleTimer::Reset()
{
	INT64 frequency;


	// Get the cycles per second speed for this system.
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	// Store it in floating point.
	m_frequency = (float)frequency;

	// Get the initial start time.
	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);


}


void SimpleTimer::Update()
{
	INT64 currentTime;
	INT64 elapsedTicks;


	// Query the current time.
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	// Calculate the difference in time since the last time we queried for the current time.
	elapsedTicks = currentTime - m_startTime;

	// Calculate the frame time.
	m_frameTime = (float)elapsedTicks / m_frequency;

	// Restart the timer.
	m_startTime = currentTime;

	return;
}


float SimpleTimer::Time()
{
	return m_frameTime;
}


void SimpleTimer::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_beginTime);
	return;
}


void SimpleTimer::Stop()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_endTime);
	return;
}


int SimpleTimer::NumbMilliSeconds()
{
	float elapsedTicks;
	INT64 frequency;
	float milliseconds;


	// Get the elapsed ticks between the two times.
	elapsedTicks = (float)(m_endTime - m_beginTime);

	// Get the ticks per second speed of the timer.
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	// Calculate the elapsed time in milliseconds.
	milliseconds = (elapsedTicks / (float)frequency) * 1000.0f;

	return (int)milliseconds;
}
