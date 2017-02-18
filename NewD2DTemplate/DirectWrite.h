#pragma once

#include <dwrite.h>
#include <DirectXMath.h>
#include <d2d1.h>
#include <map>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <unordered_map>
using namespace DirectX;
using namespace std;
class DirectWrite
{
public:
	DirectWrite();
	~DirectWrite();

	// Creates a format for all text you want to write in this style and font
	// The returned text format interface can get and set line spacing and 
	// paragraph alignment as well as word wrapping.
	IDWriteTextFormat *CreateFormat(LPWSTR FontName, float FontSize,
		DWRITE_FONT_WEIGHT FontWeight = DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE FontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH FontStretch = DWRITE_FONT_STRETCH_NORMAL,
		LPWSTR FontLocale = L"en-us");

private:
	IDWriteFactory *pFactory;
};

typedef std::pair<std::wstring, float> FontPair;
typedef std::unordered_map<std::string, IDWriteTextFormat*> FontMap;
class TextHandler
{
private:
	DirectWrite m_FontFactory;
	FontMap m_Fonts;
public:
	struct FontDesc
	{
		LPWSTR text = L"\0";
		IDWriteTextFormat *pFormat = nullptr;
		D2D1_RECT_F drawRect = D2D1::RectF();
		D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
		FontDesc() {}
		FontDesc(LPWSTR n, IDWriteTextFormat* f, D2D1_RECT_F rect, D2D1_COLOR_F color)
			:text(n), pFormat(f), drawRect(rect), color(color)
		{}
	};
public:
	TextHandler()
	{}
	~TextHandler()
	{
		for (auto it : m_Fonts)
			it.second->Release();
	}
	bool InitializeFont(const string fontTitle, const FontPair desc)
	{
		m_Fonts[fontTitle] = m_FontFactory.CreateFormat(const_cast<LPWSTR>(desc.first.c_str()), desc.second);
		if (!m_Fonts[fontTitle]) { return false; }
		return true;
	}
	IDWriteTextFormat* GetFormat(const string fontTitle)
	{
		return m_Fonts[fontTitle];
	}

};

