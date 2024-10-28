#include "Scene.hpp"

#include "../core/Files.hpp"
#include "../core/utils/Logger.hpp"
#include "Entity.hpp"
#include "../core/ResourceManager.h"
#include "../PlayMode.hpp"
#include "../scripting/Player.hpp"

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
    ResourceManager::LoadTexture("../resources/characters/Full.png", "player_idle");
    ResourceManager::LoadTexture("../resources/ingredients/Flower.png", "flower");
    ResourceManager::LoadTexture("../resources/ingredients/Mushroom.png", "mushroom");
    ResourceManager::LoadTexture("../resources/ingredients/Purple_Quartz.png", "purple_quartz");
    ResourceManager::LoadTexture("../resources/ingredients/White_Quartz.png", "white_quartz");
    ResourceManager::LoadTexture("../resources/background/Table.png", "table");
    ResourceManager::LoadTexture("../resources/background/Counter.png", "counter");

    // PLAYER ////////////////////////////////////////////////////////////////// 
    Entity* e = Instantiate(glm::vec2{200, 200}, glm::vec2{96, 96}, 0.0f);
    e->AddComponent<SpritesheetLoader>(SPRITESHEET_SHADER, 6, 7);
    e->AddComponent<SpritesheetRenderer>("player_idle");
    e->AddComponent<Player>();

    // CUSTOMER TEST ////////////////////////////////////////////////////////////////// 
    // TODO: Use something else for the counter/potion deposit place
    Entity* e2 = Instantiate(glm::vec2{1500, 300}, glm::vec2{256, 256}, 0.0f);
    e2->AddComponent<SpriteLoader>(SPRITE_SHADER);
    e2->AddComponent<SpriteRenderer>("counter");
    e2->AddComponent<Customer>(); // TODO: add some proximity check
    

    // INGREDIENTS //////////////////////////////////////////////////
    // TODO: Serialize scene loading, this is hideous
    Entity* e3 = Instantiate(glm::vec2{800, 200}, glm::vec2{64, 64}, 0.0f);
    e3->AddComponent<SpriteLoader>(SPRITE_SHADER);
    e3->AddComponent<SpriteRenderer>("flower");
    e3->AddComponent<Ingredient>();

    Entity* e4 = Instantiate(glm::vec2{1200, 600}, glm::vec2{64, 64}, 0.0f);
    e4->AddComponent<SpriteLoader>(SPRITE_SHADER);
    e4->AddComponent<SpriteRenderer>("mushroom");
    e4->AddComponent<Ingredient>();

    Entity* e5 = Instantiate(glm::vec2{400, 400}, glm::vec2{64, 64}, 0.0f);
    e5->AddComponent<SpriteLoader>(SPRITE_SHADER);
    e5->AddComponent<SpriteRenderer>("purple_quartz");
    e5->AddComponent<Ingredient>();

    Entity* e6 = Instantiate(glm::vec2{600, 800}, glm::vec2{64, 64}, 0.0f);
    e6->AddComponent<SpriteLoader>(SPRITE_SHADER);
    e6->AddComponent<SpriteRenderer>("white_quartz");
    e6->AddComponent<Ingredient>();
    
    Entity* e7 = Instantiate(glm::vec2{500, 500}, glm::vec2{256, 256}, 0.0f);
    e7->AddComponent<SpriteLoader>(SPRITE_SHADER);
    e7->AddComponent<SpriteRenderer>("table");
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
