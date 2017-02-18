#include "SpriteSheet.h"
#include "Locator.h"
SpriteSheet::SpriteSheet(std::wstring filename, float clipWidth, float clipHeight)
	:clipWidth(clipWidth),clipHeight(clipHeight)
{
	texture = std::make_unique<D2D1Texture>(Locator::RenderTarget, filename);
	assert(texture);
	width = (float)texture->Width();
	height = (float)texture->Height();
}

SpriteSheet::~SpriteSheet()
{
}

RectF SpriteSheet::GetClippedImage(const int & index)
{
	float columns = width / clipWidth;
	int col = index % (int)columns;
	int row =  index / (int)columns;
	return RectF((float)col*clipWidth,(float)row*clipWidth,
		((float)col*clipWidth)+clipWidth,((float)row*clipWidth)+clipWidth);
}

ID2D1Bitmap * SpriteSheet::GetTexture()
{
	return texture->GetBitmap();
}
