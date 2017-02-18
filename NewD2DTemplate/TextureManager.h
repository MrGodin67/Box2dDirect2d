#pragma once
#include "includes.h"

#include "SpriteSheet.h"

static const char textureNames[6][256] =
{
	{ "" },
	{ "" },
	{ "" },
	{ "" },
	{ "" },
	{ "" }
};



class TextureManager
{
	std::unordered_map<std::string, std::unique_ptr<SpriteSheet>> m_textures;
public:
	struct ImageData
	{
		std::string name;
		std::wstring filepath;
		float clipW, clipH;
		ImageData(std::string in_name, std::wstring in_filepath, float in_clipW, float in_clipH)
			:name(in_name), filepath(in_filepath), clipW(in_clipW), clipH(in_clipH)
		{}
	};
	struct ImageClip
	{
		ID2D1Bitmap* bitmap = nullptr;
		RectF rect = {0.0f,0.0f,0.0f,0.0f};
	};
public:
	TextureManager() {}
	~TextureManager() {}
	void LoadImages(std::vector<ImageData>& images);
	ImageClip GetClip(std::string imageName, const int& index);
	SpriteSheet* GetImage(const std::string& name);
};