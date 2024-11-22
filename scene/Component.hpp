#pragma once

class Entity;
class Scene;

#include "UUID.hpp"
#include "RectTransform.hpp"

struct Component
{
	virtual ~Component() = default;

	virtual void Update() {}

	void SetEntity(Entity*);

	RectTransform* GetTransform();
	Scene* GetScene();
	UUID GetEntityID();
	
	void SetActive(bool active) { isActive = active; }

	Entity* entity;
	bool isActive = true;
};

#define SETUP_EMPTY_CALLBACKS(T)																\
template<>																				\
void Scene::OnComponentAdded<T>(Entity& entity, T& component)				\
{																						\
}																						\
template<>																				\
void Scene::OnComponentRemoved<T>(Entity& entity, T& component)			\
{																						\
}