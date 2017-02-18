#pragma once
#include "includes.h"
class D2D1Texture
{
	UINT width, height;
	ID2D1Bitmap *pBmp = nullptr;
public:
	D2D1Texture();
	D2D1Texture(ID2D1RenderTarget* rt,const std::wstring &Basename);
	~D2D1Texture();
	
	HRESULT Load(ID2D1RenderTarget* rt,const std::wstring &Basename);
	UINT Width() const;
	UINT Height() const;
	
	ID2D1Bitmap* GetBitmap()const { return pBmp; }
	
	
};

