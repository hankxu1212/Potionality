#include "Scene.hpp"
#include "../core/Files.hpp"

#include <iostream>

void Scene::Load(const std::string& path)
{
	if (!Files::Exists(Files::Path(path)))
		throw std::runtime_error("Path [" + path + "] does not exist.");

    // deserialize here
}

Scene::~Scene()
{
	Unload();
}

void Scene::Unload()
{
}

void Scene::Update()
{
}

void Scene::Render()
{

}