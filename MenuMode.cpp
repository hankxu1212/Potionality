#include "MenuMode.hpp"
#include "PlayMode.hpp"

#include "renderText.hpp"
#include "sprites/SpriteLoader.h"
#include "sound/SoundManager.hpp"

SpriteLoader* sprtRenderer;

MenuMode::~MenuMode()
{
	delete sprtRenderer;
}

void MenuMode::Init()
{
	sprtRenderer = new SpriteLoader(SPRITE_SHADER);
}

bool MenuMode::handle_event(SDL_Event const& evt, glm::uvec2 const& window_size)
{
	if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_UP) {
			OnUpPressed();
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_DOWN) {
			OnDownPressed();
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_SPACE) {
			OnSpacePressed();
			return true;
		}
	}
	return false;
}

void MenuMode::update(float elapsed)
{
	TextRenderer::Get()->RenderText("Potionality", 500, 700, 0.005f);
	TextRenderer::Get()->RenderText("Start Game/Continue", 500, 300, 0.002f);
	TextRenderer::Get()->RenderText("Quit", 500, 200, 0.002f);

	sprtRenderer->DrawSprite("UI_Circle", glm::vec2{ 400, positions[m_CurrentOption] }, glm::vec2{32, 32});
}
 
void MenuMode::draw(glm::uvec2 const& drawable_size)
{
}

void MenuMode::OnUpPressed()
{
	m_CurrentOption = (m_CurrentOption + 1) % 2;
	
	SoundManager::Get()->PlayOneShot("LightSwitchSFX", 0.2f);
}

void MenuMode::OnDownPressed()
{
	m_CurrentOption--;
	if (m_CurrentOption < 0)
		m_CurrentOption += 2;

	SoundManager::Get()->PlayOneShot("LightSwitchSFX", 0.2f);
}

void MenuMode::OnSpacePressed()
{
	SoundManager::Get()->PlayOneShot("LightSwitchSFX", 0.2f);

	if (m_CurrentOption == 0)
	{
		PlayMode* playMode = dynamic_cast<PlayMode*>(Mode::current.get());
		playMode->inMenuMode = false;
	}
	else
	{
		// quit
		Mode::set_current(nullptr);
	}
}