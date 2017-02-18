#pragma once
#include "includes.h"
#include "D2D1Texture.h"

#include "Rect.h"
class SpriteSheet
{
	float width;
	float height;
	float clipWidth;
	float clipHeight;
	std::unique_ptr<D2D1Texture> texture;
public:
	SpriteSheet(std::wstring filename,float clipWidth,float clipHeight);
	~SpriteSheet();
	RectF GetClippedImage(const int& index);
	ID2D1Bitmap* GetTexture();
};
