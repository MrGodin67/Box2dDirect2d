#include "Box2dSprite.h"
#include "Graphics.h"
Box2dSprite::Box2dSprite(ID2D1Bitmap* image, D2D1_SIZE_F& dim, D2D1_RECT_F& clipRect, float transparency)
	:m_image(image),m_dimensions(dim),m_clipRect(clipRect),m_transparency(transparency)
{
}



///////////////////////////////////////////////////
void Box2dSprite::Drawable::Rasterize(Graphics& gfx)
{
	matRot = D2D1::Matrix3x2F::Rotation(Utils::RadToDeg(m_parent.m_body->GetAngle()), 
	{ m_parent.m_body->GetPosition().x,m_parent.m_body->GetPosition().y });
	
	D2D1_RECT_F rect = D2D1::RectF(
		m_parent.m_body->GetPosition().x - m_parent.m_dimensions.width / 2.0f,
		m_parent.m_body->GetPosition().y - m_parent.m_dimensions.height / 2.0f,
		m_parent.m_body->GetPosition().x + m_parent.m_dimensions.width / 2.0f,
		m_parent.m_body->GetPosition().y + m_parent.m_dimensions.height / 2.0f);
	gfx.DrawSpriteBox2D(matRot*matTrans,Box2dSprite::RenderDesc(m_parent.m_image,rect,m_parent.m_clipRect,m_parent.m_transparency));

}
void Box2dSprite::Drawable::Transform(const D2D1::Matrix3x2F& mat)
{
	matTrans =  mat;
}

RectF Box2dSprite::Drawable::GetAABB()
{
	return RectF(
		m_parent.m_body->GetPosition().x - m_parent.m_dimensions.width * 0.5f,
		m_parent.m_body->GetPosition().y - m_parent.m_dimensions.height * 0.5f,
		m_parent.m_body->GetPosition().x + m_parent.m_dimensions.width * 0.5f,
		m_parent.m_body->GetPosition().y + m_parent.m_dimensions.height * 0.5f
	);
}
