#pragma once

class Entity;
class Scene;

#include "RectTransform.hpp"

class Component
{
public:
	virtual ~Component() = default;

	virtual void Update() {}

	void SetEntity(Entity*);

	RectTransform* GetTransform();
	Scene* GetScene();

	Entity* entity;
};
