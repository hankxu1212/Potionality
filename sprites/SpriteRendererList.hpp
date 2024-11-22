#pragma once

#include "SpriteLoader.h"
#include "../scene/Component.hpp"

#include <vector>

class SpriteRendererList : public Component
{
public:
	void Update() override;

	void AppendSprite(const std::string& newSprite) { sprites.push_back(newSprite); }

	int currentSpriteIndex = -1;
	std::vector<std::string> sprites;

	SpriteLoader* spriteLoader = nullptr;

};

