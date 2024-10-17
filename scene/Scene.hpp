#pragma once

#include <string>
#include "Entity.hpp"

class Scene
{
public:
	Scene();
	~Scene();

	void Unload();

	void Update();

	void Render();

private:
	std::vector<std::unique_ptr<Entity>> entities;
};