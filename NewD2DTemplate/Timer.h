
#pragma once
#include <chrono>

class Timer
{
	float deltaTime;
	int fps;
	std::chrono::time_point<std::chrono::steady_clock> tp;
public:
	Timer() {}
	void Frame() 
	{
		const auto now = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration<float>(now - tp).count();
		tp = now;
		fps = (int)(1.0f / deltaTime);
	};
	int Fps()
	{
		return fps;
	}
	float DeltatTime()
	{
		return deltaTime;
	}
};