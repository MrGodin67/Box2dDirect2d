#pragma once

#include "Box2D\Box2D.h"
#include "VecRect.h"
#include "Utils.h"
#include "Box2dSprite.h"
class DynamicBox : public Box2dSprite
{
	
public:
	DynamicBox(b2World* world, Vec2f pos,ID2D1Bitmap* image, D2D1_SIZE_F& dim, D2D1_RECT_F& clipRect, float transparency);
	virtual Drawable GetDrawable()override;
	b2Body* GetBody();
	b2Fixture* GetFixture();
};

class StaticBox : public Box2dSprite
{

public:
	StaticBox(b2World* world, Vec2f pos, ID2D1Bitmap* image, D2D1_SIZE_F& dim, D2D1_RECT_F& clipRect, float transparency);
	virtual Drawable GetDrawable()override;
	b2Body* GetBody();
	b2Fixture* GetFixture();
};