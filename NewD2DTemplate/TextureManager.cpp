#include "TextureManager.h"

void TextureManager::LoadImages(std::vector<ImageData>& images)
{
	for (size_t i = 0;i < images.size(); i++)
	{
		auto& it = m_textures.find( images[i].name);
		if (it == m_textures.end())// if not found
		{
			m_textures[images[i].name] = std::make_unique<SpriteSheet>(images[i].filepath, images[i].clipW, images[i].clipH);
			assert(m_textures[images[i].name]);
		}
		
	}
}

TextureManager::ImageClip TextureManager::GetClip(std::string imageName, const int & index)
{
	
	auto& it = m_textures.find(imageName);
	if (it != m_textures.end())// if found
	{
		ImageClip clip;
		clip.bitmap = m_textures[imageName]->GetTexture();
		clip.rect = m_textures[imageName]->GetClippedImage(index);
		return clip;
	}
	return ImageClip();
}

SpriteSheet * TextureManager::GetImage(const std::string& name)
{
	auto& it = m_textures.find(name);
	if (it != m_textures.end())// if found
	{		
		return  m_textures[name].get();
	}
	
	return nullptr;
}
