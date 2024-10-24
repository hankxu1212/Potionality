#pragma once

#include "SpritesheetLoader.h"
#include "../scene/Component.hpp"

class SpritesheetRenderer : public Component
{
public:
	SpritesheetRenderer(const std::string& spriteToDraw_);

	void Update() override;

	const std::string spriteToDraw;
	SpritesheetLoader* spritesheetLoader = nullptr;
};

