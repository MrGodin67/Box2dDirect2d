#pragma once
#include <random>
#include <windows.h>

template<class G>
class RandomGenerator
{
	std::random_device rd;
	G rng;
public:
	RandomGenerator()
	{
		rng = G(rd());
	}

	template<typename T>
	T Get(T min, T max) {}
	template<>
	int Get<int>(int min, int max)
	{
		std::uniform_int_distribution<int> dist(min, max);
		return dist(rng);
	}
	template<>
	double Get<double>(double min, double max)
	{
		std::uniform_real_distribution<double> dist(min, max);
		return dist(rng);
	}
	template<>
	float Get<float>(float min, float max)
	{
		std::uniform_real_distribution<float> dist(min, max);
		return dist(rng);
	}


};