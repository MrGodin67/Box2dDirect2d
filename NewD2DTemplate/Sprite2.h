#pragma once
#include "VecRect.h"
#include "AnimationSequence.h"

enum _EntityType
{
	drone,
	fighter,
	artillary,
	radar,
	turret,
	tech,
	data,
	none
};
class TextSprite
{
protected:
	D2D1_COLOR_F color;
	D2D1_COLOR_F backcolor;
	RectF pos;
	WCHAR text[512];
	bool drawBackground;
	IDWriteTextFormat* pFormat;
public:
	TextSprite() {}
	TextSprite(IDWriteTextFormat* format, RectF pos, std::wstring text, D2D1_COLOR_F color, D2D1_COLOR_F backColor = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), bool drawBack = false)
		:pFormat(format), pos(pos), color(color), backcolor(backColor), drawBackground(drawBack)
	{
		wsprintf(this->text, L"%s", text.c_str());
	
	}
	virtual ~TextSprite() {}
	virtual void UpdateText(std::wstring str);
	virtual void Draw(class Graphics& gfx);

};


class Sprite : public AnimationSequence
{
protected:
	Extents<float> m_extents;
	Vec2f m_position;
	std::vector<float> m_rotAngles;
	
public:
	Sprite() {}
	Sprite(Animation::RenderDesc& desc,std::vector<int> indices,float interval,
		std::string imageName)
		:AnimationSequence(desc,indices,interval,imageName),
		m_extents((desc.drawRect.right - desc.drawRect.left), (desc.drawRect.bottom - desc.drawRect.top))
	{
		m_rotAngles.resize(indices.size());
		for (size_t c = 0; c < m_rotAngles.size(); c++)
			m_rotAngles[c] = 0.0f;
		m_position = Vec2f(desc.drawRect.left + ((desc.drawRect.right - desc.drawRect.left) / 2.0f),
			desc.drawRect.top + ((desc.drawRect.bottom - desc.drawRect.top) / 2.0f));
		m_renderDesc.image = Locator::ImageManager->GetImage(m_imageName)->GetTexture();
	}
	
	virtual void Draw(class Camera& cam)override;
	virtual void Update(const float& dt)override;
	
	virtual void UpdatePosition(Vec2f velocity);
	virtual void SetPosition(Vec2f pos);
	virtual Vec2f GetPosition();
	virtual RectF GetRect();
	virtual void UpdateAngle(int index, float& angle);
};