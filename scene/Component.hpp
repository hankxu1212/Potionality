#pragma once

class Entity;
class Scene;
class RectTransform;

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
