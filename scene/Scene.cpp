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
    ResourceManager::LoadTexture(Files::Path("../resources/animations/Smoke_5x5.png").c_str(), true, "Smoke5x5");
    ResourceManager::LoadTexture(Files::Path("../resources/animations/Cat.png").c_str(), true, "Cat");

    // PLAYER ////////////////////////////////////////////////////////////////// 
    // initiates an entity with coordinates x=200, y=200, and sprite size width=300, height=400, with rotation=45 degrees
    Entity* e = Instantiate(glm::vec2{200, 200}, glm::vec2{300, 400}, 45.0f);
    // adds a sprite loader with the corresponding SHADER "sprite"
    e->AddComponent<SpriteLoader>(SPRITE_SHADER); // constant defined in PlayMode.cpp
    // render this static sprite by adding a sprite renderer component. note: the key must correspond exactly with the texture handle!
    e->AddComponent<SpriteRenderer>("potion");
    // adds a player Behaviour script for movement
    e->AddComponent<Player>();

    // CUSTOMER TEST ////////////////////////////////////////////////////////////////// 
    Entity* e2 = Instantiate(glm::vec2{ 500, 0 }, glm::vec2{ 700, 400 }, 0.0f);
    e2->AddComponent<SpriteLoader>(SPRITE_SHADER);
    e2->AddComponent<SpriteRenderer>("man");
    e2->AddComponent<Customer>(); // TODO: add some proximity check

    //// SPRITESHEET TEST ////////////////////////////////////////////////////////////////// 
    Entity* e3 = Instantiate(glm::vec2{ 400, 0 }, glm::vec2{ 200, 200 }, 0.0f);
    e3->AddComponent<SpritesheetLoader>(SPRITESHEET_SHADER, 5, 5);
    e3->AddComponent<SpritesheetRenderer>("Smoke5x5");

    //// CAT!! ////////////////////////////////////////////////////////////////// 
    Entity* e4 = Instantiate(glm::vec2{ 90, 90 }, glm::vec2{ 200, 200 }, 0.0f);
    e4->AddComponent<SpritesheetLoader>(SPRITESHEET_SHADER, 10, 10);
    e4->AddComponent<SpritesheetRenderer>("Cat");
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