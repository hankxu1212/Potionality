#include "Scene.hpp"
#include "../core/Files.hpp"
#include "../core/utils/Logger.hpp"

#include <iostream>
#include "Entity.hpp"

Scene::Scene()
{
}

Scene::~Scene()
{
	Unload();
}

void Scene::Load()
{
    auto e = Instantiate();
    // e->Load("../resources/test.png", 2, 2);
    //e->Load("../resources/characters/Guy_flipped.png", 8, 16);
    e->AddComponent<Player>();
}

void Scene::Unload()
{
}

void Scene::Update()
{
    for (auto& ent : entities)
    {
        ent->Update();
    }
}

void Scene::Render()
{
}