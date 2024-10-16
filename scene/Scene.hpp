#pragma once

#include <string>
#include "Entity.hpp"

class Scene
{
public:
	Scene() = default;
	~Scene();

	void Load(const std::string& path);
	void Unload();

	void Update();

	void Render();
};