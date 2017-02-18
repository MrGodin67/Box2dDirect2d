#pragma once

#include "Animation.h"

#include "Locator.h"

class AnimationSequence : public Animation
{
protected:
	std::vector<int> m_seqIndices;
	float m_interval = 0.0f;
	float m_timer = 0.0f;
	int m_seqIndex = 0;
	std::string m_imageName;
	bool m_looping = false;
	bool m_done = false;
public:
	AnimationSequence() {}
	AnimationSequence(Animation::RenderDesc& desc,std::vector<int>& seq_indices,float& interval,
		std::string imageName,bool loop = false)
	   :
	Animation(desc),
	m_seqIndices(seq_indices),
	m_interval(interval),
	m_imageName(imageName),
	m_looping(loop)
	{}
	virtual ~AnimationSequence() {}
	virtual void Draw(class Camera& cam);
	virtual void Update(const float& dt);
	virtual bool Done();
	virtual bool Looping();
};