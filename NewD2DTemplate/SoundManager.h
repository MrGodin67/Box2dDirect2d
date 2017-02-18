#pragma once
#include "SoundEffect.h"
#include "includes.h"
class SoundManager
{
	std::unordered_map<std::string, SoundEffect> m_soundFX;
	float pl = 0.0f;;
	float volume = 1.0f;
public:
	SoundManager() {}
	~SoundManager() {}
	void AddSound(std::string name, std::wstring file)
	{
		m_soundFX[name] = SoundEffect({ file.c_str() });
	}
	void Play(std::string sound,float in_volume)
	{
		m_soundFX[sound].Play(pl,in_volume);
	}
	void Play(std::string sound)
	{
		m_soundFX[sound].Play(pl,volume);
	}
	void SetVolume(float vol) { volume = vol; }
};