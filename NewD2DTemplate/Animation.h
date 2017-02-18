#pragma once

#include "Drawable.h"
#include "includes.h"
#include "Rect.h"
#include "SpriteSheet.h"

class Animation
{
	
public:
	struct RenderDesc
	{
		ID2D1Bitmap* image;
		D2D1_RECT_F drawRect;
		D2D1_BITMAP_INTERPOLATION_MODE interpMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
		float opague = 1.0f;
		D2D1_RECT_F clipRect;
		float angle = 0.0f;
		RenderDesc() {}
		RenderDesc(ID2D1Bitmap* bmp, D2D1_RECT_F& drawrect, D2D1_RECT_F& cliprect, float transparency)
			:
			image(bmp),
			drawRect(drawrect),
			clipRect(cliprect),
			opague(transparency)
		{}
	};
protected:
	RenderDesc m_renderDesc;
	Vec2f m_center;
	Vec2f m_scale = { 1.0f,1.0f };
	float m_drawWidth, m_drawHeight; 
	float m_rotAngle = 0.0f;
	
public:
	Animation() {}
    Animation(RenderDesc& desc);
	virtual ~Animation() {}
	RenderDesc GetRenderDesc() { return m_renderDesc; }
	virtual void SetScaling(Vec2f& scale);
	virtual void SetRotation(float& angle);
	virtual void SetPosition(Vec2f& pos);
	virtual bool PointIn(Vec2f& pt);
public:
	class Drawable : public ::Drawable
	{
		Animation& m_parent;

	public:
		Drawable(Animation& p);
		virtual ~Drawable() {};
		virtual void Rasterize(class Graphics& gfx)override;
		virtual void Transform(const D2D1::Matrix3x2F& mat)override;
	};
	Drawable GetDrawable();


};



