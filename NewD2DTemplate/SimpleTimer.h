#pragma once


#include <windows.h>
class alignas(16) SimpleTimer
{
public:
	SimpleTimer();
	SimpleTimer(const SimpleTimer&);

	~SimpleTimer();
	void Update();
	float Time();
	void Reset();
	void Start();
	void Stop();
	int NumbMilliSeconds();


private:
	float m_frequency = 0.0f;
	INT64 m_startTime = 0;
	float m_frameTime = 0.0f;
	INT64 m_beginTime = 0;
	INT64 m_endTime = 0;
};