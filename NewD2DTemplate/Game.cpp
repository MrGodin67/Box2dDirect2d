#include "Game.h"
#include "Direct3DWindow.h"




Game::Game(Direct3DWindow & wnd)
	:
	window(wnd),
	gfx(wnd.ScreenWidth(),wnd.ScreenHeight(),wnd.WindowHandle(),
		true, FULL_SCREEN,1000.0f,0.01f),
	m_cam(&gfx,(float)wnd.ScreenWidth(), (float)wnd.ScreenHeight()),
	timerSprite(Locator::TextManager->GetFormat("Tahoma16"),RectF(10.0f,10.0f,120.0f,20.0f),L"",D2D1::ColorF(1.0f,1.0f,1.0f,1.0f)),
	numCreatedSprite(Locator::TextManager->GetFormat("Tahoma16"), RectF(10.0f, 80.0f, 120.0f, 100.0f), L"", D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f))
{
	m_cam.ConfineToMap(RectF(0.0f, 0.0f,3000.0f, (float)wnd.ScreenHeight()));
	LoadImages();
	LoadWorld();
	m_player = std::make_unique<Player>(m_world.get(), Vec2f(200.0f, 436.0f), D2D1::SizeF(20.0f, 64.0f));
}

bool Game::Play(const float& deltaTime)
{
	HRESULT hr;
	if (FAILED(hr = ConstructScene(deltaTime))) 
	{ return false; }
	if (FAILED(hr = RenderScene())) { return false; }
	return true;
}

HRESULT Game::ConstructScene(const float& deltaTime)
{
	if (window.kbd.KeyIsPressed(VK_ESCAPE))
		return E_FAIL;
	GameTime.Frame();
	timerSprite.UpdateText(std::to_wstring(GameTime.Fps()));
	HandleInput();
	m_world->Step(GameTime.DeltatTime(),6,2);
	m_player->Update(GameTime.DeltatTime(), window.kbd);

	Vec2f pos(m_player->GetPosition().x, m_player->GetPosition().y);
	m_cam.UpdatePosition(pos);
	numCreatedSprite.UpdateText(std::to_wstring(m_player->GetPosition().y));
	return S_OK;
}

HRESULT Game::RenderScene()
{
	HRESULT hr;
	hr = gfx.BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	if (FAILED(hr)) { return hr; }
	
	timerSprite.Draw(gfx);

	for (auto& box : m_Boxes)
		m_cam.Rasterize(box.GetDrawable());

	for (auto& sbox : m_Static)
		m_cam.Rasterize(sbox.GetDrawable());

	m_cam.Rasterize(m_player->GetDrawable());
	
	numCreatedSprite.Draw(gfx);
	hr = gfx.EndScene();
	if (FAILED(hr)) { return hr; }
	return hr;
}

void Game::EndApp()
{
	PostQuitMessage(0);
}

void Game::LoadImages()
{
	m_textures = std::make_unique<TextureManager>();
	Locator::SetImageManager(m_textures.get());
	std::vector<TextureManager::ImageData> images;
	images.emplace_back("level1", L"media\\level1.png", 64.0f, 64.0f);
	images.emplace_back("dude", L"media\\dude.png", 64.0f, 64.0f);
	m_textures->LoadImages(images);
}

void Game::LoadWorld()
{
	m_world = std::make_unique<b2World>(b2Vec2(0.0f, 16.0f));
	m_world->SetAllowSleeping(true);
	float y = 50.0f;
	float x = 100.0f;
	
	RandomGenerator<std::mt19937> randG;
	TextureManager::ImageClip clip;
	clip = m_textures->GetClip("level1", 5);
	for (int c = 0; c < 30; c++)
	{
		m_Boxes.emplace_back(m_world.get(), Vec2f(randG.Get<float>(10.0f, 500.0f), randG.Get<float>(630.0f, 635.0f)), 
			clip.bitmap, D2D1::SizeF(32.0f,32.0f),clip.rect.ToD2D(),1.0f);
		y += 8.0f;
		x += 10.0f;
		numItemsCreated++;
	}
	clip = m_textures->GetClip("level1", 1);
	x = 0.0f;
	for (int i = 0; i < 32; i++)
	{
		m_Static.emplace_back(m_world.get(), Vec2f(x, 700.0f), clip.bitmap, D2D1::SizeF(64.0f, 64.0f), clip.rect.ToD2D(), 1.0f);
		x += 64.0f;
		numItemsCreated++;
	}
	x = 128.0f;
	for (int i = 0; i < 4; i++)
	{
		m_Static.emplace_back(m_world.get(), Vec2f(x, 500.0f), clip.bitmap, D2D1::SizeF(64.0f, 64.0f), clip.rect.ToD2D(), 1.0f);
		x += 64.0f;
		numItemsCreated++;
	}

	x = 632.0f;
	for (int i = 0; i < 4; i++)
	{
		m_Static.emplace_back(m_world.get(), Vec2f(x, 400.0f), clip.bitmap, D2D1::SizeF(64.0f, 64.0f), clip.rect.ToD2D(), 1.0f);
		x += 64.0f;
		numItemsCreated++;
	}
	numCreatedSprite.UpdateText(std::to_wstring(numItemsCreated));
}

void Game::HandleInput()
{
	Vec2f scroll = { 0.0f,0.0f };

	if (window.kbd.KeyIsPressed('D'))
		scroll.x = 10.0f;
	if (window.kbd.KeyIsPressed('A'))
		scroll.x = -10.0f;

	if(scroll != Vec2f(0.0f,0.0f))
	   m_cam.Scroll(scroll);
}

void Game::LoadAudio()
{
	m_soundFX = std::make_unique<SoundManager>();
	Locator::SetSoundManager(m_soundFX.get());
}




