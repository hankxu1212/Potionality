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
    ResourceManager::LoadTexture(Files::Path("../resources/characters/Full.png").c_str(), true, "player_idle");
    //ResourceManager::LoadTexture(Files::Path("../resources/characters/jump.png").c_str(), true, "player_jump");
    //ResourceManager::LoadTexture(Files::Path("../resources/characters/walk.png").c_str(), true, "player_walk");
    ResourceManager::LoadTexture(Files::Path("../resources/characters/Guy_flipped.png").c_str(), true, "man");
    ResourceManager::LoadTexture(Files::Path("../resources/ingredients/Flower.png").c_str(), true, "flower");
    ResourceManager::LoadTexture(Files::Path("../resources/ingredients/Mushroom.png").c_str(), true, "mushroom");
    ResourceManager::LoadTexture(Files::Path("../resources/ingredients/Purple_Quartz.png").c_str(), true, "purple_quartz");
    ResourceManager::LoadTexture(Files::Path("../resources/ingredients/White_Quartz.png").c_str(), true, "white_quartz");
    //ResourceManager::LoadTexture(Files::Path("../resources/animations/Smoke_5x5.png").c_str(), true, "Smoke5x5");
    //ResourceManager::LoadTexture(Files::Path("../resources/animations/Cat.png").c_str(), true, "Cat");

    // PLAYER ////////////////////////////////////////////////////////////////// 
    // initiates an entity with coordinates x=200, y=200, and sprite size width=96, height=96, with rotation=0 degrees
    Entity* e = Instantiate(glm::vec2{200, 200}, glm::vec2{96, 96}, 0.0f);
    // adds a sprite loader with the corresponding SHADER "sprite"
    e->AddComponent<SpritesheetLoader>(SPRITESHEET_SHADER, 6, 4); // constant defined in PlayMode.cpp
    // render this animated sprite by adding a sprite renderer component. note: the key must correspond exactly with the texture handle!
    e->AddComponent<SpritesheetRenderer>("player_idle");
    // adds a player Behaviour script for movement
    e->AddComponent<Player>();

    // CUSTOMER TEST ////////////////////////////////////////////////////////////////// 
    // TODO: Use something else for the counter/potion deposit place
    Entity* e2 = Instantiate(glm::vec2{1480, 340}, glm::vec2{400, 800}, 180.0f);
    e2->AddComponent<SpriteLoader>(SPRITE_SHADER);
    e2->AddComponent<SpriteRenderer>("man");
    e2->AddComponent<Customer>(); // TODO: add some proximity check

    // INGREDIENTS //////////////////////////////////////////////////
    // Note: Make sure the coordinates in ingredient_locations match the ones used in Instantiate()
    // TODO: There should be a way to use glm::vec2 as a key, which would be much more intuitive
    Entity* e3 = Instantiate(glm::vec2{800, 200}, glm::vec2{64, 64}, 0.0f);
    e3->AddComponent<SpriteLoader>(SPRITE_SHADER);
    e3->AddComponent<SpriteRenderer>("flower");
    e3->AddComponent<Ingredient>();
    ingredient_locations[{800, 200}] = e3;

    Entity* e4 = Instantiate(glm::vec2{1200, 600}, glm::vec2{64, 64}, 0.0f);
    e4->AddComponent<SpriteLoader>(SPRITE_SHADER);
    e4->AddComponent<SpriteRenderer>("mushroom");
    e4->AddComponent<Ingredient>();
    ingredient_locations[{1200, 600}] = e4;

    Entity* e5 = Instantiate(glm::vec2{400, 400}, glm::vec2{64, 64}, 0.0f);
    e5->AddComponent<SpriteLoader>(SPRITE_SHADER);
    e5->AddComponent<SpriteRenderer>("purple_quartz");
    e5->AddComponent<Ingredient>();
    ingredient_locations[{400, 400}] = e5;

    Entity* e6 = Instantiate(glm::vec2{600, 800}, glm::vec2{64, 64}, 0.0f);
    e6->AddComponent<SpriteLoader>(SPRITE_SHADER);
    e6->AddComponent<SpriteRenderer>("white_quartz");
    e6->AddComponent<Ingredient>();
    ingredient_locations[{600, 800}] = e6;

    Entity* test = GetClosestIngredient(glm::vec2{500, 500});
    LOG_INFO("Closest ingredient coordinates:");
    LOG_INFO(test->transform()->position.x);
    LOG_INFO(test->transform()->position.y);
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