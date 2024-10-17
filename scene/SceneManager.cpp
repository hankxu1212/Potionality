#include "SceneManager.hpp"
#include "Entity.hpp"

SceneManager::SceneManager()
{
	scene = std::make_unique<Scene>();
	LoadDefault();
}

SceneManager::~SceneManager()
{
	Shutdown();
}

void SceneManager::LoadDefault()
{
}

void SceneManager::Update()
{
	if (!scene)
		return;

	scene->Update();
	scene->Render();
}

void SceneManager::Shutdown()
{
	if (scene) {
		scene->Unload();
		scene.reset();
	}
}