#include "Scene.hpp"

#include "../core/Files.hpp"
#include "../core/utils/Logger.hpp"
#include "Entity.hpp"
#include "../core/ResourceManager.h"
#include "../PlayMode.hpp"
#include "../scripting/Player.hpp"
#include "../core/utils/sejp/sejp.hpp"

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
    ResourceManager::LoadTexture("../resources/background/Background.png", "background");
    ResourceManager::LoadTexture("../resources/potions/Red_potion.png", "red_potion");

    // Background 
    Entity* backgroundEntity = Instantiate("Background", glm::vec2{0, 0}, glm::vec2{1920, 1080}, 0.0f);
    backgroundEntity->AddComponent<SpriteLoader>(LIT_SPRT_SHADER);
    backgroundEntity->AddComponent<SpriteRenderer>("background");

    // PLAYER ////////////////////////////////////////////////////////////////// 
    Entity* e = Instantiate("Player", glm::vec2{200, 200}, glm::vec2{96, 96}, 0.0f);
    e->AddComponent<SpritesheetLoader>(LIT_SPRTSHEET_SHADER, 6, 11);
    e->AddComponent<SpritesheetRenderer>("player_idle");
    e->AddComponent<Player>();

    // CUSTOMER TEST ////////////////////////////////////////////////////////////////// 
    Entity* e2 = Instantiate("Customer1", glm::vec2{1500, 300}, glm::vec2{256, 256}, 0.0f);
    e2->AddComponent<SpriteLoader>(LIT_SPRT_SHADER);
    e2->AddComponent<SpriteRenderer>("counter");
    e2->AddComponent<Customer>(); // TODO: add some proximity check
    
    // INGREDIENTS //////////////////////////////////////////////////
    // TODO: Serialize scene loading, this is hideous
    Entity* e3 = Instantiate("FlowerIngredient", glm::vec2{800, 900}, glm::vec2{64, 64}, 0.0f);
    e3->AddComponent<SpriteLoader>(LIT_SPRT_SHADER);
    e3->AddComponent<SpriteRenderer>("flower");
    e3->AddComponent<Ingredient>();

    Entity* e4 = Instantiate("MushroomIngredient", glm::vec2{1000, 600}, glm::vec2{64, 64}, 0.0f);
    e4->AddComponent<SpriteLoader>(LIT_SPRT_SHADER);
    e4->AddComponent<SpriteRenderer>("mushroom");
    e4->AddComponent<Ingredient>();

    Entity* e5 = Instantiate("PurpleQuartzIngredient", glm::vec2{200, 400}, glm::vec2{64, 64}, 0.0f);
    e5->AddComponent<SpriteLoader>(LIT_SPRT_SHADER);
    e5->AddComponent<SpriteRenderer>("purple_quartz");
    e5->AddComponent<Ingredient>();

    Entity* e6 = Instantiate("WhiteQuartzIngredient", glm::vec2{400, 800}, glm::vec2{64, 64}, 0.0f);
    e6->AddComponent<SpriteLoader>(LIT_SPRT_SHADER);
    e6->AddComponent<SpriteRenderer>("white_quartz");
    e6->AddComponent<Ingredient>();
    
    Entity* e7 = Instantiate("Table", glm::vec2{500, 500}, glm::vec2{256, 256}, 0.0f);
    e7->AddComponent<SpriteLoader>(LIT_SPRT_SHADER);
    e7->AddComponent<SpriteRenderer>("table");

    Entity* e8 = Instantiate("RedPotion", glm::vec2{1600, 375}, glm::vec2{64, 64}, 0.0f);
    e8->AddComponent<SpriteLoader>(LIT_SPRT_SHADER);
    e8->AddComponent<SpriteRenderer>("red_potion");
    e8->AddComponent<Potion>(); 
    // Doesn't do much right now, but might be useful down the line
    // Sprites start as active, so deactivate if we don't want to immediately draw it
    SpriteRenderer* potionSprite = e8->GetComponent<SpriteRenderer>();
    potionSprite->Deactivate();
}

void Scene::Unload()
{
}

void Scene::Deserialize(const std::string& path)
{
}

void Scene::Update()
{
    ExecuteDestroyQueue();
    for (auto& ent : entities)
    {
        ent.second->Update();
    }
}

void Scene::Render()
{
}

void Scene::SetEntity(Entity* ent)
{
    entities[ent->uuid()] = std::unique_ptr<Entity>(ent);
}

void Scene::Destroy(Entity* ent)
{
    for (auto it = entities.begin(); it != entities.end(); ++it) 
    {
        if (it->second.get() == ent) 
        {
            m_DestroyQueue.push_back(it->second->uuid());
            break;
        }
    }
}

void Scene::Destroy(size_t uuid)
{
    m_DestroyQueue.push_back(uuid);
}

void Scene::Destroy(const std::string& name)
{
    for (auto it = entities.begin(); it != entities.end(); ++it) 
    {
        if (strcmp(it->second->name().c_str(), name.c_str()) == 0) 
        {
            m_DestroyQueue.push_back(it->second->uuid());
            break;
        }
    }
}

void Scene::ExecuteDestroyQueue()
{
    if (m_DestroyQueue.empty())
        return;

    for (int i = 0; i < m_DestroyQueue.size(); ++i)
    {
        entities.erase(m_DestroyQueue[i]);
    }
    m_DestroyQueue.clear();
}
