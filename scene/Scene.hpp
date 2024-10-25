#pragma once

#include <string>
#include <list>
#include <memory>
#include <map>
#include <utility>
#include <glm/glm.hpp>

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

	Entity* GetClosestIngredient(glm::vec2 playerPos) {
		float min_distance = -1;
		Entity* closest = nullptr;
		for (const auto& mapping : ingredient_locations) {
			std::pair<float, float> coord = mapping.first;
			float distance = glm::distance(playerPos, glm::vec2{coord.first, coord.second});
			if (distance < min_distance || min_distance == -1) {
				min_distance = distance;
				closest = mapping.second;
			}
		}
		return closest;
	}


private:
	std::list<std::unique_ptr<Entity>> entities;
	std::map<std::pair<float, float>, Entity*> ingredient_locations; // TODO: Better to use glm::vec2 as keys? Kind of a pain though
};