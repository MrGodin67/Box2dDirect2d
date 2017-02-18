#include "Box.h"
#include "Graphics.h"

DynamicBox::DynamicBox(b2World* world, Vec2f pos,ID2D1Bitmap* image, D2D1_SIZE_F& dim, D2D1_RECT_F& clipRect, float transparency)
	:Box2dSprite(image,dim,clipRect,transparency)
{
	b2BodyDef bDef;
	bDef.type = b2_dynamicBody;
	bDef.position.Set(pos.x, pos.y);
	bDef.awake = true;
	bDef.userData = this;
	m_body = world->CreateBody(&bDef);
	b2PolygonShape shape;
	shape.SetAsBox(dim.width / 2, dim.height / 2);
	b2FixtureDef fix;
	fix.shape = &shape;
	fix.density = 1.0f;
	fix.friction = 0.53f;// dim.width * 0.02f;
	fix.restitution = 0.1f;
	m_fixture = m_body->CreateFixture(&fix);
}

Box2dSprite::Drawable DynamicBox::GetDrawable()
{
	return Drawable(*this);
}
b2Body * DynamicBox::GetBody()
{
	return m_body;
}

b2Fixture * DynamicBox::GetFixture()
{
	return m_fixture;
}
////////////////////////////////////////////
StaticBox::StaticBox(b2World* world, Vec2f pos, ID2D1Bitmap* image, D2D1_SIZE_F& dim, D2D1_RECT_F& clipRect, float transparency)
	:Box2dSprite(image, dim, clipRect, transparency)
{
	b2BodyDef groundDef;
	groundDef.position.Set(pos.x, pos.y);
	
	m_body = world->CreateBody(&groundDef);
	b2PolygonShape gBox;
	gBox.SetAsBox(dim.width / 2, dim.height / 2);
	b2FixtureDef fix;
	fix.shape = &gBox;
	fix.density = 1.0f;
	fix.friction = 2.03f;// dim.width * 0.02f;
	m_body->CreateFixture(&fix);
}

Box2dSprite::Drawable StaticBox::GetDrawable()
{
	return Drawable(*this);
}
b2Body * StaticBox::GetBody()
{
	return m_body;
}

b2Fixture * StaticBox::GetFixture()
{
	return m_fixture;
}

