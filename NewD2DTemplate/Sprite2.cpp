#include "Sprite2.h"
#include "Camera.h"
#include "Graphics.h"
void Sprite::Draw(Camera & cam)
{
	for (size_t c = 0; c < m_seqIndices.size(); c++)
	{
		TextureManager::ImageClip iClip = Locator::ImageManager->GetClip(m_imageName, m_seqIndices[c]);
		m_renderDesc.clipRect = iClip.rect.ToD2D();
		m_renderDesc.image = iClip.bitmap;
		m_renderDesc.angle = m_rotAngles[c];
		cam.Rasterize(GetDrawable());
	}
	
}
void Sprite::Update(const float & dt)
{
	
	m_renderDesc.drawRect = m_extents.GetRect(m_position).ToD2D();
}

void Sprite::UpdatePosition(Vec2f velocity)
{
	m_position += velocity;
	m_renderDesc.drawRect = m_extents.GetRect(m_position).ToD2D();
	m_center = Vec2f(m_renderDesc.drawRect.left + m_extents.GetMax().x, m_renderDesc.drawRect.top + m_extents.GetMax().y);
}

void Sprite::SetPosition(Vec2f pos)
{
	m_position = pos;
	m_renderDesc.drawRect = RectF(m_extents.GetRect(pos)).ToD2D();
	m_center = Vec2f(m_renderDesc.drawRect.left + m_extents.GetMax().x, m_renderDesc.drawRect.top + m_extents.GetMax().y);
}

Vec2f Sprite::GetPosition()
{
	return m_position;
}

RectF Sprite::GetRect()
{
	return m_extents.GetRect(m_position);
}

void Sprite::UpdateAngle(int index, float & angle)
{
	m_rotAngles[index] = angle;
}




void TextSprite::UpdateText(std::wstring str)
{
	
	wsprintf(this->text, L"%s", str.c_str());
}

void TextSprite::Draw(Graphics & gfx)
{
	if (drawBackground)
	{
		gfx.DrawFilledScreenRectangle(pos.ToD2D(), backcolor);
		gfx.DrawRectangle(D2D1::Matrix3x2F::Identity(), pos.ToD2D(), color);

	}
	gfx.RenderText(text, pFormat, pos.ToD2D(), color);
}