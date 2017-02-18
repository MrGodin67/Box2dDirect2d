#pragma once
#include <d2d1.h>
#include "Vec2.h"
#include "Rect.h"
class Drawable
{
protected:
	D2D1::Matrix3x2F matRot;
	D2D1::Matrix3x2F matScale;
	D2D1::Matrix3x2F matTrans;
	
public:
	virtual ~Drawable() {};
	virtual void SetScale(const Vec2f& scale) {};
	virtual void Rasterize(class Graphics& gfx) = 0;
	virtual void Transform(const D2D1::Matrix3x2F& mat) = 0;
	virtual RectF GetAABB() { return RectF(); }
};