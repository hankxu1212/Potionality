#include "Scene.hpp"

#include "../core/Files.hpp"
#include "../core/utils/Logger.hpp"
#include "Entity.hpp"
#include "../core/ResourceManager.h"
#include "../PlayMode.hpp"

Scene::Scene()
{
}

Scene::~Scene()
{
	Unload();
}

void Scene::Load()
{
    // load textures
    ResourceManager::LoadTexture(Files::Path("../resources/potions/Blue_potion.png").c_str(), true, "potion");
    ResourceManager::LoadTexture(Files::Path("../resources/characters/Guy_flipped.png").c_str(), true, "man");

    // PLAYER ////////////////////////////////////////////////////////////////// 
    // initiates an entity with coordinates x=200, y=200, and sprite size width=300, height=400, with rotation=45 degrees
    Entity* e = Instantiate(glm::vec2{200, 200}, glm::vec2{300, 400}, 45.0f);
    // adds a sprite renderer with the corresponding SHADER "sprite"
    e->AddComponent<SpriteRenderer>(SPRITE_SHADER); // constant defined in PlayMode.cpp
    // cuz player references SpriteRenderer on awake, SpriteRenderer needs to be added first
    e->AddComponent<Player>();

    // CUSTOMER TEST ////////////////////////////////////////////////////////////////// 
    // initiates an entity with coordinates x=200, y=200, and sprite size width=300, height=400, with rotation=45 degrees
    Entity* e2 = Instantiate(glm::vec2{ 500, 0 }, glm::vec2{ 700, 400 }, 0.0f);
    e2->AddComponent<SpriteRenderer>(SPRITE_SHADER); // constant defined in PlayMode.cpp
    e2->AddComponent<Customer>();
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