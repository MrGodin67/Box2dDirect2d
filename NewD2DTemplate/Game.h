#pragma once
#include "Graphics.h"

#include <memory>

#include "Camera.h"
#include "GameState.h"
#include "SpriteSheet.h"
#include "Sprite2.h"
#include "SoundManager.h"
#include "Timer.h"
#include "VecRect.h"
#include "TextureManager.h"
#include "Box.h"
#include "Box2D\Box2D.h"
#include "Randomizer.h"
#include "Player.h"
#pragma comment(lib,"Box2d.lib")



class Game
{
	
	class Direct3DWindow& window;
	Graphics gfx;
	void EndApp();
	void LoadImages();
	void LoadWorld();
	void HandleInput();
	void LoadAudio();
	void LoadLevel();
	
private:
	_GameState m_gameState = _GameState::paused;
	std::unique_ptr<SoundManager> m_soundFX;
	std::unique_ptr<b2World> m_world;
	std::unique_ptr<TextureManager> m_textures;
	std::unique_ptr<Player> m_player;
	std::vector<DynamicBox> m_Boxes;
	std::vector<StaticBox> m_Static;
	TextSprite timerSprite;
	
	TextSprite numCreatedSprite;
	Timer GameTime;
	Camera m_cam;
	int numItemsCreated = 0;
public:
	Game(class Direct3DWindow& wnd);
	bool Play(const float& deltaTime);
	HRESULT ConstructScene(const float& deltaTime);
	HRESULT RenderScene();
	
	////////////////////////////////////
	// LOGIC
	////////////////////////////////////

private:
	
	class_scope constexpr float tileSize = 64.0f;
	class_scope constexpr float tileOffset = 64.0f * 0.5f;
};
