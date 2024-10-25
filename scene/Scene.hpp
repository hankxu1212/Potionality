#pragma once

#include <string>
#include <list>
#include <memory>
#include <map>
#include <utility>

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

	
public: // event functions. Do not create function definitions!
	template<typename T>
	void OnComponentAdded(Entity&, T&);

	template<typename T>
	void OnComponentRemoved(Entity&, T&);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Entity management

	template<typename... TArgs>
	Entity* Instantiate(TArgs&... args) 
	{ 
		entities.emplace_back(std::make_unique<Entity>(this, args...)); 
		return entities.back().get();
	}

	// template taking lvalues
	template<typename... TArgs>
	Entity* Instantiate(TArgs&&... args)
	{
		entities.emplace_back(std::make_unique<Entity>(this, args...));
		return entities.back().get();
	}


private:
	std::list<std::unique_ptr<Entity>> entities;
	std::map<std::pair<float, float>, Entity*> ingredient_locations; // TODO: Better to use glm::vec2 as keys? Kind of a pain though
};