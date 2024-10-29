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

void PlayMode::Init()
{
	// create all modules
	loadText();
	auto& registry = Module::GetRegistry();
	for (auto it = registry.begin(); it != registry.end(); ++it)
		CreateModule(it);

	glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);

	// configure static sprite shader
	ResourceManager::LoadShader(Files::Path("../shaders/sprite.vert").c_str(), Files::Path("../shaders/sprite.frag").c_str(), nullptr, SPRITE_SHADER);
	ResourceManager::GetShader(SPRITE_SHADER).Use().SetInteger("image", 0);
	ResourceManager::GetShader(SPRITE_SHADER).SetMatrix4("projection", projection);

	// configure dynamic spritesheet shader
	ResourceManager::LoadShader(Files::Path("../shaders/spritesheet.vert").c_str(), Files::Path("../shaders/spritesheet.frag").c_str(), nullptr, SPRITESHEET_SHADER);
	ResourceManager::GetShader(SPRITESHEET_SHADER).Use().SetInteger("image", 0);
	ResourceManager::GetShader(SPRITESHEET_SHADER).SetMatrix4("projection", projection);

	glm::vec2 lightPos(1200, 700);

	// configure lit sprite shader
	ResourceManager::LoadShader(Files::Path("../shaders/litsprite.vert").c_str(), Files::Path("../shaders/litsprite.frag").c_str(), nullptr, LIT_SPRT_SHADER);
	ResourceManager::GetShader(LIT_SPRT_SHADER).Use().SetInteger("image", 0);
	ResourceManager::GetShader(LIT_SPRT_SHADER).SetMatrix4("projection", projection);
	ResourceManager::GetShader(LIT_SPRT_SHADER).SetVector2f("lightPosition", lightPos);

	// configure dynamic lit sprite shader
	ResourceManager::LoadShader(Files::Path("../shaders/litspritesheet.vert").c_str(), Files::Path("../shaders/litspritesheet.frag").c_str(), nullptr, LIT_SPRTSHEET_SHADER);
	ResourceManager::GetShader(LIT_SPRTSHEET_SHADER).Use().SetInteger("image", 0);
	ResourceManager::GetShader(LIT_SPRTSHEET_SHADER).SetMatrix4("projection", projection);
	ResourceManager::GetShader(LIT_SPRTSHEET_SHADER).SetVector2f("lightPosition", lightPos);

	// initialize time class
	Time::Now = 0;

	// initialize scripting engine
	ScriptingEngine *engine = new ScriptingEngine();

	// requires an active scripting engine
	// for each component, call Awake
	SceneManager::Get()->LoadActiveScene();

	// for each component, call Start
	PushLayer(engine);
}

PlayMode::~PlayMode() {
	DestroyStage(Module::DestroyStage::Pre);

	m_LayerStack.Detach();

	DestroyStage(Module::DestroyStage::Normal);

	m_LayerStack.Destroy();

	DestroyStage(Module::DestroyStage::Post);
}

// each layerstack handles events
bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) 
{
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