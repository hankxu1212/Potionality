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
    auto e = Instantiate(glm::vec2{200, 200}, glm::vec2{300, 400}, 45.0f);
    // e->Load("../resources/test.png", 2, 2);
    //e->Load("../resources/characters/Guy_flipped.png", 8, 16);
    e->AddComponent<Player>();
    e->AddComponent<SpriteRenderer>("sprite");
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