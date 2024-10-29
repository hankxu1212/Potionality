#pragma once

#include "SpriteLoader.h"
#include "../scene/Component.hpp"

class SpriteRenderer : public Component
{
public:
	SpriteRenderer(const std::string& spriteToDraw_);

	void Update() override;

	inline void Activate() { active = true; }
	inline void Deactivate() { active = false; }
	inline bool GetActive() { return active; }

	const std::string spriteToDraw;
	SpriteLoader* spriteLoader = nullptr;

private:
	bool active = true;
};

