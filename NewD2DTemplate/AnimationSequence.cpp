#include "AnimationSequence.h"
#include "Camera.h"



void AnimationSequence::Draw(Camera & cam)
{
	
	m_renderDesc.clipRect = Locator::ImageManager->GetImage(m_imageName)->GetClippedImage(m_seqIndex).ToD2D();
	cam.Rasterize(GetDrawable());
}

void AnimationSequence::Update(const float & dt)
{
	if (!m_done)
	{
		if ((m_timer += dt) > m_interval)
		{
			m_seqIndex++;
			m_timer = 0.0f;
			if (m_seqIndex >= m_seqIndices.size())
			{
				if (m_looping)
					m_seqIndex = 0;
				else
					m_done = true;
			}
		}
	}
}

bool AnimationSequence::Done()
{
	return m_done;
}

bool AnimationSequence::Looping()
{
	return m_looping;
}
