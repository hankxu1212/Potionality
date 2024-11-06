#pragma once

#include <string>
#include <list>
#include <memory>
#include <glm/glm.hpp>
#include <unordered_map>
#include <map>
#include <filesystem>

#include "../core/utils/sejp/sejp.hpp"

class Entity;

class Scene
{
public:
	Scene();
	~Scene();

	void Load();

	void Unload();

	void Update();

	void Render();

public: // serialization
	// { name, serialized object map }
	using TValueMap = std::map<std::string, sejp::value>;

	using TValueArray = std::vector<sejp::value>;

	// { name, serialized object unordered map }
	using TValueUMap = std::unordered_map<std::string, sejp::value>;

	void Deserialize(const std::string& path);

public: // event functions. Do not create function definitions!
	template<typename T>
	void OnComponentAdded(Entity&, T&);

	template<typename T>
	void OnComponentRemoved(Entity&, T&);

	void SetEntity(Entity* ent);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Entity management

	Entity* Instantiate(const std::string& name, const glm::vec2& pos, const glm::vec2& size, float rotation, float depth);

	// destroy by entity handle, O(n). Can use.
	void Destroy(Entity* ent);

	// destroy by entity id O(log n). Should use.
	void Destroy(size_t uuid);

	// destroy by entity name, O(n+). Discouraged to use.
	void Destroy(const std::string& name);

private:
	std::unordered_map<size_t, std::unique_ptr<Entity>> entities;

	// entity destruction
	std::vector<size_t> m_DestroyQueue;
	void ExecuteDestroyQueue();

	// root of the scene file
	std::filesystem::path sceneRootAbsolutePath;

	void DestroyChildren(size_t uuidOfParent);
};
