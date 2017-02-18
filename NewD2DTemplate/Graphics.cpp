#include "Graphics.h"
#include <assert.h>
#include "Locator.h"

Graphics::Graphics(int screenWidth,int screenHeight,HWND & hwnd,
	bool v_sync,bool fullscreen, float scrnDepth, float scrnNear)
	:
	Direct3D((UINT)screenWidth,(UINT)screenHeight,hwnd,v_sync,fullscreen,scrnDepth,scrnNear)
{
	HRESULT hr;
	hr = CreateTextObjects();
	assert(hr == S_OK);
	Locator::SetD2DRenderTarget(m_pD2DRenderTarget);
	Locator::SetTextManager(&m_TextFactory);
}

HRESULT Graphics::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4] = { red,green,blue,alpha };
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);
	m_pImmediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	HRESULT hr = InvokeD2DDeviceResources();

	if (SUCCEEDED(hr))
	{
		m_pD2DRenderTarget->BeginDraw();
		m_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}
	
	return hr;
}

HRESULT Graphics::EndScene()
{

	HRESULT hr = S_OK;
	hr = m_pD2DRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
     	ReleaseD2DDeviceResources();
	}
	if (m_vsync_enabled)
	{
		hr = m_pSwapChain->Present(1,0);// capped frame rate
	}
	else
	{
		hr = m_pSwapChain->Present(0, 0);// full on flip'n 
	}
	return hr;
}
void Graphics::DrawTriangle(D2D1_MATRIX_3X2_F &matTrans, D2D1_POINT_2F points[], D2D1_COLOR_F& color, float line_width)
{
	for (int i = 0; i < 3; i++)
	{
		D2D1_POINT_2F start = points[i];
		D2D1_POINT_2F end;
		if (i + 1 < 3)
			end = points[i + 1];
		else if (i + 1 >= 3)
			end = points[0];

		DrawLine(matTrans, start, end, color, line_width);
	}
}
void Graphics::DrawLine(D2D1_MATRIX_3X2_F &matTrans, D2D1_POINT_2F start, D2D1_POINT_2F end, D2D1_COLOR_F& color, float lineWidth)
{
	
	m_pD2DWhiteBrush->SetColor({ color.r,color.g,color.b,color.a });
	m_pD2DRenderTarget->SetTransform(matTrans);
	m_pD2DRenderTarget->DrawLine(
		D2D1::Point2F(start.x, start.y),
		D2D1::Point2F(end.x, end.y),
		m_pD2DWhiteBrush,
		lineWidth
	);
	m_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
};
//==============================================================================
void Graphics::DrawFilledScreenRectangle(D2D1_RECT_F& rect, D2D1_COLOR_F& color)
{
	m_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	
	m_pD2DWhiteBrush->SetColor({ color.r,color.g,color.b,color.a });
	m_pD2DRenderTarget->FillRectangle(&rect, m_pD2DWhiteBrush);
}
void Graphics::DrawScreenRectangle(D2D1_RECT_F rect, D2D1_COLOR_F& color)
{
	m_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	m_pD2DWhiteBrush->SetColor(color);
	m_pD2DRenderTarget->DrawRectangle(&rect, m_pD2DWhiteBrush );

}
void Graphics::DrawRectangle(D2D1_MATRIX_3X2_F &matTrans, D2D1_RECT_F& rect,D2D1_COLOR_F& color)
{
	m_pD2DRenderTarget->SetTransform(matTrans);
	
	m_pD2DWhiteBrush->SetColor(color);
	m_pD2DRenderTarget->DrawRectangle(&rect, m_pD2DWhiteBrush);
	//m_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}


void Graphics::DrawSprite(D2D1_MATRIX_3X2_F &trans, D2D1_RECT_F &PosSize, ID2D1Bitmap *pSprite,
	D2D1_RECT_F *ClipRect, float Opacity,D2D1_BITMAP_INTERPOLATION_MODE InterpMode)
{
	m_pD2DRenderTarget->SetTransform(trans);
	m_pD2DRenderTarget->DrawBitmap(pSprite, PosSize, Opacity, InterpMode, ClipRect);

	//reset transform so we have no carry-over to next sprite
	m_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void Graphics::RenderText(LPWSTR String, IDWriteTextFormat *pFormat,
	const D2D1_RECT_F &DrawRect, D2D1_COLOR_F &Color)
{
	
	m_pD2DWhiteBrush->SetColor(Color);

	UINT strLen = lstrlen(String);
	m_pD2DRenderTarget->DrawTextW(String, strLen, pFormat, DrawRect, m_pD2DWhiteBrush);
}

void Graphics::Rasterize(Drawable & obj)
{
	obj.Rasterize(*this);
}


HRESULT Graphics::CreateTextObjects()
{
	bool result;
	result = m_TextFactory.InitializeFont("Tahoma32", { L"Tahoma", 32.0f });
	if (!result) { return E_FAIL; }
	result = m_TextFactory.InitializeFont("Tahoma22", { L"Tahoma", 22.0f });
	if (!result) { return E_FAIL; }
	result = m_TextFactory.InitializeFont("Tahoma20", { L"Tahoma", 20.0f });
	if (!result) { return E_FAIL; }
	result = m_TextFactory.InitializeFont("Tahoma18", { L"Tahoma", 18.0f });
	if (!result) { return E_FAIL; }
	result = m_TextFactory.InitializeFont("Tahoma16", { L"Tahoma", 16.0f });
	if (!result) { return E_FAIL; }
	result = m_TextFactory.InitializeFont("Tahoma14", { L"Tahoma", 14.0f });
	if (!result) { return E_FAIL; }
	result = m_TextFactory.InitializeFont("Tahoma12", { L"Tahoma", 12.0f });
	if (!result) { return E_FAIL; }
	result = m_TextFactory.InitializeFont("Tahoma10", { L"Tahoma", 10.0f });
	if (!result) { return E_FAIL; }
	return S_OK;
}
