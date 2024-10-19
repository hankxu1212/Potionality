#include "PlayMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>
#include <random>

#include "math/Math.hpp"
#include "scene/SceneManager.hpp"
#include "core/utils/Logger.hpp"
#include "core/Time.hpp"

void PlayMode::Init()
{
	// create all modules
	auto& registry = Module::GetRegistry();
	for (auto it = registry.begin(); it != registry.end(); ++it)
		CreateModule(it);
	
	loadText(characters,font_texs); //load in the font

	// initialize time class
	Time::Now = 0;

	SceneManager::Get()->LoadActiveScene();
}

PlayMode::~PlayMode() {
	DestroyStage(Module::DestroyStage::Pre);

	m_LayerStack.Detach();

	DestroyStage(Module::DestroyStage::Normal);

	m_LayerStack.Destroy();

	DestroyStage(Module::DestroyStage::Post);
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) 
{
	// each layerstack handles events
	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
	{
		if ((*it)->OnEvent(evt))
			break;
	}

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
	Time::DeltaTime = elapsed;
	Time::Now += elapsed;
	// runs application updates. dont edit or add code before this. 
	{
		UpdateStage(Module::UpdateStage::Pre);
		UpdateStage(Module::UpdateStage::Normal);

		for (Layer* layer : m_LayerStack)
		{
			layer->OnUpdate();
		}

		UpdateStage(Module::UpdateStage::Post);
	}

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
	
	//--- actually draw ---
	PPU::Get()->draw(drawable_size);

	UpdateStage(Module::UpdateStage::Render);
	
	// {
	// 	// ok why isn't the background clear...?
	// 	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	// 	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 	glEnable(GL_DEPTH_TEST);
	// 	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.
	// 	RenderText("abcdefghijklmnopqrstuvwxyz wow testing character's", -.90f,-0.8f, .003f, characters,font_texs);
	// }

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