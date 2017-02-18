#pragma once
#include "Drawable.h"
#include "Box2D\Box2D.h"
#include "Utils.h"


class alignas(16) Box2dSprite
{

public:
	struct RenderDesc
	{
		ID2D1Bitmap* image;
		D2D1_RECT_F drawRect;
		D2D1_BITMAP_INTERPOLATION_MODE interpMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
		float opague = 1.0f;
		D2D1_RECT_F clipRect;
		RenderDesc() {}
		RenderDesc(ID2D1Bitmap* bmp, D2D1_RECT_F& drawrect, D2D1_RECT_F& cliprect, float transparency)
			:
			image(bmp),drawRect(drawrect),clipRect(cliprect),opague(transparency)
		{}
	};
	class Drawable : public ::Drawable
	{
	private:
		Box2dSprite& m_parent;
	public:
		Drawable(Box2dSprite& owner)
			:m_parent(owner)
		{}
		virtual void Rasterize(class Graphics& gfx)override;
		virtual void Transform(const D2D1::Matrix3x2F& mat)override;
		virtual RectF GetAABB()override;
	};
public:
	Box2dSprite(ID2D1Bitmap* image,D2D1_SIZE_F& dim,D2D1_RECT_F& clipRect,float transparency);
	virtual Drawable GetDrawable() = 0;
	
protected:
	D2D1_SIZE_F m_dimensions;
	D2D1_RECT_F m_clipRect;
	float m_transparency;

	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	ID2D1Bitmap* m_image = nullptr;
	
};