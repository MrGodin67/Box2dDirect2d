#pragma once
#include "Direct3D_2D.h"
#include "DirectWrite.h"
#include "Rect.h"
#include "Animation.h"
#include "RenderTarget.h"
#include "Box2dSprite.h"
#include "Player.h"
class Graphics : public Direct3D,public RenderTarget
{
private:
	TextHandler m_TextFactory;
	
public:
	Graphics(int screenWidth, int screenHeight, HWND & hwnd,
		bool v_sync, bool fullscreen, float scrnDepth, float scrnNear);

	HRESULT BeginScene(float red, float green, float blue, float alpha);
	HRESULT EndScene();
	void DrawTriangle(D2D1_MATRIX_3X2_F &matTrans, D2D1_POINT_2F points[], D2D1_COLOR_F& color, float line_width);
	void DrawLine(D2D1_MATRIX_3X2_F &matTrans, D2D1_POINT_2F start, D2D1_POINT_2F end, D2D1_COLOR_F& color, float width);
	void DrawRectangle(D2D1_MATRIX_3X2_F &matTrans, D2D1_RECT_F& rect, D2D1_COLOR_F& color);
	void DrawScreenRectangle(D2D1_RECT_F rect, D2D1_COLOR_F& color);
	void DrawFilledScreenRectangle(D2D1_RECT_F& rect, D2D1_COLOR_F& color);
	void DrawSprite(D2D1_MATRIX_3X2_F &trans, D2D1_RECT_F &PosSize, ID2D1Bitmap *pSprite,
		D2D1_RECT_F *ClipRect = nullptr,float Opacity = 1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE InterpMode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
	void DrawSprite(D2D1_MATRIX_3X2_F &trans, Animation::RenderDesc& desc)
	{
		DrawSprite(trans, desc.drawRect, desc.image, &desc.clipRect, desc.opague, desc.interpMode);
	}
	void DrawPlayer(D2D1_MATRIX_3X2_F &trans, Player::RenderDesc& desc)
	{
		DrawSprite(trans, desc.drawRect, desc.image, &desc.clipRect, desc.opague, desc.interpMode);
	}
	void DrawSpriteBox2D(D2D1_MATRIX_3X2_F &trans, Box2dSprite::RenderDesc& desc)
	{
		DrawSprite(trans, desc.drawRect, desc.image, &desc.clipRect, desc.opague, desc.interpMode);
	}
	void RenderText(LPWSTR String, IDWriteTextFormat *pFormat,
		const D2D1_RECT_F &DrawRect, D2D1_COLOR_F &Color);
	virtual void Rasterize( Drawable& obj)override;
private:
	HRESULT CreateTextObjects();
};