#pragma once

#include "SpritesheetLoader.h"
#include "../scene/Component.hpp"

class SpritesheetRenderer : public Component
{
public:
	SpritesheetRenderer(const std::string& spriteToDraw_);

	void Update() override;

	void SetFrameRate(float fps) { invFPS = 1.0f / fps; }

	// defines a loop region inside the sprite renderer.
	// Note: loop regions are constrained in single rows only, you may not extend your animation onto multiple rows
	void SetLoopRegion(float row_y, float col_x);

	SpritesheetLoader* spritesheetLoader = nullptr;
	float size_x, size_y;

private:
	std::string spriteToDraw;

 	// uv offsets
	float uv_x = 0;
    float uv_y = 0;
	float loop_y = 0;
	float loop_x = 0;

	// animation
    float invFPS = 1.0 / 3;
    float prevTime;
};

