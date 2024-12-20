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
#include "scripting/ScriptingEngine.hpp"
#include "core/ResourceManager.h"
#include "MenuMode.hpp"
#include "sound/SoundManager.hpp"

MenuMode* mm;

void PlayMode::Init()
{
	// depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -10.0f, 10.0f);

	// configure static sprite shader
	ResourceManager::LoadShader(Files::Path("shaders/sprite.vert").c_str(), Files::Path("shaders/sprite.frag").c_str(), nullptr, SPRITE_SHADER);
	ResourceManager::GetShader(SPRITE_SHADER).Use().SetInteger("image", 0);
	ResourceManager::GetShader(SPRITE_SHADER).SetMatrix4("projection", projection);

	// configure dynamic spritesheet shader
	ResourceManager::LoadShader(Files::Path("shaders/spritesheet.vert").c_str(), Files::Path("shaders/spritesheet.frag").c_str(), nullptr, SPRITESHEET_SHADER);
	ResourceManager::GetShader(SPRITESHEET_SHADER).Use().SetInteger("image", 0);
	ResourceManager::GetShader(SPRITESHEET_SHADER).SetMatrix4("projection", projection);

	// configure font rendering shader
	ResourceManager::LoadShader(Files::Path("shaders/font.vert").c_str(), Files::Path("shaders/font.frag").c_str(), nullptr, FONT_SHADER);
	ResourceManager::GetShader(FONT_SHADER).Use().SetInteger("image", 0);

	// create all modules
	auto& registry = Module::GetRegistry();
	for (auto it = registry.begin(); it != registry.end(); ++it)
		CreateModule(it);

	// initialize time class
	Time::Now = 0;

	// initialize scripting engine
	ScriptingEngine *engine = new ScriptingEngine();

	// requires an active scripting engine
	// for each component, call Awake
	SceneManager::Get()->LoadActiveScene();

	// for each component, call Start
	PushLayer(engine);

	mm = new MenuMode();
	mm->Init();
}

PlayMode::~PlayMode() 
{
	delete mm;

	DestroyStage(Module::DestroyStage::Pre);

	m_LayerStack.Detach();

	DestroyStage(Module::DestroyStage::Normal);

	m_LayerStack.Destroy();

	DestroyStage(Module::DestroyStage::Post);
}

// each layerstack handles events
bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) 
{
	if (inMenuMode)
		return mm->handle_event(evt, window_size);

	if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			OnEscapePressed();
			return true;
		}
	}

	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
	{
		if ((*it)->OnEvent(evt))
			return true;
	}
	return false;
}

void PlayMode::update(float elapsed) 
{
	Time::Now += elapsed;
	Time::DeltaTime = elapsed;

	if (inMenuMode) {
		mm->update(elapsed);
		return;
	}

	UpdateStage(Module::UpdateStage::Pre);

	UpdateStage(Module::UpdateStage::Normal);

	for (Layer* layer : m_LayerStack)
	{
		layer->OnUpdate();
	}

	UpdateStage(Module::UpdateStage::Post);
}

void PlayMode::draw(glm::uvec2 const &drawable_size) 
{
	UpdateStage(Module::UpdateStage::Render);
}

void PlayMode::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
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

void PlayMode::OnEscapePressed()
{
	SoundManager::Get()->PlayOneShot("LightSwitchSFX", 0.2f);
	inMenuMode = !inMenuMode;
}
