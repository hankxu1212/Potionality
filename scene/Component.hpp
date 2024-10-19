#pragma once

class Entity;
class Scene;

class Component
{
public:
	virtual ~Component() = default;

	virtual void Update() {}

	void SetEntity(Entity*);

	Scene* GetScene();

	Entity* entity;
};
