#pragma once

#include "Box2dSprite.h"
#include "includes.h"
#include "Locator.h"
#include <array>
#include "keyboard.h"
class Player 
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
			image(bmp), drawRect(drawrect), clipRect(cliprect), opague(transparency)
		{}
	};
	class Drawable : public ::Drawable
	{
	private:
		Player& m_parent;
	public:
		Drawable(Player& owner)
			:m_parent(owner)
		{}
		virtual void Rasterize(class Graphics& gfx)override;
		virtual void Transform(const D2D1::Matrix3x2F& mat)override;
		virtual RectF GetAABB()override;
	};
protected:
	std::array<int, 4> m_leftIndices = { 0,1,2,3 };
	std::array<int, 4> m_rightIndices = { 4,5,6,7 };
	std::array<int, 4> m_leftJumpIndices = { 12,13,14,15 };
	std::array<int, 4> m_rightJumpIndices = {8, 9,10,11 };
	std::vector<std::array<int, 4>> m_indexes;
	int m_indicesIndex = 1;
	int m_incIndex = 2;
	float timer = 0.0f;
	float timerInc = 0.25f;
	bool isMoving = false;
	D2D1_SIZE_F m_dimensions;
	D2D1_RECT_F m_clipRect;
	float m_transparency = 1.0f;
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	ID2D1Bitmap* m_image = nullptr;
public:
	Player(b2World* world, Vec2f pos, D2D1_SIZE_F& dim);
	void Update(float dt,Keyboard& kbd);
	Drawable GetDrawable();
	b2Vec2 GetPosition();
	
};