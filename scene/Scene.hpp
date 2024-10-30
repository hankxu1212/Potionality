#pragma once

#include <string>
#include <list>
#include <memory>
#include <glm/glm.hpp>
#include <unordered_map>
#include <mutex>

class Entity;

class Scene
{
public:
	Scene();
	~Scene();

	void Load();

	void Unload();

	void Deserialize(const std::string& path);

	void Update();

	void Render();

public: // event functions. Do not create function definitions!
	template<typename T>
	void OnComponentAdded(Entity&, T&);

	template<typename T>
	void OnComponentRemoved(Entity&, T&);

	void SetEntity(Entity* ent);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Entity management

	template<typename... TArgs>
	Entity* Instantiate(const std::string& name, TArgs&... args)
	{
		Entity* ent = new Entity(this, name, args...);
		SetEntity(ent);
		return ent;
	}

	template<typename... TArgs>
	Entity* Instantiate(const std::string& name, TArgs&&... args)
	{
		Entity* ent = new Entity(this, name, args...);
		SetEntity(ent);
		return ent;
	}

	// destroy by entity handle, O(n). Can use.
	void Destroy(Entity* ent);

	// destroy by entity id O(log n). Should use.
	void Destroy(size_t uuid);

	// destroy by entity name, O(n+). Discouraged to use.
	void Destroy(const std::string& name);

private:
	std::unordered_map<size_t, std::unique_ptr<Entity>> entities;

	std::vector<size_t> m_DestroyQueue;
	void ExecuteDestroyQueue();
};