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
#include "SpriteRenderer.h"

SpriteRenderer* Renderer;

void PlayMode::Init()
{
	// create all modules
	auto& registry = Module::GetRegistry();
	for (auto it = registry.begin(); it != registry.end(); ++it)
		CreateModule(it);
	
	loadText(characters,font_texs); //load in the font

	ResourceManager::LoadShader(Files::Path("../shaders/sprite.vert").c_str(), Files::Path("../shaders/sprite.frag").c_str(), nullptr, "sprite");

	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

	// set render-specific controls
	Shader spriteShader = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(spriteShader);

	// load textures
	ResourceManager::LoadTexture(Files::Path("../resources/potions/Blue_potion.png").c_str(), true, "face");

	// initialize time class
	Time::Now = 0;

	// initialize scripting engine
	PushLayer(new ScriptingEngine());

	SceneManager::Get()->LoadActiveScene();

}

PlayMode::~PlayMode() {
	delete Renderer;

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

void PlayMode::update(float elapsed) {
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

	Texture2D spr = ResourceManager::GetTexture("face");
	Renderer->DrawSprite(spr,
		glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	
	RenderText("abcdefghijklmnopqrstuvwxyz wow testing character's", -.90f,-0.8f, .003f, characters,font_texs);
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