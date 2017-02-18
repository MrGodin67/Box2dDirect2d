

#include "DirectWrite.h"
#include "Direct3D_2D.h"

DirectWrite::DirectWrite()
{

	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		(IUnknown**)(&pFactory));


}

IDWriteTextFormat *DirectWrite::CreateFormat(LPWSTR FontName, float FontSize,
	DWRITE_FONT_WEIGHT FontWeight, DWRITE_FONT_STYLE FontStyle,
	DWRITE_FONT_STRETCH FontStretch, LPWSTR FontLocale)
{

	IDWriteTextFormat *pFormat = nullptr;
	HRESULT hr = pFactory->CreateTextFormat(FontName, nullptr, FontWeight, FontStyle,
		FontStretch, FontSize, FontLocale, &pFormat);

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed to create TextFormat.",
			L"Error creating TextFormat.", MB_OK);
		PostQuitMessage(0);
	}

	return pFormat;
}



DirectWrite::~DirectWrite()
{
	SAFE_RELEASE(pFactory);
}


