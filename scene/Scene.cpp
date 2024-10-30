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
    Deserialize("../resources/scenes/MainScene.json");
}

void Scene::Unload()
{
}

static void MakeComponents(const Scene::TValueArray& componentObj, Entity* newEntity)
{
    for (int i = 0; i < componentObj.size(); ++i)
    {
        if (auto& opt = componentObj[i].as_object())
        {
            const auto& val = opt.value();

            const std::string& type = val.at("type").as_string().value();

            const std::string& activeStr = val.at("active").as_string().value();
            bool active = strcmp(activeStr.c_str(), "true") == 0 ? true : false;

            if (type == "SpriteLoader") 
            {
                const std::string& shader = val.at("shader").as_string().value();
                newEntity->AddComponent<SpriteLoader>(active, shader);
            }
            else if (type == "SpriteRenderer") 
            {
                const std::string& texture = val.at("texture").as_string().value();
                newEntity->AddComponent<SpriteRenderer>(active, texture);
            }
            else if (type == "SpritesheetLoader") 
            {
                const std::string& shader = val.at("shader").as_string().value();
                uint32_t w = val.at("width").as_uint32t();
                uint32_t h = val.at("height").as_uint32t();
                newEntity->AddComponent<SpritesheetLoader>(active, shader, w, h);
            }
            else if (type == "SpritesheetRenderer")
            {
                const std::string& texture = val.at("texture").as_string().value();
                newEntity->AddComponent<SpritesheetRenderer>(active, texture);
            }
            else if (type == "Behaviour") 
            {
                const std::string& classname = val.at("classname").as_string().value();
                if (classname == "Player") {
                    newEntity->AddComponent<Player>(active);
                }
                else if (classname == "Customer") {
                    newEntity->AddComponent<Customer>(active);
                }
                else if (classname == "Ingredient") {
                    newEntity->AddComponent<Ingredient>(active);
                }
                else if (classname == "Potion") {
                    newEntity->AddComponent<Potion>(active);
                }
            }
        }
    }
}

static void MakeEntities(Scene* scene, const Scene::TValueArray& entityObj)
{
    for (int i = 0; i < entityObj.size(); ++i)
    {
        if (auto& opt = entityObj[i].as_object())
        {
            const auto& val = opt.value();

            const std::string& name = val.at("name").as_string().value();
            glm::vec2 translation = val.at("translation").as_vec2();
            float rotation = val.at("rotation").as_float();
            glm::vec2 scale = val.at("scale").as_vec2();
            float depth = val.at("depth").as_float();

            const std::string& activeStr = val.at("active").as_string().value();
            bool active = strcmp(activeStr.c_str(), "true") == 0 ? true : false;

            Entity* newEnt = scene->Instantiate(name, translation, scale, rotation, depth);
            newEnt->SetActive(active);

            const auto& components = val.at("components").as_array().value();
            MakeComponents(components, newEnt);
        }
    }
}

static void MakeTextures(const Scene::TValueArray& texObj)
{
    for (int i = 0; i < texObj.size(); ++i)
    {
        if (auto& opt = texObj[i].as_object())
        {
            const std::string& path = opt.value().at("src").as_string().value();
            const std::string& name = opt.value().at("name").as_string().value();

            ResourceManager::LoadTexture(path.c_str(), name);
        }
    }
}

void Scene::Deserialize(const std::string& path)
{
    sceneRootAbsolutePath = Files::Path(path);

    try {
        sejp::value loaded = sejp::load(sceneRootAbsolutePath.string());
        auto& map = loaded.as_object().value();

        MakeTextures(map.at("textures").as_array().value());
        MakeEntities(this, map.at("entities").as_array().value());
    }
    catch (std::exception& e) {
        LOG_ERROR_F("Failed to deserialize a scene with path: [{}]. Error: {}", std::move(path), e.what());
    }
}

void Scene::Update()
{
    ExecuteDestroyQueue();
    for (auto& ent : entities)
    {
        if (ent.second->isActive())
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


template<typename... TArgs>
Entity* Scene::Instantiate(const std::string& name, TArgs&... args)
{
    Entity* ent = new Entity(this, name, args...);
    SetEntity(ent);
    return ent;
}

template<typename... TArgs>
Entity* Scene::Instantiate(const std::string& name, TArgs&&... args)
{
    Entity* ent = new Entity(this, name, args...);
    SetEntity(ent);
    return ent;
}