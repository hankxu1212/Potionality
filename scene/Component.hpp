#pragma once

class Entity;
class Scene;

#include "UUID.hpp"
#include "RectTransform.hpp"

class Component
{
public:
	virtual ~Component() = default;

	virtual void Update() {}

	void SetEntity(Entity*);

	RectTransform* GetTransform();
	Scene* GetScene();
	UUID GetEntityID();

	Entity* entity;
};
