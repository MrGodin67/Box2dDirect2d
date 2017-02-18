#pragma once
#include "includes.h"
#include "SoundManager.h"
#include "DirectWrite.h"
#include "TextureManager.h"
class Locator
{
public:
	static SoundManager* SoundEngine;
	static ID2D1RenderTarget* RenderTarget;
	static TextHandler* TextManager;
	static TextureManager* ImageManager;
	static void SetD2DRenderTarget(ID2D1RenderTarget* rt);
	static void SetSoundManager(SoundManager* sm);
	static void SetTextManager(TextHandler* tx);
	static void SetImageManager(TextureManager* tmgr);
	
};