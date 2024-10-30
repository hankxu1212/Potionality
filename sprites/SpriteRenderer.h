#pragma once

#include "SpriteLoader.h"
#include "../scene/Component.hpp"

class SpriteRenderer : public Component
{
public:
	SpriteRenderer(const std::string& spriteToDraw_);

	void Update() override;

	const std::string spriteToDraw;
	SpriteLoader* spriteLoader = nullptr;

private:
	bool active = true;
};

