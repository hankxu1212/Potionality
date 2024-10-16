#include "PlayMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>
#include <random>

#include "math/Math.hpp"
#include "scene/SceneManager.hpp"

void PlayMode::Init()
{
	// create all modules
	auto& registry = Module::GetRegistry();
	for (auto it = registry.begin(); it != registry.end(); ++it)
		CreateModule(it);

	// load some testing pngs
	Entity e;
	e.Load("resources/test.png");
	
	PPU::Get()->tile_table[32] = {{
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Row 1
		0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Row 2
		0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  // Row 3
		0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,  // Row 4
		0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,  // Row 5
		0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,  // Row 6
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,  // Row 7
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01   // Row 8
	}};

	PPU::Get()->palette_table.resize(64);
	PPU::Get()->sprites.resize(128);

	//used for the player:
	PPU::Get()->palette_table[7] = {
		glm::u8vec4(0xff, 0xff, 0x00, 0xff),
		glm::u8vec4(0x00, 0xff, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
	};
}

PlayMode::~PlayMode() {
	DestroyModules();
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) 
{
	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {
	UpdateModules(); // update all engine modules except Render stage

	//slowly rotates through [0,1):
	// (will be used to set background color)
	background_fade += elapsed / 10.0f;
	background_fade -= std::floor(background_fade);

	constexpr float PlayerSpeed = 30.0f;
	if (left.pressed) player_at.x -= PlayerSpeed * elapsed;
	if (right.pressed) player_at.x += PlayerSpeed * elapsed;
	if (down.pressed) player_at.y -= PlayerSpeed * elapsed;
	if (up.pressed) player_at.y += PlayerSpeed * elapsed;

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---

	//background color will be some hsv-like fade:
	PPU::Get()->background_color = glm::u8vec4(
		std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 0.0f / 3.0f) ) ) ))),
		std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 1.0f / 3.0f) ) ) ))),
		std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 2.0f / 3.0f) ) ) ))),
		0xff
	);

	//background scroll:
	PPU::Get()->background_position.x = int32_t(-0.5f * player_at.x);
	PPU::Get()->background_position.y = int32_t(-0.5f * player_at.y);

	//player sprite:
	PPU::Get()->sprites[0].x = int8_t(player_at.x);
	PPU::Get()->sprites[0].y = int8_t(player_at.y);
	PPU::Get()->sprites[0].index = 32;
	PPU::Get()->sprites[0].attributes = 7;

	//--- actually draw ---
	PPU::Get()->draw(drawable_size);

	UpdateStage(Module::UpdateStage::Render);
}

void PlayMode::UpdateModules()
{
	UpdateStage(Module::UpdateStage::Pre);
	UpdateStage(Module::UpdateStage::Normal);
	UpdateStage(Module::UpdateStage::Post);
}

void PlayMode::DestroyModules()
{
	DestroyStage(Module::DestroyStage::Pre);
	DestroyStage(Module::DestroyStage::Normal);
	DestroyStage(Module::DestroyStage::Post);
}

void PlayMode::CreateModule(Module::RegistryMap::const_iterator it)
{
	if (m_Modules.find(it->first) != m_Modules.end())
		return;

	// TODO: Prevent circular dependencies.
	for (auto requireId : it->second.requiredModules)
		CreateModule(Module::GetRegistry().find(requireId));

	auto&& module = it->second.create();
	m_Modules[it->first] = std::move(module);
	m_ModuleStages[it->second.stage].emplace_back(it->first);
	m_ModuleDestroyStages[it->second.destroyStage].emplace_back(it->first);
}

void PlayMode::DestroyModule(TypeId id, Module::DestroyStage stage)
{
	if (!m_Modules[id])
		return;

	// Destroy all module dependencies first.
	for (const auto& [registrarId, registrar] : Module::GetRegistry()) {
		if (std::find(registrar.requiredModules.begin(), registrar.requiredModules.end(), id) != registrar.requiredModules.end()
			&& registrar.destroyStage == stage) {
			DestroyModule(registrarId, stage);
		}
	}

	m_Modules[id].reset();
}

void PlayMode::UpdateStage(Module::UpdateStage stage)
{
	for (auto& moduleId : m_ModuleStages[stage])
		m_Modules[moduleId]->Update();
}

void PlayMode::DestroyStage(Module::DestroyStage stage)
{
	for (auto& moduleId : m_ModuleDestroyStages[stage])
		DestroyModule(moduleId, stage);
}