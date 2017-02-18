#pragma once


#include "includes.h"
#include "Vec2.h"
#include "Rect.h"
#include "RenderTarget.h"
class Camera : public RenderTarget
{
	RenderTarget * m_nextRenderTarget = nullptr;
public:// next = vp
	Camera(RenderTarget * next,float width, float height);
	virtual void Rasterize(class Drawable& obj)override;
	Vec2f GetPos();
	void ConfineToMap(RectF& map_frame);
	void Resize(float& w, float& h);
	void UpdatePosition(Vec2f& in_pos);
	Vec2f ConvertToWorldSpace(Vec2f in_pos);
	bool PointInViewFrame(Vec2f pt, const Vec2f& offset);
	RectF GetViewFrame() { return viewFrame; }
	void Scroll(Vec2f& dir);
	
private:
	
	Vec2f pos;
	Vec2f center;
	Vec2f scroll_pos;
	RectF mapFrame;
	RectF viewFrame;
	float screen_width, screen_height;
	
};
