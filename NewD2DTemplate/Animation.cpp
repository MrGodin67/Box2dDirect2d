#include "Animation.h"
#include "Graphics.h"


Animation::Animation(RenderDesc& desc)
	:
	m_renderDesc(desc)
{
	m_drawWidth = desc.drawRect.right - desc.drawRect.left;
	m_drawHeight = desc.drawRect.bottom - desc.drawRect.top;
	m_center.x = desc.drawRect.left + (m_drawWidth * 0.5f);
	m_center.y = desc.drawRect.top + (m_drawHeight * 0.5f);
	
}


void Animation::SetScaling(Vec2f & scale)
{
	m_scale = scale;
}

void Animation::SetRotation(float & angle)
{
	m_rotAngle = angle;
}

void Animation::SetPosition(Vec2f & pos)
{
	m_renderDesc.drawRect = RectF( pos.x,pos.y,pos.x + m_drawWidth,pos.y+m_drawHeight).ToD2D();
	m_center.x = pos.x + (m_drawWidth * 0.5f);
	m_center.y = pos.y + (m_drawHeight * 0.5f);
}

bool Animation::PointIn(Vec2f & pt)
{
	
	return RectF(m_renderDesc.drawRect).Contains(pt);
}







Animation::Drawable::Drawable(Animation& p)
	:
	m_parent(p)

{
	matTrans = D2D1::Matrix3x2F::Identity();
	matRot =  D2D1::Matrix3x2F::Identity();
	matScale = D2D1::Matrix3x2F::Identity();
	
	
}

Animation::Drawable Animation::GetDrawable()
{
	return Drawable(*this);
}

void Animation::Drawable::Rasterize(Graphics& gfx)
{
		Transform(D2D1::Matrix3x2F::Rotation(m_parent.m_renderDesc.angle, { m_parent.m_center.x,m_parent.m_center.y }));
		Transform(D2D1::Matrix3x2F::Scale(
		{ m_parent.m_scale.x, m_parent.m_scale.y },
		{ m_parent.m_center.x,m_parent.m_center.y }));
		gfx.DrawSprite(matTrans, m_parent.m_renderDesc);
	
}
void Animation::Drawable::Transform(const D2D1::Matrix3x2F& mat)
{
	matTrans = mat * matTrans;
}




