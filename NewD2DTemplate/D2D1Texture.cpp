
#include "D2D1Texture.h"


D2D1Texture::D2D1Texture()
{
}

D2D1Texture::D2D1Texture(ID2D1RenderTarget* rt,const std::wstring & Basename)
{
	Load(rt,Basename);
}


D2D1Texture::~D2D1Texture()
{
	SAFE_RELEASE(pBmp);
}

HRESULT D2D1Texture::Load(ID2D1RenderTarget* rt,const std::wstring & Basename)
{
	IWICBitmapDecoder *pDecoder = nullptr;
	IWICBitmapFrameDecode *pFrame = nullptr;
	IWICFormatConverter *pConverter = nullptr;
	IWICBitmap *pBitmap = nullptr;
	IWICImagingFactory *pFactory = nullptr;
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pFactory));

	if (SUCCEEDED(hr))
	{
		hr = pFactory->CreateDecoderFromFilename(Basename.c_str(), nullptr, GENERIC_READ,
			WICDecodeMetadataCacheOnDemand, &pDecoder);
	}
	if (SUCCEEDED(hr))
	{
		pDecoder->GetFrame(0, &pFrame);
	}
	if (SUCCEEDED(hr))
	{

		hr = pFactory->CreateFormatConverter(&pConverter);
	}
	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPRGBA,
			WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);
	}
	if (SUCCEEDED(hr))
	{
		hr = pFactory->CreateBitmapFromSource(pConverter, WICBitmapCacheOnDemand, &pBitmap);
		if (pBitmap)
			pBitmap->GetSize(&width, &height);

		if(SUCCEEDED(hr))
			hr = rt->CreateBitmapFromWicBitmap(pBitmap, &pBmp);
	}
	assert(hr == S_OK);

	SAFE_RELEASE(pFactory);
	SAFE_RELEASE(pBitmap);
	SAFE_RELEASE(pDecoder);
	SAFE_RELEASE(pFrame);
	return hr;
}

UINT D2D1Texture::Width() const
{
	return width;
}

UINT D2D1Texture::Height() const
{
	return height;
}
